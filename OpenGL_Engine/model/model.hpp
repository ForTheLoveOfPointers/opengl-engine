#pragma once
#include <vector>
#include <string>

#include "../model_includes/model_include.hpp"

namespace Model {

	class Model {
	public:
		Model(char* path);
		void draw_model();

	private:
		std::vector<Mesh> meshes;
		void load_model(std::string path);
	};

}