#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "CgEngineCore/Log.hpp"
#include "VertexArray.hpp"

#include<memory>


namespace CGEngine {




    struct Vertex {

        // Позиция
        glm::vec3 Position;

        // Нормаль
        glm::vec3 Normal;

        // Текстурные координаты
        glm::vec2 TexCoords;

        // Касательный вектор
        glm::vec3 Tangent;

        // Вектор бинормали (вектор, перпендикулярный касательному вектору и вектору нормали)
        glm::vec3 Bitangent;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        // Данные меша
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        //VertexArray VAO;
       // unsigned int VAO, VBO, EBO;

        std::unique_ptr<VertexArray> VAO;
        std::unique_ptr<VertexBuffer> VBO;
        std::unique_ptr<IndexBuffer> EBO;
        // Конструктор
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        

          // Рендеринг меша
        void Draw(unsigned int id);
          
          

    private:
        // Данные для рендеринга 
        BufferLayout vertlay
        { ShaderDataType::Float3,
          ShaderDataType::Float3,
          ShaderDataType::Float2,
          ShaderDataType::Float3,
          ShaderDataType::Float3 
        };
        BufferLayout texlay{ ShaderDataType::Float3 };

        // Инициализируем все буферные объекты/массивы
        void setupMesh();

    };
}