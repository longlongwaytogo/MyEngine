#include <gfx/scene/Mesh.h>

namespace gfx {

Mesh::Mesh() {}

Mesh::~Mesh() {
    releaseFromGPU();
}

void Mesh::setVertices(const std::vector<Vertex>& vertices) {
    m_vertices = vertices;
    calculateBounds();
    m_uploadedToGPU = false;
}

void Mesh::setIndices(const std::vector<u32>& indices) {
    m_indices = indices;
    m_uploadedToGPU = false;
}

void Mesh::setSubMeshes(const std::vector<SubMesh>& subMeshes) {
    m_subMeshes = subMeshes;
}

void Mesh::setName(const std::string& name) {
    m_name = name;
}

const std::string& Mesh::getName() const {
    return m_name;
}

const std::vector<Vertex>& Mesh::getVertices() const {
    return m_vertices;
}

const std::vector<u32>& Mesh::getIndices() const {
    return m_indices;
}

const std::vector<SubMesh>& Mesh::getSubMeshes() const {
    return m_subMeshes;
}

void Mesh::uploadToGPU(Device* device) {
    if (m_uploadedToGPU || !device) {
        return;
    }
    
    if (!m_vertices.empty()) {
        BufferDesc vertexDesc{};
        vertexDesc.size = m_vertices.size() * sizeof(Vertex);
        vertexDesc.usage = BufferUsage::VERTEX;
        vertexDesc.memoryProperty = MemoryProperty::DEVICE_LOCAL;
        vertexDesc.initialData = m_vertices.data();
        m_vertexBuffer = device->createBuffer(vertexDesc);
    }
    
    if (!m_indices.empty()) {
        BufferDesc indexDesc{};
        indexDesc.size = m_indices.size() * sizeof(u32);
        indexDesc.usage = BufferUsage::INDEX;
        indexDesc.memoryProperty = MemoryProperty::DEVICE_LOCAL;
        indexDesc.initialData = m_indices.data();
        m_indexBuffer = device->createBuffer(indexDesc);
    }
    
    m_uploadedToGPU = true;
}

void Mesh::releaseFromGPU() {
    m_vertexBuffer.reset();
    m_indexBuffer.reset();
    m_uploadedToGPU = false;
}

Buffer* Mesh::getVertexBuffer() const {
    return m_vertexBuffer.get();
}

Buffer* Mesh::getIndexBuffer() const {
    return m_indexBuffer.get();
}

const glm::vec3& Mesh::getBoundsMin() const {
    return m_boundsMin;
}

const glm::vec3& Mesh::getBoundsMax() const {
    return m_boundsMax;
}

glm::vec3 Mesh::getBoundsCenter() const {
    return (m_boundsMin + m_boundsMax) * 0.5f;
}

glm::vec3 Mesh::getBoundsExtent() const {
    return m_boundsMax - m_boundsMin;
}

void Mesh::calculateBounds() {
    if (m_vertices.empty()) {
        m_boundsMin = glm::vec3(0.0f);
        m_boundsMax = glm::vec3(0.0f);
        return;
    }
    
    m_boundsMin = m_vertices[0].position;
    m_boundsMax = m_vertices[0].position;
    
    for (const auto& vertex : m_vertices) {
        m_boundsMin = glm::min(m_boundsMin, vertex.position);
        m_boundsMax = glm::max(m_boundsMax, vertex.position);
    }
}

std::shared_ptr<Mesh> Mesh::createTriangle() {
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices = {
        {{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}
    };
    
    std::vector<u32> indices = {0, 1, 2};
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    mesh->setSubMeshes({{0, 3, 0}});
    
    return mesh;
}

std::shared_ptr<Mesh> Mesh::createQuad() {
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices = {
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}
    };
    
    std::vector<u32> indices = {0, 1, 2, 0, 2, 3};
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    mesh->setSubMeshes({{0, 6, 0}});
    
    return mesh;
}

std::shared_ptr<Mesh> Mesh::createCube() {
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}
    };
    
    std::vector<u32> indices = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    mesh->setSubMeshes({{0, 36, 0}});
    
    return mesh;
}

std::shared_ptr<Mesh> Mesh::createSphere(u32 segments, u32 rings) {
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    
    for (u32 ring = 0; ring <= rings; ++ring) {
        float v = (float)ring / (float)rings;
        float phi = v * glm::pi<float>();
        
        for (u32 segment = 0; segment <= segments; ++segment) {
            float u = (float)segment / (float)segments;
            float theta = u * glm::two_pi<float>();
            
            glm::vec3 pos(
                std::sin(phi) * std::cos(theta),
                std::cos(phi),
                std::sin(phi) * std::sin(theta)
            );
            
            vertices.push_back({pos, pos, {u, v}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
        }
    }
    
    for (u32 ring = 0; ring < rings; ++ring) {
        for (u32 segment = 0; segment < segments; ++segment) {
            u32 v0 = ring * (segments + 1) + segment;
            u32 v1 = v0 + 1;
            u32 v2 = v0 + (segments + 1);
            u32 v3 = v2 + 1;
            
            indices.push_back(v0);
            indices.push_back(v2);
            indices.push_back(v1);
            
            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);
        }
    }
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    mesh->setSubMeshes({{0, (u32)indices.size(), 0}});
    
    return mesh;
}

}
