#pragma once

#include<string>
#include<memory>
#include<map>
#include<vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


//struct aiNode;
//struct aiScene;
//struct aiMesh;
//struct aiMaterial;



namespace CGEngine {
	
	class Model;
	class Texture2D;
	class Mesh;
	class ShaderProgram;
	struct ModelNode;
	struct Texture;


	class ResourseManager {

	public:
		ResourseManager() = delete;
		~ResourseManager() = delete;
		ResourseManager(const ResourseManager&) = delete;
		ResourseManager(ResourseManager&&) = delete;
		ResourseManager& operator = (const ResourseManager&) = delete;
		ResourseManager& operator=(ResourseManager&&) = delete;


		static std::shared_ptr<Model> loadOBJ(std::string const& path);
		static std::shared_ptr<Texture2D>  loadTexture(const std::string& filename);
		static std::shared_ptr<Texture2D>  getTexture(const std::string& name);
		static std::shared_ptr<ShaderProgram> loadShader(std::string const& name, std::string const& vertex_path, std::string const& fragment_path);
		static std::shared_ptr<ShaderProgram> getShader(std::string const& name);



	//private:
		static std::string getFileString(const std::string& relativePath);
		static ModelNode processNode(aiNode* node, const aiScene* scene);
		static Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		static std::vector<std::shared_ptr<Texture2D>> initMaterialTextures(const aiScene* scene);
		static std::shared_ptr<Texture2D> modelLoadTexture(const std::string& filename);
		static void getExeDir(std::string& path);


		typedef std::map<const std::string, std::shared_ptr<ShaderProgram>> ShaderMap;
		static ShaderMap m_Shaders;
		typedef std::map<const std::string, std::shared_ptr<Model>> ModelMap;
		static ModelMap m_Models;
		typedef std::map<const std::string, std::shared_ptr<Texture2D>> TextureMap;
		static TextureMap m_Textures;

		static std::string m_path;
		//static std::string directory;
	};

}