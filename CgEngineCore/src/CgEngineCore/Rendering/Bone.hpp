#pragma once
#include "glm/glm.hpp"
#include <string>
#include <list>
namespace CGEngine {

	class Bone {
	public:
		int id;
		std::string name;
		std::list<Bone> children;

		glm::mat4 animatedTransform;

		glm::mat4 localBindTransform;
		glm::mat4 inverseBindTransform;

		
		Bone(int id, std::string name, glm::mat4 bindLocalTransform);
		~Bone();
		void addChild(Bone child);

		glm::mat4 getAnimatedTransform() const;
	
		void setAnimatedTransform(glm::mat4 newAnimatedTransform);

		void calcInverseBindTransform(glm::mat4 parentBindTransform);


	};

}