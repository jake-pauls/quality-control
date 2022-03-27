// quality-control
// Model.cpp
// 2022-03-26

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.hpp"
#include "Assert.hpp"

Model::Model()
{ }

Model::Model(const std::string& modelFilePath)
{
    LoadModel(modelFilePath);
}

void Model::LoadModel(const std::string& modelFilePath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG("AssimpError #" << importer.GetErrorString());
        return;
    }
    
    _directory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));
    
    LOG("AssimpSuccess::" << _directory);
}
