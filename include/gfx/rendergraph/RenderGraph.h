#pragma once

#include <gfx/rendergraph/Resource.h>
#include <gfx/rendergraph/RenderPass.h>
#include <gfx/hal/Device.h>
#include <gfx/hal/CommandBuffer.h>
#include <gfx/hal/Swapchain.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace gfx {

class RenderGraph {
public:
    RenderGraph(Device* device);
    ~RenderGraph();
    
    void addResource(std::unique_ptr<RenderGraphResource> resource);
    void addPass(std::unique_ptr<RenderGraphPass> pass);
    
    void addTextureResource(const std::string& name, const TextureDesc& desc);
    void addBufferResource(const std::string& name, const BufferDesc& desc);
    
    RenderGraphResource* getResource(const std::string& name);
    
    void compile();
    void execute(Swapchain* swapchain = nullptr);
    
    void reset();
    
private:
    struct PassNode {
        RenderGraphPass* pass;
        std::vector<PassNode*> dependencies;
        std::vector<PassNode*> dependents;
        bool visited = false;
        bool inStack = false;
    };
    
    Device* m_device;
    std::map<std::string, std::unique_ptr<RenderGraphResource>> m_resources;
    std::vector<std::unique_ptr<RenderGraphPass>> m_passes;
    std::vector<PassNode> m_passNodes;
    
    bool m_compiled = false;
    
    void buildDependencyGraph();
    bool detectCycles(PassNode* node);
    std::vector<PassNode*> topologicalSort();
    
    void recordCommandBuffer(CommandBuffer* cmdBuffer, RenderGraphPass* pass);
    void executePasses(const std::vector<PassNode*>& sortedPasses, Swapchain* swapchain);
    
    void executePassesParallel(const std::vector<PassNode*>& sortedPasses);
    void executePassThread(PassNode* node);
    
    std::vector<std::thread> m_threads;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    size_t m_completedPasses = 0;
};

}
