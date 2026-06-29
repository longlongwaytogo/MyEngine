#pragma once

#include <gfx/core/types.h>
#include <gfx/hal/Texture.h>
#include <gfx/hal/Buffer.h>
#include <string>
#include <memory>
#include <variant>

namespace gfx {

enum class ResourceType {
    Texture,
    Buffer,
    SwapchainImage
};

enum class ResourceAccess {
    None,
    Read,
    Write,
    ReadWrite
};

enum class ResourceState {
    Undefined,
    Present,
    ColorAttachment,
    DepthAttachment,
    ShaderRead,
    ShaderWrite,
    TransferSrc,
    TransferDst
};

class RenderGraphResource {
public:
    RenderGraphResource(const std::string& name, ResourceType type);
    virtual ~RenderGraphResource() = default;
    
    const std::string& getName() const;
    ResourceType getType() const;
    
    void setAccess(ResourceAccess access);
    ResourceAccess getAccess() const;
    
    void setState(ResourceState state);
    ResourceState getState() const;
    
    virtual std::shared_ptr<Texture> getTexture() const;
    virtual std::shared_ptr<Buffer> getBuffer() const;
    
protected:
    std::string m_name;
    ResourceType m_type;
    ResourceAccess m_access = ResourceAccess::None;
    ResourceState m_state = ResourceState::Undefined;
};

class TextureResource : public RenderGraphResource {
public:
    TextureResource(const std::string& name, const TextureDesc& desc);
    
    std::shared_ptr<Texture> getTexture() const override;
    const TextureDesc& getDesc() const;
    
private:
    std::shared_ptr<Texture> m_texture;
    TextureDesc m_desc;
};

class BufferResource : public RenderGraphResource {
public:
    BufferResource(const std::string& name, const BufferDesc& desc);
    
    std::shared_ptr<Buffer> getBuffer() const override;
    const BufferDesc& getDesc() const;
    
private:
    std::shared_ptr<Buffer> m_buffer;
    BufferDesc m_desc;
};

}
