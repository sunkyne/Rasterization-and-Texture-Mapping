#pragma once

#include <stdlib.h>
#include <math.h>
#include <vector>

#include <glm/glm.hpp>


class Triangle {
	private:
		glm::vec3 v[3];		// Triangle vertices
		glm::vec3 c[3];		// Vertex color
		glm::vec2 t[3];		// Texture coordinates

	public:

		// Default constructor
		Triangle();

		// Constructor without texture coordinates
		Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2);

		// Constructor with texture coordinates
		Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec2& t0, glm::vec2& t1, glm::vec2& t2);

		// Rendering the triangle using OpenGL
		void RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, bool textureMode, int colorMode, float minZ, float maxZ);

		// Rendering the triangle using CPU
		void RenderCPU(glm::mat4& modelViewMatrix, glm::mat4& projectionMatrix, glm::mat4& viewportMatrix, int width, int height, float color[][1024][3], float depth[][1024], int colorMode, float minZ, float maxZ, bool isTex, int texMode, std::vector<float*> texture, int texWidth, int texHeight);

		// Color triangle
		void color(glm::vec3& c0, glm::vec3& c1, glm::vec3& c2);

		void baryCoords(glm::vec4 v4[3], int x, int y, float& alpha, float& beta, float& gamma);

		void interpTex(glm::vec2& tCoord, glm::mat4& modelViewMatrix, float a, float b, float g, int texWidth, int texHeight);

		void texColor(glm::vec3& color, std::vector<float*> texture, int index, glm::vec2 tCoord, int texWidth);

		void wrapTex(glm::vec2& textCoord, int texWidth, int texHeight);

		void bilinearInterp(glm::vec3& color, glm::vec2 tCoord, std::vector<float*> texture, int index, int texWidth, int texHeight);
};
