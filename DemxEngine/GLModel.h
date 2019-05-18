#pragma once

#include "Mesh.h"

namespace Demx
{
	class GLModel
	{
	public:
		GLModel::GLModel(std::string name, std::string path);
		GLModel::~GLModel(void);

		GLuint VAO;
		GLuint* VBO;
		GLuint IB;

		std::string name;
		std::string path;

		// model info
		std::vector<Mesh> meshes;
	};
}
