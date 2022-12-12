#include "Bone.hpp"
#include "CgEngineCore/Log.hpp"


namespace CGEngine {

	

	Bone::Bone(int id, std::string name, glm::mat4 bindLocalTransform)
	{
		this->id = id;
		this->name = name;
		this->localBindTransform = bindLocalTransform;
	}
	Bone::~Bone()
	{
		//LOG_CRIT("HEHE");
	}
	void Bone::addChild(Bone child)
	{
		children.push_back(child);
	}

	glm::mat4 Bone::getAnimatedTransform() const
	{
		return animatedTransform;
	}

	void Bone::setAnimatedTransform(glm::mat4 newAnimatedTransform)
	{
		animatedTransform = newAnimatedTransform;
	}

	void Bone::calcInverseBindTransform(glm::mat4 parentBindTransform)
	{
		glm::mat4 bindTransform = parentBindTransform * localBindTransform;
		inverseBindTransform = glm::inverse(bindTransform);
		for (auto it : children) {
			it.calcInverseBindTransform(bindTransform);
		}
	}

}