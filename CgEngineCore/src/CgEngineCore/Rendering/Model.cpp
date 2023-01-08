#include "Model.hpp"



namespace CGEngine {
    Model::Model(std::vector<ModelNode> nodes)
    {
        this->nodes = std::move( nodes);
    }

    Model::Model(ModelNode node)
    {
        nodes.push_back(std::move(node));
    }

     



    
    
   
       
}