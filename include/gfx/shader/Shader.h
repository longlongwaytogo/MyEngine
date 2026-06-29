#pragma once

#include <gfx/core/types.h>
#include <string>
#include <vector>
#include <memory>

namespace gfx {

class Shader {
public:
    enum class Stage {
        Vertex,
        Fragment,
        Geometry,
        TessellationControl,
        TessellationEvaluation,
        Compute
    };
    
    enum class Format {
        GLSL,
        HLSL,
        SPIRV,
        DXBC,
        DXIL
    };
    
    struct Desc {
        Stage stage;
        Format format;
        std::vector<u8> binary;
        std::string source;
        std::string entryPoint;
    };
    
    Shader(const Desc& desc);
    ~Shader();
    
    Stage getStage() const;
    Format getFormat() const;
    const std::vector<u8>& getBinary() const;
    const std::string& getSource() const;
    const std::string& getEntryPoint() const;
    
private:
    Stage m_stage;
    Format m_format;
    std::vector<u8> m_binary;
    std::string m_source;
    std::string m_entryPoint;
};

}
