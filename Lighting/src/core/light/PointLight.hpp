#include "../Object.h"

class PointLight : public Object
{
public:
	PointLight(std::string&& name, Mesh* mesh, Material* material, Shader* shader)
		: Object(std::move(name), mesh, material, shader)
	{
	}

private:

};