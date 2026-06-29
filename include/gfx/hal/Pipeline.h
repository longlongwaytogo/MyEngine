#pragma once

#include <gfx/core/types.h>
#include <gfx/hal/RenderPass.h>
#include <vector>
#include <memory>
#include <string>

namespace gfx {

class RenderPass;

enum class ShaderStage {
    VERTEX = 0,
    TESSELLATION_CONTROL = 1,
    TESSELLATION_EVALUATION = 2,
    GEOMETRY = 3,
    FRAGMENT = 4,
    COMPUTE = 5
};

enum class PrimitiveTopology {
    POINT_LIST = 0,
    LINE_LIST = 1,
    LINE_STRIP = 2,
    TRIANGLE_LIST = 3,
    TRIANGLE_STRIP = 4,
    TRIANGLE_FAN = 5,
    LINE_LIST_ADJACENCY = 6,
    LINE_STRIP_ADJACENCY = 7,
    TRIANGLE_LIST_ADJACENCY = 8,
    TRIANGLE_STRIP_ADJACENCY = 9,
    PATCH_LIST = 10
};

enum class PolygonMode {
    FILL = 0,
    LINE = 1,
    POINT = 2
};

enum class CullMode {
    NONE = 0,
    FRONT = 1,
    BACK = 2,
    FRONT_AND_BACK = 3
};

enum class FrontFace {
    CLOCKWISE = 0,
    COUNTER_CLOCKWISE = 1
};

enum class BlendFactor {
    ZERO = 0,
    ONE = 1,
    SRC_COLOR = 2,
    ONE_MINUS_SRC_COLOR = 3,
    DST_COLOR = 4,
    ONE_MINUS_DST_COLOR = 5,
    SRC_ALPHA = 6,
    ONE_MINUS_SRC_ALPHA = 7,
    DST_ALPHA = 8,
    ONE_MINUS_DST_ALPHA = 9,
    CONSTANT_COLOR = 10,
    ONE_MINUS_CONSTANT_COLOR = 11,
    CONSTANT_ALPHA = 12,
    ONE_MINUS_CONSTANT_ALPHA = 13,
    SRC_ALPHA_SATURATE = 14,
    SRC1_COLOR = 15,
    ONE_MINUS_SRC1_COLOR = 16,
    SRC1_ALPHA = 17,
    ONE_MINUS_SRC1_ALPHA = 18
};

enum class BlendOp {
    ADD = 0,
    SUBTRACT = 1,
    REVERSE_SUBTRACT = 2,
    MIN = 3,
    MAX = 4
};

struct VertexInputAttribute {
    u32 location;
    u32 binding;
    Format format;
    u32 offset;
};

struct VertexInputBinding {
    u32 binding;
    u32 stride;
    bool instanced = false;
};

struct ShaderModule {
    ShaderStage stage;
    const void* code;
    size_t size;
    std::string entryPoint = "main";
};

struct RasterizerState {
    PolygonMode polygonMode = PolygonMode::FILL;
    CullMode cullMode = CullMode::BACK;
    FrontFace frontFace = FrontFace::COUNTER_CLOCKWISE;
    bool depthClampEnable = false;
    bool rasterizerDiscardEnable = false;
    f32 depthBiasConstantFactor = 0.0f;
    f32 depthBiasClamp = 0.0f;
    f32 depthBiasSlopeFactor = 0.0f;
    f32 lineWidth = 1.0f;
};

struct ColorBlendAttachment {
    bool blendEnable = false;
    BlendFactor srcColorBlendFactor = BlendFactor::ONE;
    BlendFactor dstColorBlendFactor = BlendFactor::ZERO;
    BlendOp colorBlendOp = BlendOp::ADD;
    BlendFactor srcAlphaBlendFactor = BlendFactor::ONE;
    BlendFactor dstAlphaBlendFactor = BlendFactor::ZERO;
    BlendOp alphaBlendOp = BlendOp::ADD;
    u32 colorWriteMask = 0xF;
};

struct DepthStencilState {
    bool depthTestEnable = true;
    bool depthWriteEnable = true;
    bool stencilTestEnable = false;
};

struct PipelineLayoutDesc {
    std::vector<u32> pushConstantRanges;
};

struct PipelineDesc {
    std::vector<ShaderModule> shaders;
    std::vector<VertexInputAttribute> vertexAttributes;
    std::vector<VertexInputBinding> vertexBindings;
    PrimitiveTopology topology = PrimitiveTopology::TRIANGLE_LIST;
    RasterizerState rasterizer;
    std::vector<ColorBlendAttachment> colorBlendAttachments;
    DepthStencilState depthStencil;
    PipelineLayoutDesc layout;
    RenderPass* renderPass = nullptr;
    u32 subpass = 0;
};

class PipelineLayout {
public:
    virtual ~PipelineLayout() = default;
    
    virtual const PipelineLayoutDesc& getDesc() const = 0;
    
    virtual void* getNativeHandle() const = 0;
};

class Pipeline {
public:
    virtual ~Pipeline() = default;
    
    virtual const PipelineDesc& getDesc() const = 0;
    
    virtual PipelineLayout* getLayout() const = 0;
    
    virtual void* getNativeHandle() const = 0;
};

}
