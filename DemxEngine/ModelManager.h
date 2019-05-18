#pragma once

#include <unordered_map>

namespace Demx
{
	class Material;

	class ModelManager
	{
		std::unordered_map<std::string, std::vector<Demx::Material>*> materials;

	public:
		ModelManager(void);
		~ModelManager(void);

		void addMaterial(std::string name, Demx::Material material);
		std::vector<Demx::Material>* getModelMaterials(const std::string& name);
		GLvoid cleanup();
	};
}
