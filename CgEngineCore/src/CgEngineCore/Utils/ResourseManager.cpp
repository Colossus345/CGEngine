#include "ResourseManager.hpp"
#include "CgEngineCore/Rendering/Model.hpp"
#include "CgEngineCore/Rendering/Texture2D.hpp"
#include "CgEngineCore/Rendering/ShaderProgram.hpp"
#include "glm/gtc/quaternion.hpp"
#include "CgEngineCore/Rendering/Bone.hpp"

#include<iostream>
#include<fstream>


namespace CGEngine {

    ResourseManager::TextureMap ResourseManager::m_Textures;
    ResourseManager::ModelMap   ResourseManager::m_Models;
    ResourseManager::ShaderMap  ResourseManager::m_Shaders;
    std::string                 ResourseManager::m_path;
    std::string directory;

    int b_id;
    void parse_node(const aiNode* pNode) {

        std::cout << "Node name " << pNode->mName.C_Str() << " num meshes " << pNode->mNumMeshes << " num child " << pNode->mNumChildren << std::endl;
        for (int i = 0; i < pNode->mNumChildren; i++) {
            std::cout << std::endl;
            parse_node(pNode->mChildren[i]);
        }
    }

    std::shared_ptr<Model> ResourseManager::loadOBJ(std::string const& path)
    {

        std::string filename = m_path + '/' + path;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);


        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR("ERROR::ASSIMP::{0}", std::string(importer.GetErrorString()));
            return nullptr;
        }

        directory = filename.substr(0, filename.find_last_of('/'));

        parse_node(scene->mRootNode);

        std::shared_ptr<Model>& p_model = m_Models.emplace(filename, std::make_unique<Model>(processNode(scene->mRootNode, scene))).first->second;

        return p_model;
    }

    std::shared_ptr<Texture2D> ResourseManager::loadTexture(const std::string& filename)
    {
        std::string name = m_path + '/' + filename;

        if (m_Textures.count(name)) {
            return getTexture(name);
        }
        int width, height, nrComponents;
        unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrComponents, 0);
        if (!data)
        {
            LOG_WARN("Texture failed to load at path: \"{0}\" ", name);



            if (m_Textures.count(m_path + '/' + "res/miss.png")) {
                return getTexture(m_path + '/' + "res/miss.png");
            }
            else {
                return loadTexture("res/miss.png");
            }

        }
        std::shared_ptr<Texture2D> p_tex = m_Textures.emplace(name,
            std::make_shared<Texture2D>(data, width, height, nrComponents)).first->second;

        stbi_image_free(data);


        return p_tex;
    }
    std::shared_ptr<Texture2D> ResourseManager::modelLoadTexture(const std::string& filename)
    {
        std::string name = filename;

        if (m_Textures.count(name)) {
            return getTexture(name);
        }
        int width, height, nrComponents;
        unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrComponents, 0);
        if (!data)
        {
            LOG_WARN("Texture failed to load at path: \"{0}\" ", name);

            if (filename == "res/miss.png")
                return nullptr;
            return loadTexture("res/miss.png");

        }
        std::shared_ptr<Texture2D>& p_tex = m_Textures.emplace(name,
            std::make_shared<Texture2D>(data, width, height, nrComponents)).first->second;

        stbi_image_free(data);


        return p_tex;
    }

    std::shared_ptr<Texture2D> ResourseManager::getTexture(const std::string& name)
    {
        TextureMap::const_iterator it = m_Textures.find(name);
        if (it != m_Textures.end())
        {
            return it->second;
        }
        LOG_WARN("Cant find texture with path: {0}", name);
        return nullptr;
    }

    std::shared_ptr<ShaderProgram> ResourseManager::loadShader(std::string const& shaderName, std::string const& vertex_path, std::string const& fragment_path)
    {
        std::string vertexString = getFileString(vertex_path);
        if (vertexString.empty()) {
            LOG_ERROR("No vertex shader at path {0}", vertex_path);
            return nullptr;
        }

        std::string fragmentString = getFileString(fragment_path);
        if (fragmentString.empty()) {
            LOG_ERROR("No fragment shader at path {0}", fragment_path);
            return nullptr;
        }

        std::shared_ptr<ShaderProgram>& newShader = m_Shaders.emplace(shaderName, std::make_shared<ShaderProgram>(vertexString.c_str(), fragmentString.c_str())).first->second;
        if (newShader->isCompiled()) {

            return newShader;

        }
        LOG_ERROR("Cant load shader program\nVertex:{0}\nFragment:{1}", vertex_path, fragment_path);
        return nullptr;
    }

    std::shared_ptr<ShaderProgram> ResourseManager::getShader(std::string const& name)
    {
        return std::shared_ptr<ShaderProgram>();
    }

    std::string ResourseManager::getFileString(const std::string& relativePath)
    {
        std::fstream f;
        f.open(m_path + '/' + relativePath.c_str(), std::ios::in | std::ios::binary);

        if (!f.is_open()) {
            LOG_ERROR("Cannot open file {0}", m_path + '/' + relativePath.c_str());
            return std::string();
        }

        std::stringstream buffer;

        buffer << f.rdbuf();

        return buffer.str();
    }

    ModelNode ResourseManager::processNode(aiNode* node, const aiScene* scene)
    {
        std::vector<Mesh> meshes;
        std::vector<ModelNode> nodes;

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {

            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            Mesh a = processMesh(mesh, scene);
            meshes.push_back(std::move(a));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            //if (node->mChildren[i]->mNumMeshes > 0)
                nodes.push_back(processNode(node->mChildren[i], scene));
        }
        return ModelNode(meshes, nodes);
    }

    Mesh ResourseManager::processMesh(aiMesh* mesh, const aiScene* scene)
    {


        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;


        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;


            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;


            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;

                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];


            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }


        std::unique_ptr<Bone> skeleton =std::make_unique<Bone>(*ExtractBoneWeightForVertices(vertices, mesh, scene));
        auto texmas = initMaterialTextures(scene);
        textures.insert(textures.end(), texmas.begin(), texmas.end());

        if (textures.size() == 0) {
            std::string filename = "res/miss.png";

            textures.push_back(loadTexture(filename.c_str()));


        }

      

        
        

        return Mesh(vertices, indices, textures, std::move(skeleton));

    }

    std::vector<std::shared_ptr<Texture2D>> ResourseManager::initMaterialTextures(const aiScene* scene)
    {
        std::vector<std::shared_ptr<Texture2D>> textures;
        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            const aiMaterial* pMaterial = scene->mMaterials[i];


            if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiString Path;

                std::string filename = directory;

                aiReturn res = pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path);
                if (res == AI_SUCCESS) {
                    std::string p(Path.data);
                    if (p.substr(0, 2) == ".\\") {
                        p = p.substr(2, p.size() - 2);
                    }
                    filename += '/';
                    filename += p;

                }
                else {
                    continue;
                }

                if (m_Textures.count(filename)) {
                    textures.push_back(getTexture(filename));
                    continue;
                }
                textures.push_back(modelLoadTexture(filename));
            }



        }

        return textures;
    }

    void ResourseManager::getExeDir(std::string& path)
    {
        size_t found = path.find_last_of("/\\");
        m_path = path.substr(0, found);


    }


    inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
    {
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;

    }
    inline glm::vec3 GetGLMVec(const aiVector3D& vec)
    {
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
    {
        return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
    }

    int find_node(std::vector<aiBone*>& bonemass, const aiNode* node) {

        for (int i = 0; i < bonemass.size(); i++) {
            if (strcmp(bonemass[i]->mName.C_Str(), node->mName.C_Str())==0) {
                return i;
            }
        }
        return -1;
    }
    Bone* make_tree(std::vector<aiBone*>& bonemass, const aiNode* root, std::map<std::string, int> &bonemap)
    {
        Bone* rot;
        int index = - 1;
        bool flag = false;
       
        for (int i = 0; i < root->mNumChildren; i++) {
            index = find_node(bonemass, root->mChildren[i]);
            if (index != -1) {
                flag = true;
                bonemap[bonemass[index]->mName.C_Str()] = b_id;
                rot = (new Bone(b_id++, bonemass[index]->mName.C_Str(), ConvertMatrixToGLMFormat(bonemass[index]->mOffsetMatrix)));
            }
            else {
                continue;
            }
            Bone* temp = make_tree(bonemass, root->mChildren[i],bonemap);
            if (temp != nullptr)
                rot->addChild(*temp);
        }
        if (index == -1 && !flag) {
            return nullptr;
        }
        

        return rot;
    }


    

    Bone* ResourseManager::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
    {
        std::vector<aiBone*> bonemass;
        for (int i = 0; i < mesh->mNumBones; i++) {
            aiBone* bone = mesh->mBones[i];
            bonemass.push_back(bone);
            
        }
        std::map<std::string, int> bonemap;
       
        Bone* skeleton = make_tree(bonemass, scene->mRootNode,bonemap);
        
        skeleton->calcInverseBindTransform(glm::mat4(1.f));

        for (int i = 0; i < bonemass.size(); i++) {
            for (int j = 0; j < bonemass[i]->mNumWeights; j++) {
                SetVertexBoneData(vertices[bonemass[i]->mWeights[j].mVertexId],bonemap[bonemass[i]->mName.C_Str()], bonemass[i]->mWeights[j].mWeight);
           }
        }
        return skeleton;
    }

    void ResourseManager::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
    {
        for (int i = 0; i < 4; ++i)
        {
           if (vertex.BoneID[i] < 0)
            {
                vertex.weights[i] = weight;
                vertex.BoneID[i] = boneID;
                break;
            } 
        }
    }
    

    


}