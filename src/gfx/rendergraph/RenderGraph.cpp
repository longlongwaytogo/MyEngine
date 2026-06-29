#include <gfx/rendergraph/RenderGraph.h>
#include <gfx/core/logging.h>
#include <gfx/hal/Device.h>
#include <algorithm>

namespace gfx {

RenderGraph::RenderGraph(Device* device) : m_device(device) {
    GFX_LOG_INFO("RenderGraph created");
}

RenderGraph::~RenderGraph() {
    reset();
    GFX_LOG_INFO("RenderGraph destroyed");
}

void RenderGraph::addResource(std::unique_ptr<RenderGraphResource> resource) {
    m_resources[resource->getName()] = std::move(resource);
    m_compiled = false;
}

void RenderGraph::addPass(std::unique_ptr<RenderGraphPass> pass) {
    m_passes.push_back(std::move(pass));
    m_compiled = false;
}

void RenderGraph::addTextureResource(const std::string& name, const TextureDesc& desc) {
    auto resource = std::make_unique<TextureResource>(name, desc);
    m_resources[name] = std::move(resource);
    m_compiled = false;
}

void RenderGraph::addBufferResource(const std::string& name, const BufferDesc& desc) {
    auto resource = std::make_unique<BufferResource>(name, desc);
    m_resources[name] = std::move(resource);
    m_compiled = false;
}

RenderGraphResource* RenderGraph::getResource(const std::string& name) {
    auto it = m_resources.find(name);
    return it != m_resources.end() ? it->second.get() : nullptr;
}

void RenderGraph::buildDependencyGraph() {
    m_passNodes.clear();
    m_passNodes.resize(m_passes.size());
    
    for (size_t i = 0; i < m_passes.size(); ++i) {
        m_passNodes[i].pass = m_passes[i].get();
        m_passNodes[i].visited = false;
        m_passNodes[i].inStack = false;
    }
    
    std::map<std::string, std::vector<size_t>> resourceWriters;
    
    for (size_t i = 0; i < m_passes.size(); ++i) {
        auto& outputs = m_passes[i]->getOutputDependencies();
        for (auto& output : outputs) {
            resourceWriters[output.resourceName].push_back(i);
        }
    }
    
    for (size_t i = 0; i < m_passes.size(); ++i) {
        auto& inputs = m_passes[i]->getInputDependencies();
        for (auto& input : inputs) {
            auto it = resourceWriters.find(input.resourceName);
            if (it != resourceWriters.end()) {
                for (auto writerIdx : it->second) {
                    m_passNodes[i].dependencies.push_back(&m_passNodes[writerIdx]);
                    m_passNodes[writerIdx].dependents.push_back(&m_passNodes[i]);
                }
            }
        }
    }
}

bool RenderGraph::detectCycles(PassNode* node) {
    node->visited = true;
    node->inStack = true;
    
    for (auto dependent : node->dependents) {
        if (!dependent->visited) {
            if (detectCycles(dependent)) {
                return true;
            }
        } else if (dependent->inStack) {
            GFX_LOG_ERROR("Cycle detected in render graph");
            return true;
        }
    }
    
    node->inStack = false;
    return false;
}

std::vector<RenderGraph::PassNode*> RenderGraph::topologicalSort() {
    std::vector<PassNode*> result;
    
    auto inDegree = [](PassNode* node) {
        return node->dependencies.size();
    };
    
    std::vector<PassNode*> queue;
    
    for (auto& node : m_passNodes) {
        node.visited = false;
        node.inStack = false;
        if (inDegree(&node) == 0) {
            queue.push_back(&node);
        }
    }
    
    while (!queue.empty()) {
        auto node = queue.back();
        queue.pop_back();
        result.push_back(node);
        
        for (auto dependent : node->dependents) {
            dependent->dependencies.erase(
                std::remove(dependent->dependencies.begin(), 
                           dependent->dependencies.end(), node),
                dependent->dependencies.end()
            );
            
            if (dependent->dependencies.empty()) {
                queue.push_back(dependent);
            }
        }
    }
    
    if (result.size() != m_passNodes.size()) {
        GFX_LOG_ERROR("Topological sort failed - cycle detected");
        return {};
    }
    
    return result;
}

void RenderGraph::compile() {
    buildDependencyGraph();
    
    for (auto& node : m_passNodes) {
        node.visited = false;
        node.inStack = false;
    }
    
    for (auto& node : m_passNodes) {
        if (!node.visited) {
            detectCycles(&node);
        }
    }
    
    m_compiled = true;
    GFX_LOG_INFO("RenderGraph compiled with %zu passes", m_passes.size());
}

void RenderGraph::recordCommandBuffer(CommandBuffer* cmdBuffer, RenderGraphPass* pass) {
    cmdBuffer->begin();
    
    for (auto& colorAttachment : pass->getColorAttachments()) {
        auto resource = getResource(colorAttachment.resourceName);
        if (resource && resource->getType() == ResourceType::Texture) {
            auto textureResource = static_cast<TextureResource*>(resource);
        }
    }
    
    if (pass->usesDepthStencil()) {
        auto resource = getResource(pass->getDepthStencilAttachment().resourceName);
        if (resource) {
        }
    }
    
    pass->execute(cmdBuffer);
    
    cmdBuffer->end();
}

void RenderGraph::executePasses(const std::vector<PassNode*>& sortedPasses, Swapchain* swapchain) {
    for (auto node : sortedPasses) {
        auto cmdBuffer = m_device->createCommandBuffer();
        recordCommandBuffer(cmdBuffer.get(), node->pass);
        m_device->submit(cmdBuffer.get(), {}, {}, nullptr);
    }
}

void RenderGraph::executePassThread(PassNode* node) {
    auto cmdBuffer = m_device->createCommandBuffer();
    recordCommandBuffer(cmdBuffer.get(), node->pass);
    m_device->submit(cmdBuffer.get(), {}, {}, nullptr);
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_completedPasses++;
    m_cv.notify_all();
}

void RenderGraph::executePassesParallel(const std::vector<PassNode*>& sortedPasses) {
    m_completedPasses = 0;
    m_threads.clear();
    
    for (auto node : sortedPasses) {
        m_threads.emplace_back(&RenderGraph::executePassThread, this, node);
    }
    
    for (auto& thread : m_threads) {
        thread.join();
    }
}

void RenderGraph::execute(Swapchain* swapchain) {
    if (!m_compiled) {
        compile();
    }
    
    auto sortedPasses = topologicalSort();
    
    if (sortedPasses.empty()) {
        GFX_LOG_ERROR("Failed to execute render graph - cycle detected or no passes");
        return;
    }
    
    if (m_device && sortedPasses.size() > 1) {
        executePassesParallel(sortedPasses);
    } else {
        executePasses(sortedPasses, swapchain);
    }
    
    if (swapchain) {
        swapchain->present({});
    }
    
    m_device->waitIdle();
}

void RenderGraph::reset() {
    m_resources.clear();
    m_passes.clear();
    m_passNodes.clear();
    m_compiled = false;
    
    for (auto& thread : m_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    m_threads.clear();
}

}
