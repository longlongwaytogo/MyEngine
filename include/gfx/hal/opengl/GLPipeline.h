#pragma once

#include <gfx/hal/Pipeline.h>

namespace gfx {

class GLPipeline : public Pipeline {
public:
    GLPipeline(const PipelineDesc& desc);
    ~GLPipeline() override;
    
    const PipelineDesc& getDesc() const override;
    PipelineLayout* getLayout() const override;
    void* getNativeHandle() const override;
    
private:
    PipelineDesc m_desc;
    unsigned int m_program = 0;
};

}
