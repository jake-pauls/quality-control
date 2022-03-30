// quality-control
// Model.hpp
// 2022-03-26

#ifndef Model_hpp
#define Model_hpp

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

class Model
{
public:
    Model();
    Model(const std::string& modelFilePath);
    
    void Draw(Shader* shader);
    
    std::vector<Mesh::Texture> LoadedTextures;
    std::vector<Mesh> Meshes;
    std::string ModelDirectory;
    
private:
    void LoadModel(const std::string& modelFilePath);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Mesh::Texture> LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
};

#endif /* Model_hpp */
