// quality-control
// Model.cpp
// 2022-03-26

#include "Obj-C-Utils-Interface.h"

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
        LOG("[AssimpError #" << importer.GetErrorString() << "] Failed to load model from file");
        return;
    }
    
    ModelDirectory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));
    
    LOG("[Assimp] Succesfully loaded model with directory: " << ModelDirectory);
    
    // Begin processing model from the root node of the model
    ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(Shader* shaderProgram)
{
    for (unsigned int i = 0; i < Meshes.size(); i++)
        Meshes[i].Draw(shaderProgram);
}

/**
 * Recursively processes the nodes attached to a model loaded in an aiScene
 *
 * Although not required, recrusively processing the each node takes advantage of
 * a 'hierarchy' in the indice/vertex data
 */
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Process all meshes attached to the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Meshes.push_back(ProcessMesh(mesh, scene));
    }
    
    // Process each child node
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Mesh::Texture> textures;
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Mesh::Vertex vertex;
        glm::vec3 vector;
        
        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        
        // Normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        
        // TexCoords
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 texVector;
            
            // Vertex can contain up to 8 different texture coordinates
            // Assumption: Models in question will always have 1 vertex for each texture coordinate
            // (1:1) means we always tale the first set [0]
            
            texVector.x = mesh->mTextureCoords[0][i].x;
            texVector.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = texVector;
            
            // Tangents
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            
            // Bitangents
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
        {
            // Empty texture coordinate for the vertex
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        
        vertices.push_back(vertex);
    }
    
    // Step through each of the mesh's faces
    // This retrieves the vertex indices on each face
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    // Process Mesh materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    
    // Assumes a shader convention for samplers
    // ie:
    // diffuse -> texture_diffuseN
    // specular -> texture_specularN
    // normal -> texture_normalN
    
    // Diffuse maps
    std::vector<Mesh::Texture> diffuseMaps = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    
    // Specular maps
    std::vector<Mesh::Texture> specularMaps = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    
    // Normal maps
    std::vector<Mesh::Texture> normalMaps = LoadMaterialTexture(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    
    // Height maps
    std::vector<Mesh::Texture> heightMaps = LoadMaterialTexture(material, aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    
    return Mesh(vertices, indices, textures);
}

/**
 * Checks all material textures and loads all unloaded textures in
 * Returned as a Mesh::Texture struct
 */
std::vector<Mesh::Texture> Model::LoadMaterialTexture(aiMaterial *material, aiTextureType type, std::string typeName)
{
    std::vector<Mesh::Texture> textures;
    
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString texName;
        material->GetTexture(type, i, &texName);
        
        bool skipTexture = false;
        
        for (unsigned int j = 0; j < LoadedTextures.size(); j++)
        {
            if (std::strcmp(LoadedTextures[j].path.data(), texName.C_Str()) == 0)
            {
                textures.push_back(LoadedTextures[j]);
                
                // Texture with the same filepath has been loaded, skip to the next texture
                skipTexture = true;
                break;
            }
        }
        
        // Load the texture if it wasn't found
        if (!skipTexture)
        {
            Mesh::Texture texture;
            
            texture.id = RetrieveObjectiveCTextureData(texName.C_Str());
            texture.type = typeName;
            texture.path = texName.C_Str();
            textures.push_back(texture);
            LoadedTextures.push_back(texture);
        }
    }
    
    return textures;
}
