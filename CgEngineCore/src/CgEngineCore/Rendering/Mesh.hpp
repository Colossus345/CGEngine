#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "CgEngineCore/Log.hpp"
#include "VertexArray.hpp"
#include "Bone.hpp"
#include "Animator.hpp"
#include "CgEngineCore/test/test.hpp"

#include<memory>
#define max_infl 4

namespace CGEngine {




    struct Vertex {

        
        glm::vec3 Position;

        glm::vec3 Normal;

        glm::vec2 TexCoords;

        long  BoneID[max_infl];

        float weights[max_infl];
      
    };
    

    class Texture2D;

    class ShaderProgram;

    class Mesh {
    public:
        
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;
        
        std::unique_ptr<VertexArray> VAO;
        std::unique_ptr<VertexBuffer> VBO;
        std::unique_ptr<IndexBuffer> EBO;

        //unsigned int VAO;
        //unsigned int  VBO;
        //unsigned int EBO;

        Bone* m_Skeleton;

       

        std::unique_ptr<Animator> anima;
        
        
        
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures, Bone* skeleton);
        Mesh& operator=(Mesh&& Mesh) noexcept;
        Mesh(Mesh&& Mesh) noexcept;

        
        void Draw(unsigned int id, ShaderProgram& shader, double& deltaTime);

        void Test(unsigned char* data,int m_width, int m_height,int format);

        void Animate(ShaderProgram& shader, std::string animation, double deltaTime);

       //std::unique_ptr<Bone> getSkeleton() const;

        void add_skeleton(Bone* skeleton);

        void kringe();

        void basa();
          

    private:
        
        BufferLayout vertlay
        { ShaderDataType::Float3,
          ShaderDataType::Float3,
          ShaderDataType::Float2,
          ShaderDataType::Int4,
          ShaderDataType::Float4
        };
        
        
        void setupMesh();

    };
}