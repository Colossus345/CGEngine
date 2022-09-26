#include<iostream>
#include<memory>
#include"CgEngineCore/Application.hpp"

class MyApp : public CGEngine::Application {
	virtual void on_update() override {
		//std::cout << "Update: " << frame++ << std::endl;
	}

	int frame = 0;
 };

int main() {

	auto myApp = std::make_unique<MyApp>();

	int returnCode = myApp->start(1024, 768, "Myapp");

	return 0;
}