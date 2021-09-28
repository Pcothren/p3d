#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> 
#include <glm/gtx/euler_angles.hpp>

static constexpr float PI = 3.14159265f;
static constexpr double PI_D = 3.1415926535897932;

struct mvTransforms
{
	glm::mat4 model = glm::identity<glm::mat4>();
	glm::mat4 modelView = glm::identity<glm::mat4>();
	glm::mat4 modelViewProjection = glm::identity<glm::mat4>();
};