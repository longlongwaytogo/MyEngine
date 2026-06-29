#include <gfx/shader/ShaderCompiler.h>
#include <gfx/core/logging.h>
#include <slang.h>
#include <slang-com-ptr.h>
#include <fstream>
#include <sstream>

namespace gfx {

class SlangShaderCompiler : public ShaderCompiler {
public:
    SlangShaderCompiler();
    ~SlangShaderCompiler() override;
    
    ShaderCompiler::CompileResult compileFromSource(const std::string& source, 
                                                    const ShaderCompiler::CompileOptions& options) override;
    
    ShaderCompiler::CompileResult compileFromFile(const std::string& filePath, 
                                                  const ShaderCompiler::CompileOptions& options) override;
    
    std::unique_ptr<Shader> compileToShader(const std::string& source,
                                             Shader::Stage stage,
                                             const ShaderCompiler::CompileOptions& options) override;
    
    std::unique_ptr<Shader> compileFileToShader(const std::string& filePath,
                                                Shader::Stage stage,
                                                const ShaderCompiler::CompileOptions& options) override;
    
    bool compileOffline(const std::string& sourcePath,
                        const std::string& outputPath,
                        Shader::Stage stage,
                        const ShaderCompiler::CompileOptions& options) override;
    
private:
    Slang::ComPtr<slang::IGlobalSession> m_globalSession;
    Slang::ComPtr<slang::ISession> m_session;
    
    SlangCompileTarget getSlangTarget(const ShaderCompiler::CompileOptions& options);
    Shader::Format getOutputFormat(const ShaderCompiler::CompileOptions& options);
    SlangStage getSlangStage(Shader::Stage stage);
};

SlangShaderCompiler::SlangShaderCompiler() {
    SlangResult result = slang::createGlobalSession(m_globalSession.writeRef());
    if (SLANG_FAILED(result)) {
        GFX_LOG_ERROR("Failed to create Slang global session");
        return;
    }
    
    slang::SessionDesc sessionDesc;
    result = m_globalSession->createSession(sessionDesc, m_session.writeRef());
    if (SLANG_FAILED(result)) {
        GFX_LOG_ERROR("Failed to create Slang session");
        return;
    }
    
    GFX_LOG_INFO("Slang ShaderCompiler created");
}

SlangShaderCompiler::~SlangShaderCompiler() {
    GFX_LOG_INFO("Slang ShaderCompiler destroyed");
}

SlangCompileTarget SlangShaderCompiler::getSlangTarget(const ShaderCompiler::CompileOptions& options) {
    switch (options.target) {
        case ShaderCompiler::Target::GLSL_450:
            return SLANG_GLSL;
        case ShaderCompiler::Target::GLSL_ES_300:
            return SLANG_GLSL;
        case ShaderCompiler::Target::HLSL_SM5:
            return SLANG_HLSL;
        case ShaderCompiler::Target::HLSL_SM6:
            return SLANG_HLSL;
        case ShaderCompiler::Target::SPIRV_1_5:
            return SLANG_SPIRV;
        case ShaderCompiler::Target::SPIRV_1_6:
            return SLANG_SPIRV;
        default:
            return SLANG_GLSL;
    }
}

Shader::Format SlangShaderCompiler::getOutputFormat(const ShaderCompiler::CompileOptions& options) {
    switch (options.target) {
        case ShaderCompiler::Target::GLSL_450:
        case ShaderCompiler::Target::GLSL_ES_300:
            return Shader::Format::GLSL;
        case ShaderCompiler::Target::HLSL_SM5:
        case ShaderCompiler::Target::HLSL_SM6:
            return Shader::Format::HLSL;
        case ShaderCompiler::Target::SPIRV_1_5:
        case ShaderCompiler::Target::SPIRV_1_6:
            return Shader::Format::SPIRV;
        default:
            return Shader::Format::GLSL;
    }
}

SlangStage SlangShaderCompiler::getSlangStage(Shader::Stage stage) {
    switch (stage) {
        case Shader::Stage::Vertex: return SLANG_STAGE_VERTEX;
        case Shader::Stage::Fragment: return SLANG_STAGE_FRAGMENT;
        case Shader::Stage::Geometry: return SLANG_STAGE_GEOMETRY;
        case Shader::Stage::Compute: return SLANG_STAGE_COMPUTE;
        case Shader::Stage::TessellationControl: return SLANG_STAGE_HULL;
        case Shader::Stage::TessellationEvaluation: return SLANG_STAGE_DOMAIN;
        default: return SLANG_STAGE_VERTEX;
    }
}

ShaderCompiler::CompileResult SlangShaderCompiler::compileFromSource(const std::string& source, 
                                                                     const ShaderCompiler::CompileOptions& options) {
    ShaderCompiler::CompileResult result;
    result.outputFormat = getOutputFormat(options);
    
    auto target = getSlangTarget(options);
    
    Slang::ComPtr<slang::ICompileRequest> request;
    SlangResult compileResult = m_session->createCompileRequest(request.writeRef());
    if (SLANG_FAILED(compileResult)) {
        result.success = false;
        result.errorMessage = "Failed to create compile request";
        return result;
    }
    
    request->setCodeGenTarget(target);
    
    for (const auto& define : options.defines) {
        request->addPreprocessorDefine(define.first.c_str(), define.second.c_str());
    }
    
    for (const auto& includePath : options.includePaths) {
        request->addSearchPath(includePath.c_str());
    }
    
    int tuIndex = request->addTranslationUnit(SLANG_SOURCE_LANGUAGE_SLANG, "shader");
    request->addTranslationUnitSourceString(tuIndex, "shader", source.c_str());
    request->addEntryPoint(tuIndex, options.entryPoint.c_str(), SLANG_STAGE_VERTEX);
    
    compileResult = request->compile();
    
    const char* diagnostics = request->getDiagnosticOutput();
    if (diagnostics && strlen(diagnostics) > 0) {
        result.errorMessage = std::string(diagnostics);
        if (SLANG_FAILED(compileResult)) {
            GFX_LOG_ERROR("Slang compilation error:\n%s", result.errorMessage.c_str());
        }
    }
    
    if (SLANG_FAILED(compileResult)) {
        result.success = false;
        return result;
    }
    
    Slang::ComPtr<ISlangBlob> codeBlob;
    compileResult = request->getEntryPointCodeBlob(0, 0, codeBlob.writeRef());
    if (SLANG_FAILED(compileResult) || !codeBlob) {
        result.success = false;
        result.errorMessage = "Failed to get compiled code";
        return result;
    }
    
    result.binary.resize(codeBlob->getBufferSize());
    memcpy(result.binary.data(), codeBlob->getBufferPointer(), codeBlob->getBufferSize());
    result.success = true;
    
    GFX_LOG_INFO("Slang shader compiled successfully");
    return result;
}

ShaderCompiler::CompileResult SlangShaderCompiler::compileFromFile(const std::string& filePath, 
                                                                   const ShaderCompiler::CompileOptions& options) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        ShaderCompiler::CompileResult result;
        result.success = false;
        result.errorMessage = "Failed to open file: " + filePath;
        return result;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return compileFromSource(buffer.str(), options);
}

std::unique_ptr<Shader> SlangShaderCompiler::compileToShader(const std::string& source,
                                                              Shader::Stage stage,
                                                              const ShaderCompiler::CompileOptions& options) {
    auto compileResult = compileFromSource(source, options);
    if (!compileResult.success) {
        return nullptr;
    }
    
    Shader::Desc desc;
    desc.stage = stage;
    desc.format = compileResult.outputFormat;
    desc.binary = std::move(compileResult.binary);
    desc.source = source;
    desc.entryPoint = options.entryPoint;
    
    return std::make_unique<Shader>(desc);
}

std::unique_ptr<Shader> SlangShaderCompiler::compileFileToShader(const std::string& filePath,
                                                                 Shader::Stage stage,
                                                                 const ShaderCompiler::CompileOptions& options) {
    auto compileResult = compileFromFile(filePath, options);
    if (!compileResult.success) {
        return nullptr;
    }
    
    Shader::Desc desc;
    desc.stage = stage;
    desc.format = compileResult.outputFormat;
    desc.binary = std::move(compileResult.binary);
    desc.entryPoint = options.entryPoint;
    
    return std::make_unique<Shader>(desc);
}

bool SlangShaderCompiler::compileOffline(const std::string& sourcePath,
                                         const std::string& outputPath,
                                         Shader::Stage stage,
                                         const ShaderCompiler::CompileOptions& options) {
    auto compileResult = compileFromFile(sourcePath, options);
    if (!compileResult.success) {
        return false;
    }
    
    std::ofstream outputFile(outputPath, std::ios::binary);
    if (!outputFile.is_open()) {
        GFX_LOG_ERROR("Failed to open output file: %s", outputPath.c_str());
        return false;
    }
    
    outputFile.write(reinterpret_cast<const char*>(compileResult.binary.data()), 
                     compileResult.binary.size());
    outputFile.close();
    
    GFX_LOG_INFO("Shader compiled offline to: %s", outputPath.c_str());
    return true;
}

std::unique_ptr<ShaderCompiler> ShaderCompiler::create() {
    return std::make_unique<SlangShaderCompiler>();
}

}