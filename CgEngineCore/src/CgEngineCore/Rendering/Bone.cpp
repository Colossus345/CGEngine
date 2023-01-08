#include "Bone.hpp"
#include "CgEngineCore/Log.hpp"


namespace CGEngine {

	

	void Bone::setupAnim( aiAnimation** animation, const unsigned int numAnim)
	{
		int ind = 0;
		
		for(int j = 0; j<numAnim;j++)
		for (int i = 0; i < animation[j]->mNumChannels; i++) {
			if (animation[j]->mChannels[i]->mNodeName.C_Str() == name) {
				ind = i;
				this->animation[animation[j]->mName.C_Str()] = new Animation(animation[j]->mChannels[i],animation[j]->mDuration);
				break;
			}
		}
	}

	Bone::Bone(int id, std::string name, glm::mat4 bindLocalTransform,glm::mat4 offset, aiAnimation** animation, const unsigned int numAnim)
	{
		this->id = id;
		this->name = name;
		this->localBindTransform = bindLocalTransform;
		
		animatedTransform = glm::mat4(1.f);
		this->offset = offset;
		setupAnim(animation,numAnim);
	}
	Bone::~Bone()
	{
		//LOG_WARN("HEHE");
	}
	void Bone::addChild(Bone child)
	{
		children.push_back(child);
	}

	void Bone::applyRoot(glm::mat4 offset)
	{
		localBindTransform *= offset;
	}

	glm::mat4 Bone::getAnimatedTransform() const
	{
		return animatedTransform;
	}

	void Bone::updateAnimation(double currentTime, std::string anima ,glm::mat4 parentTransform, std::vector<glm::mat4>& mFinalBoneMatrix)
	{
		if (animation.count(anima)) {
			glm::mat4 translation = animation[anima]->InterpolatePosition(currentTime);
			glm::mat4 rotation = animation[anima]->InterpolateRotation(currentTime);
			glm::mat4 scale = animation[anima]->InterpolateScaling(currentTime);
			animatedTransform = translation * rotation * scale;
		}
		

		mFinalBoneMatrix[id] = parentTransform * animatedTransform * localBindTransform * offset;
		for (auto& it : children) {
			it.updateAnimation(currentTime,anima, parentTransform * animatedTransform * localBindTransform,mFinalBoneMatrix);
		}

		//return mFinalBoneMatrix;
	}

	void Bone::setAnimatedTransform(glm::mat4 newAnimatedTransform)
	{
		animatedTransform = newAnimatedTransform;
	}

	void Bone::calcInverseBindTransform(glm::mat4 parentBindTransform)
	{
		glm::mat4 bindTransform = parentBindTransform * localBindTransform;
		inverseBindTransform = glm::inverse(bindTransform);
		for (auto& it : children) {
			it.calcInverseBindTransform(bindTransform);
		}
	}

}