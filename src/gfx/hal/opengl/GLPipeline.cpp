#include <gfx/hal/opengl/GLPipeline.h>
#include <glad/gl.h>

namespace gfx {

GLPipeline::GLPipeline(const PipelineDesc& desc) : m_desc(desc) {
    m_program = glCreateProgram();
}

GLPipeline::~GLPipeline() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
    }
}

const PipelineDesc& GLPipeline::getDesc() const {
    return m_desc;
}

PipelineLayout* GLPipeline::getLayout() const {
    return nullptr;
}

void* GLPipeline::getNativeHandle() const {
    return reinterpret_cast<void*>(m_program);
}

}
