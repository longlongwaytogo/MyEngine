#include <gfx/rendergraph/RenderPass.h>

namespace gfx {

RenderGraphPass::RenderGraphPass(const Desc& desc) 
    : m_name(desc.name),
      m_inputDependencies(desc.inputDependencies),
      m_outputDependencies(desc.outputDependencies),
      m_colorAttachments(desc.colorAttachments),
      m_depthStencilAttachment(desc.depthStencilAttachment),
      m_usesDepthStencil(desc.usesDepthStencil),
      m_execute(desc.execute) {}

RenderGraphPass::~RenderGraphPass() {}

const std::string& RenderGraphPass::getName() const {
    return m_name;
}

const std::vector<RenderGraphPass::ResourceDependency>& RenderGraphPass::getInputDependencies() const {
    return m_inputDependencies;
}

const std::vector<RenderGraphPass::ResourceDependency>& RenderGraphPass::getOutputDependencies() const {
    return m_outputDependencies;
}

const std::vector<RenderGraphPass::ColorAttachment>& RenderGraphPass::getColorAttachments() const {
    return m_colorAttachments;
}

const RenderGraphPass::DepthStencilAttachment& RenderGraphPass::getDepthStencilAttachment() const {
    return m_depthStencilAttachment;
}

bool RenderGraphPass::usesDepthStencil() const {
    return m_usesDepthStencil;
}

void RenderGraphPass::execute(CommandBuffer* cmdBuffer) {
    if (m_execute) {
        m_execute(cmdBuffer);
    }
}

}
