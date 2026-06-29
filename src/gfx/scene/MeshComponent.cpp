#include <gfx/scene/MeshComponent.h>

namespace gfx {

MeshComponent::MeshComponent() {}

void MeshComponent::setMesh(std::shared_ptr<Mesh> mesh) {
    m_mesh = mesh;
}

std::shared_ptr<Mesh> MeshComponent::getMesh() const {
    return m_mesh;
}

void MeshComponent::setMaterial(std::shared_ptr<Material> material) {
    m_material = material;
}

std::shared_ptr<Material> MeshComponent::getMaterial() const {
    return m_material;
}

bool MeshComponent::isVisible() const {
    return m_visible;
}

void MeshComponent::setVisible(bool visible) {
    m_visible = visible;
}

bool MeshComponent::isCastShadow() const {
    return m_castShadow;
}

void MeshComponent::setCastShadow(bool cast) {
    m_castShadow = cast;
}

bool MeshComponent::isReceiveShadow() const {
    return m_receiveShadow;
}

void MeshComponent::setReceiveShadow(bool receive) {
    m_receiveShadow = receive;
}

u32 MeshComponent::getRenderLayer() const {
    return m_renderLayer;
}

void MeshComponent::setRenderLayer(u32 layer) {
    m_renderLayer = layer;
}

}
