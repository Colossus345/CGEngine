#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.hpp"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "CgEngineCore/Rendering/stb_image.h"



namespace CGEngine {

    struct ModelNode {

        ModelNode(std::vector<Mesh>& meshes, std::vector<ModelNode>& childrenNodes) {
            this->meshes = std::move(meshes);
            this->childrenNodes = std::move(childrenNodes);
        }

        std::vector<Mesh> meshes;
        std::vector<ModelNode> childrenNodes;
    };


    class Model
    {
    public:
      

        std::vector<ModelNode> nodes; 
        Model(std::vector<ModelNode> nodes);
        Model(ModelNode node);
        
       
        
    };

    
    
}