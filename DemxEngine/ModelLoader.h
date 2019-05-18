#pragma once

#include "Mesh.h"

namespace Demx
{
	class ModelLoader
	{
	public:
		std::vector<Mesh> meshes;
		std::vector<GLuint> indices;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> mappings;
		std::vector<glm::vec3> normals;

		GLboolean hasTransparency;

	public:
		ModelLoader(void);
		~ModelLoader(void);

		GL_HRESULT processModel( const std::string& path, std::string name, ModelManager* manager, std::unordered_map<std::string, Demx::Texture>& texturesMap );
	};
}
