#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "CgEngineCore/Log.hpp"
#include "VertexArray.hpp"

#include<memory>


namespace CGEngine {




    struct Vertex {

        // �������
        glm::vec3 Position;

        // �������
        glm::vec3 Normal;

        // ���������� ����������
        glm::vec2 TexCoords;

        // ����������� ������
        glm::vec3 Tangent;

        // ������ ��������� (������, ���������������� ������������ ������� � ������� �������)
        glm::vec3 Bitangent;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        // ������ ����
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        //VertexArray VAO;
       // unsigned int VAO, VBO, EBO;

        std::unique_ptr<VertexArray> VAO;
        std::unique_ptr<VertexBuffer> VBO;
        std::unique_ptr<IndexBuffer> EBO;
        // �����������
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        

          // ��������� ����
        void Draw(unsigned int id);
          
          

    private:
        // ������ ��� ���������� 
        BufferLayout vertlay
        { ShaderDataType::Float3,
          ShaderDataType::Float3,
          ShaderDataType::Float2,
          ShaderDataType::Float3,
          ShaderDataType::Float3 
        };
        BufferLayout texlay{ ShaderDataType::Float3 };

        // �������������� ��� �������� �������/�������
        void setupMesh();

    };
}