#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include <json/json.h>

#include <iostream>
#include <string>

#include <stb_image.h>

#include "test.h"

void test() {

	std::cout << "Hello World!!\n\n";
	Json::Value root;
	Json::Reader reader;
	const std::string value = R"(
		{
			"name" : "some string"
		}
	)";

	bool isSuccess = reader.parse(value, root);
	if (isSuccess) {
		std::cout << root["name"].asString() << std::endl;
	}
	else {
		std::cout << "Failed to parse..";
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load("../assets/container.png", &width, &height, &nrChannels, 0);
	if (data) {
		std::cout << "\n\nSuccessfully loaded texture...";
	}
	else {
		std::cout << "\nFailed to load texture "<< std::endl;
	}
}