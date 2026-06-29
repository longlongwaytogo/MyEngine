#include <gfx/shader/Shader.h>

namespace gfx {

Shader::Shader(const Desc& desc) 
    : m_stage(desc.stage), 
      m_format(desc.format),
      m_binary(desc.binary),
      m_source(desc.source),
      m_entryPoint(desc.entryPoint) {}

Shader::~Shader() {}

Shader::Stage Shader::getStage() const {
    return m_stage;
}

Shader::Format Shader::getFormat() const {
    return m_format;
}

const std::vector<u8>& Shader::getBinary() const {
    return m_binary;
}

const std::string& Shader::getSource() const {
    return m_source;
}

const std::string& Shader::getEntryPoint() const {
    return m_entryPoint;
}

}
