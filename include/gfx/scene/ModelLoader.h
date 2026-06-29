#pragma once

#include <gfx/scene/Mesh.h>
#include <gfx/scene/MeshComponent.h>
#include <string>
#include <memory>
#include <vector>

namespace gfx {

class ModelLoader {
public:
    struct LoadedModel {
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Material>> materials;
        std::string name;
    };
    
    static std::unique_ptr<ModelLoader> create();
    
    virtual ~ModelLoader() = default;
    
    virtual bool loadModel(const std::string& filePath, LoadedModel& outModel) = 0;
    
    virtual std::shared_ptr<Mesh> loadMesh(const std::string& filePath) = 0;
    
    virtual std::vector<std::shared_ptr<Mesh>> loadMeshes(const std::string& filePath) = 0;
};

}
