#pragma once

#include <gfx/shader/Shader.h>
#include <string>
#include <vector>
#include <memory>

namespace gfx {

class ShaderCompiler {
public:
    enum class Target {
        GLSL_450,
        GLSL_ES_300,
        HLSL_SM5,
        HLSL_SM6,
        SPIRV_1_5,
        SPIRV_1_6
    };
    
    struct CompileOptions {
        Target target;
        bool optimize = true;
        bool debugInfo = false;
        std::vector<std::string> includePaths;
        std::vector<std::pair<std::string, std::string>> defines;
        std::string entryPoint = "main";
    };
    
    struct CompileResult {
        bool success = false;
        std::vector<u8> binary;
        std::string sourceCode;
        std::string errorMessage;
        Shader::Format outputFormat;
    };
    
    static std::unique_ptr<ShaderCompiler> create();
    
    virtual ~ShaderCompiler() = default;
    
    virtual CompileResult compileFromSource(const std::string& source, 
                                            const CompileOptions& options) = 0;
    
    virtual CompileResult compileFromFile(const std::string& filePath, 
                                          const CompileOptions& options) = 0;
    
    virtual std::unique_ptr<Shader> compileToShader(const std::string& source,
                                                     Shader::Stage stage,
                                                     const CompileOptions& options) = 0;
    
    virtual std::unique_ptr<Shader> compileFileToShader(const std::string& filePath,
                                                        Shader::Stage stage,
                                                        const CompileOptions& options) = 0;
    
    virtual bool compileOffline(const std::string& sourcePath,
                                const std::string& outputPath,
                                Shader::Stage stage,
                                const CompileOptions& options) = 0;
};

}
