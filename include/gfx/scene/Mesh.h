#pragma once

#include <gfx/core/types.h>
#include <gfx/hal/Buffer.h>
#include <gfx/hal/Device.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

namespace gfx {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct SubMesh {
    u32 indexOffset = 0;
    u32 indexCount = 0;
    u32 materialIndex = 0;
};

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<u32>& indices);
    void setSubMeshes(const std::vector<SubMesh>& subMeshes);
    
    const std::vector<Vertex>& getVertices() const;
    const std::vector<u32>& getIndices() const;
    const std::vector<SubMesh>& getSubMeshes() const;
    
    void uploadToGPU(Device* device);
    void releaseFromGPU();
    
    Buffer* getVertexBuffer() const;
    Buffer* getIndexBuffer() const;
    
    const glm::vec3& getBoundsMin() const;
    const glm::vec3& getBoundsMax() const;
    glm::vec3 getBoundsCenter() const;
    glm::vec3 getBoundsExtent() const;
    
    static std::shared_ptr<Mesh> createTriangle();
    static std::shared_ptr<Mesh> createQuad();
    static std::shared_ptr<Mesh> createCube();
    static std::shared_ptr<Mesh> createSphere(u32 segments = 32, u32 rings = 16);
    
public:
    void setName(const std::string& name);
    const std::string& getName() const;
    
private:
    void calculateBounds();
    
    std::vector<Vertex> m_vertices;
    std::vector<u32> m_indices;
    std::vector<SubMesh> m_subMeshes;
    std::string m_name;
    
    std::unique_ptr<Buffer> m_vertexBuffer;
    std::unique_ptr<Buffer> m_indexBuffer;
    
    glm::vec3 m_boundsMin{0.0f};
    glm::vec3 m_boundsMax{0.0f};
    
    bool m_uploadedToGPU = false;
};

}
