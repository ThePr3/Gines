#pragma once

#include <GL/glew.h>
#include <glm\glm.hpp>

struct VertexPositionColorTexture // For Sprites
{
	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 uv;
};

struct VertexPositionColor // For Triangles
{
	glm::vec2 position;
	glm::vec4 color;
};

