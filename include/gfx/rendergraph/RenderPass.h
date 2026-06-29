#pragma once

#include <gfx/rendergraph/Resource.h>
#include <gfx/hal/CommandBuffer.h>
#include <gfx/core/types.h>
#include <vector>
#include <string>
#include <functional>

namespace gfx {

class RenderGraphPass {
public:
    struct ResourceDependency {
        std::string resourceName;
        ResourceAccess access;
        ResourceState beforeState;
        ResourceState afterState;
    };
    
    struct ColorAttachment {
        std::string resourceName;
        ClearValue clearValue;
        bool loadOpClear = true;
        bool storeOpStore = true;
    };
    
    struct DepthStencilAttachment {
        std::string resourceName;
        ClearValue clearValue;
        bool loadOpClear = true;
        bool storeOpStore = true;
    };
    
    struct Desc {
        std::string name;
        std::vector<ResourceDependency> inputDependencies;
        std::vector<ResourceDependency> outputDependencies;
        std::vector<ColorAttachment> colorAttachments;
        DepthStencilAttachment depthStencilAttachment;
        Extent2D renderArea;
        bool usesDepthStencil = false;
        std::function<void(CommandBuffer*)> execute;
    };
    
    RenderGraphPass(const Desc& desc);
    ~RenderGraphPass();
    
    const std::string& getName() const;
    const std::vector<ResourceDependency>& getInputDependencies() const;
    const std::vector<ResourceDependency>& getOutputDependencies() const;
    const std::vector<ColorAttachment>& getColorAttachments() const;
    const DepthStencilAttachment& getDepthStencilAttachment() const;
    bool usesDepthStencil() const;
    
    void execute(CommandBuffer* cmdBuffer);
    
private:
    std::string m_name;
    std::vector<ResourceDependency> m_inputDependencies;
    std::vector<ResourceDependency> m_outputDependencies;
    std::vector<ColorAttachment> m_colorAttachments;
    DepthStencilAttachment m_depthStencilAttachment;
    bool m_usesDepthStencil;
    std::function<void(CommandBuffer*)> m_execute;
};

}
