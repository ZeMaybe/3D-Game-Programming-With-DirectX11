
#include "LightHelper.h"

#include <windows.h>

DirectionalLight::DirectionalLight()
{
	ZeroMemory(this, sizeof(DirectionalLight));
}

PointLight::PointLight()
{
	ZeroMemory(this, sizeof(PointLight));
}

SpotLight::SpotLight()
{
	ZeroMemory(this, sizeof(SpotLight));
}

Material::Material()
{
	ZeroMemory(this, sizeof(Material));
}
