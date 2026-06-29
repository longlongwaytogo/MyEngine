#include <gfx/rendergraph/Resource.h>
#include <gfx/hal/Device.h>

namespace gfx {

RenderGraphResource::RenderGraphResource(const std::string& name, ResourceType type) 
    : m_name(name), m_type(type) {}

const std::string& RenderGraphResource::getName() const {
    return m_name;
}

ResourceType RenderGraphResource::getType() const {
    return m_type;
}

void RenderGraphResource::setAccess(ResourceAccess access) {
    m_access = access;
}

ResourceAccess RenderGraphResource::getAccess() const {
    return m_access;
}

void RenderGraphResource::setState(ResourceState state) {
    m_state = state;
}

ResourceState RenderGraphResource::getState() const {
    return m_state;
}

std::shared_ptr<Texture> RenderGraphResource::getTexture() const {
    return nullptr;
}

std::shared_ptr<Buffer> RenderGraphResource::getBuffer() const {
    return nullptr;
}

TextureResource::TextureResource(const std::string& name, const TextureDesc& desc) 
    : RenderGraphResource(name, ResourceType::Texture), m_desc(desc) {}

std::shared_ptr<Texture> TextureResource::getTexture() const {
    return m_texture;
}

const TextureDesc& TextureResource::getDesc() const {
    return m_desc;
}

BufferResource::BufferResource(const std::string& name, const BufferDesc& desc) 
    : RenderGraphResource(name, ResourceType::Buffer), m_desc(desc) {}

std::shared_ptr<Buffer> BufferResource::getBuffer() const {
    return m_buffer;
}

const BufferDesc& BufferResource::getDesc() const {
    return m_desc;
}

}
