#include <gfx/scene/ModelLoader.h>
#include <gfx/core/logging.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

namespace gfx {

class AssimpModelLoader : public ModelLoader {
public:
    AssimpModelLoader();
    ~AssimpModelLoader() override;
    
    bool loadModel(const std::string& filePath, LoadedModel& outModel) override;
    std::shared_ptr<Mesh> loadMesh(const std::string& filePath) override;
    std::vector<std::shared_ptr<Mesh>> loadMeshes(const std::string& filePath) override;
    
private:
    void processNode(aiNode* node, const aiScene* scene, LoadedModel& outModel);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::shared_ptr<Material> processMaterial(aiMaterial* material, const std::string& directory);
};

AssimpModelLoader::AssimpModelLoader() {
    GFX_LOG_INFO("AssimpModelLoader created");
}

AssimpModelLoader::~AssimpModelLoader() {
    GFX_LOG_INFO("AssimpModelLoader destroyed");
}

void AssimpModelLoader::processNode(aiNode* node, const aiScene* scene, LoadedModel& outModel) {
    for (u32 i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        outModel.meshes.push_back(processMesh(mesh, scene));
    }
    
    for (u32 i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, outModel);
    }
}

std::shared_ptr<Mesh> AssimpModelLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    
    for (u32 i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
        
        if (mesh->mNormals) {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }
        
        if (mesh->mTextureCoords[0]) {
            vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
        }
        
        if (mesh->mTangents) {
            vertex.tangent.x = mesh->mTangents[i].x;
            vertex.tangent.y = mesh->mTangents[i].y;
            vertex.tangent.z = mesh->mTangents[i].z;
        }
        
        if (mesh->mBitangents) {
            vertex.bitangent.x = mesh->mBitangents[i].x;
            vertex.bitangent.y = mesh->mBitangents[i].y;
            vertex.bitangent.z = mesh->mBitangents[i].z;
        }
        
        vertices.push_back(vertex);
    }
    
    for (u32 i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (u32 j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    auto resultMesh = std::make_shared<Mesh>();
    resultMesh->setVertices(vertices);
    resultMesh->setIndices(indices);
    resultMesh->setName(mesh->mName.C_Str());
    
    return resultMesh;
}

std::shared_ptr<Material> AssimpModelLoader::processMaterial(aiMaterial* material, const std::string& directory) {
    (void)material;
    (void)directory;
    return nullptr;
}

bool AssimpModelLoader::loadModel(const std::string& filePath, LoadedModel& outModel) {
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(
        filePath,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_OptimizeMeshes
    );
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        GFX_LOG_ERROR("Assimp failed to load model: %s", importer.GetErrorString());
        return false;
    }
    
    outModel.name = scene->mRootNode->mName.C_Str();
    processNode(scene->mRootNode, scene, outModel);
    
    GFX_LOG_INFO("Loaded model: %s with %zu meshes", filePath.c_str(), outModel.meshes.size());
    return true;
}

std::shared_ptr<Mesh> AssimpModelLoader::loadMesh(const std::string& filePath) {
    LoadedModel model;
    if (loadModel(filePath, model)) {
        if (!model.meshes.empty()) {
            return model.meshes[0];
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Mesh>> AssimpModelLoader::loadMeshes(const std::string& filePath) {
    LoadedModel model;
    if (loadModel(filePath, model)) {
        return model.meshes;
    }
    return {};
}

std::unique_ptr<ModelLoader> ModelLoader::create() {
    return std::make_unique<AssimpModelLoader>();
}

}
