#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "CgEngineCore/Rendering/stb_image.h"



namespace CGEngine {


    class Model
    {
    public:
        // ������ ������ 
        std::vector<Texture> textures_loaded; // (�����������) ��������� ��� ����������� ��������, ����� ���������, ��� ��� �� ��������� ����� ������ ����
        std::vector<Mesh> meshes;
        std::string directory;
        bool gammaCorrection;

        // ����������� � �������� ��������� ���������� ���� � 3D-������
        Model(std::string const& path, bool gamma = false);

        // ������������ ������, � ������ � ��� � ����
        /*void Draw(Shader shader)
        {
            for (unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].Draw(shader);
        }*/

    private:
        // ��������� ������ � ������� Assimp � ��������� ���������� ���� � ������� meshes
        void loadModel(std::string const& path);
        

        // ����������� ��������� ����. ������������ ������ ��������� ���, ������������� � ����, � ��������� ���� ������� ��� ����� �������� ����� (���� ������ ������ �������)
        void processNode(aiNode* node, const aiScene* scene);
        

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        // ��������� ��� �������� ���������� ��������� ���� � �������� ��������, ���� ��� ��� �� ���� ���������.
        // ����������� ���������� ������������ � ���� ��������� Texture
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        
    };

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
    
    
}