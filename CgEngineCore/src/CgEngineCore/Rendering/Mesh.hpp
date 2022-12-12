#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "CgEngineCore/Log.hpp"
#include "VertexArray.hpp"
#include "Bone.hpp"

#include<memory>


namespace CGEngine {




    struct Vertex {

        
        glm::vec3 Position;

        glm::vec3 Normal;

        glm::vec2 TexCoords;

        int BoneID[4];

        int weights[4];
    };

    class Texture2D;

    class Mesh {
    public:
        
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;
        
        std::unique_ptr<VertexArray> VAO;
        std::unique_ptr<VertexBuffer> VBO;
        std::unique_ptr<IndexBuffer> EBO;

        std::unique_ptr<Bone> m_Skeleton;

        
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures, std::unique_ptr<Bone> skeleton);
        Mesh& operator=(Mesh&& Mesh) noexcept;
        Mesh(Mesh&& Mesh) noexcept;

        
        void Draw(unsigned int id);

        void Test(unsigned char* data,int m_width, int m_height,int format);

       //std::unique_ptr<Bone> getSkeleton() const;

        void add_skeleton(Bone skeleton);


          
          

    private:
        
        BufferLayout vertlay
        { ShaderDataType::Float3,
          ShaderDataType::Float3,
          ShaderDataType::Float2,
          ShaderDataType::Int4,
          ShaderDataType::Int4
        };
        
        
        void setupMesh();

    };
}