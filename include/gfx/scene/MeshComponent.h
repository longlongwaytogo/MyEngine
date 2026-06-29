#pragma once

#include <gfx/scene/Component.h>
#include <gfx/scene/Mesh.h>
#include <memory>

namespace gfx {

class Material;

class MeshComponent : public TypedComponent<MeshComponent> {
public:
    MeshComponent();
    ~MeshComponent() override = default;
    
    void setMesh(std::shared_ptr<Mesh> mesh);
    std::shared_ptr<Mesh> getMesh() const;
    
    void setMaterial(std::shared_ptr<Material> material);
    std::shared_ptr<Material> getMaterial() const;
    
    bool isVisible() const;
    void setVisible(bool visible);
    
    bool isCastShadow() const;
    void setCastShadow(bool cast);
    
    bool isReceiveShadow() const;
    void setReceiveShadow(bool receive);
    
    u32 getRenderLayer() const;
    void setRenderLayer(u32 layer);
    
private:
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
    
    bool m_visible = true;
    bool m_castShadow = true;
    bool m_receiveShadow = true;
    u32 m_renderLayer = 0;
};

}
