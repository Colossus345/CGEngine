
#include<vector>
#include<string>
#include<map>


#include"Animation.hpp"
#include "Bone.hpp"

#define MAX_BONE_INFLUENCE 100

namespace CGEngine {


	class Animator {
	public:



	Animator();
	Animator(Bone* skeleton);
	void setAnimation(std::string animation);

	void update(double deltaTime);

	void setSkeleton(Bone* skeleton);
	
	void play();
	void play(std::string animation);
	void stop();

	std::vector<glm::mat4> getTransforms() const {
		return mFinalBoneMatrix;
	}

	private:
		bool isPlaying;
		Bone* mRoot;
		std::string mAnimName;
		double mCurrentTime;
		std::vector<glm::mat4> mFinalBoneMatrix;
		

	};

}