// quality-control
// Model.hpp
// 2022-03-26

#ifndef Model_hpp
#define Model_hpp

#include <string>
#include <vector>

#include "Model.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

class Model
{
public:
    Model();
    Model(const std::string& modelFilePath);
    
    void Draw(Shader& shader);
    
private:
    std::vector<Mesh> _meshes;
    std::string _directory;
    
    void LoadModel(const std::string& modelFilePath);
};

#endif /* Model_hpp */
