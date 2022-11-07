#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

Triangle::Triangle()
{
	v[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	c[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	t[0] = glm::vec2(0.0f, 0.0f);
	t[1] = glm::vec2(0.0f, 0.0f);
	t[2] = glm::vec2(0.0f, 0.0f);
}

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);

	t[0] = glm::vec2(0.0f, 0.0f);
	t[1] = glm::vec2(0.0f, 0.0f);
	t[2] = glm::vec2(0.0f, 0.0f);

};

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec2& t0, glm::vec2& t1, glm::vec2& t2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	t[0] = t0;
	t[1] = t1;
	t[2] = t2;

	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);

};

// Rendering the triangle using OpenGL
void Triangle::RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, bool isTextured, int colorMode, float minZ, float maxZ)
{

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));
	
	// For textured object
	if (isTextured)
	{
		glEnable(GL_TEXTURE_2D);

		// Avoid modulating the texture by vertex color
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glBegin(GL_TRIANGLES);

			glTexCoord2f(t[0].x, t[0].y);
			glVertex3f(v[0].x, v[0].y, v[0].z);

			glTexCoord2f(t[1].x, t[1].y);
			glVertex3f(v[1].x, v[1].y, v[1].z);

			glTexCoord2f(t[2].x, t[2].y);
			glVertex3f(v[2].x, v[2].y, v[2].z);

		glEnd();

		glDisable(GL_TEXTURE_2D);


	}
	// For object with only vertex color
	else
	{
		float normZ = 0;
		glBegin(GL_TRIANGLES);

			if (colorMode == 0 || colorMode == 1) {
				glColor3f(c[0].x, c[0].y, c[0].z);
			}
			else if (colorMode == 2) {
				normZ = (v[0].z - minZ) / (maxZ - minZ);
				glColor3f(0.0f, 0.0f, normZ);
			}
			glVertex3f(v[0].x, v[0].y, v[0].z);

			if (colorMode == 0) {
				glColor3f(c[0].x, c[0].y, c[0].z);
			}
			else if (colorMode == 1) {
				glColor3f(c[1].x, c[1].y, c[1].z);
			}
			else if (colorMode == 2) {
				normZ = (v[1].z - minZ) / (maxZ - minZ);
				glColor3f(0.0f, 0.0f, normZ);
			}
			glVertex3f(v[1].x, v[1].y, v[1].z);

			if (colorMode == 0) {
				glColor3f(c[0].x, c[0].y, c[0].z);
			}
			else if (colorMode == 1) {
				glColor3f(c[2].x, c[2].y, c[2].z);
			}
			else if (colorMode == 2) {
				normZ = (v[2].z - minZ) / (maxZ - minZ);
				glColor3f(0.0f, 0.0f, normZ);
			}
			glVertex3f(v[2].x, v[2].y, v[2].z);
		
		glEnd();
	}

}


// Render the triangle on CPU
void Triangle::RenderCPU(glm::mat4& modelViewMatrix, glm::mat4& projectionMatrix, glm::mat4& viewportMatrix, int width, int height, float color[][1024][3], float depth[][1024], int colorMode, float minZ, float maxZ, bool isTex, int texMode, std::vector<float*> texture, int texWidth, int texHeight)
{
	glm::vec4 v4[3];
	int minX = width;
	int maxX = 0;
	int minY = height;
	int maxY = 0;

	for (int i = 0; i < 3; i++) {

		v4[i] = glm::vec4(v[i], 1.0f);
		v4[i] = viewportMatrix * projectionMatrix * modelViewMatrix * v4[i];
		v4[i] /= v4[i].w;

		if (v4[i].x < minX) {
			minX = ceil(v4[i].x);
		}
		if(v4[i].x > maxX) {
			maxX = ceil(v4[i].x);
		}
		if (v4[i].y < minY) {
			minY = ceil(v4[i].y);
		}
		if (v4[i].y > maxY) {
			maxY = ceil(v4[i].y);
		}
	}

	float alpha, beta, gamma;
	for (int x = minX; x < maxX; x++) {
		for (int y = minY; y < maxY; y++) {

			if (x > width || x < 0 || y > height || y < 0) {
				continue;
			}

			baryCoords(v4, x, y, alpha, beta, gamma);

			bool inside = alpha >= 0 && beta >= 0 && gamma >= 0;

			if (inside) {
				float z = alpha * v4[0].z + beta * v4[1].z + gamma * v4[2].z;
				if (z < depth[y][x]) {
					depth[y][x] = z;
					glm::vec3 col;
					if (isTex) {
						glm::vec2 tCoord;
						if (texMode == 0) {
							interpTex(tCoord, modelViewMatrix, alpha, beta, gamma, texWidth, texHeight);
							tCoord = floor(tCoord);
							wrapTex(tCoord, texWidth, texHeight);

							texColor(col, texture, 0, tCoord, texWidth);
						}
						else if (texMode == 1) {
							interpTex(tCoord, modelViewMatrix, alpha, beta, gamma, texWidth, texHeight);
							tCoord -= 0.5f;

							bilinearInterp(col, tCoord, texture, 0, texWidth, texHeight);
						}
						else if (texMode == 2) {
							glm::vec2 tCoordX;
							glm::vec2 tCoordY;

							float alphaX, betaX, gammaX, alphaY, betaY, gammaY;

							baryCoords(v4, x + 1, y, alphaX, betaX, gammaX);
							baryCoords(v4, x, y + 1, alphaY, betaY, gammaY);

							interpTex(tCoord, modelViewMatrix, alpha, beta, gamma, texWidth, texHeight);
							interpTex(tCoordX, modelViewMatrix, alphaX, betaX, gammaX, texWidth, texHeight);
							interpTex(tCoordY, modelViewMatrix, alphaY, betaY, gammaY, texWidth, texHeight);
							tCoord -= 0.5f;
							tCoordX -= 0.5f;
							tCoordY -= 0.5f;

							glm::vec2 dX = tCoordX - tCoord;
							glm::vec2 dY = tCoordY - tCoord;

							float l = std::max(sqrt(dX.x * dX.x + dX.y * dX.y), sqrt(dY.x * dY.x + dY.y * dY.y));
							l = CLAMP(l, 1, 1024);

							float d = log2f(l);

							int dLow = floor(d);
							int dHigh = ceil(d);

							glm::vec3 colLow, colHigh;
							bilinearInterp(colLow, tCoord, texture, dLow, texWidth, texHeight);
							bilinearInterp(colHigh, tCoord, texture, dHigh, texWidth, texHeight);

							float x = d - dLow;
							col = colLow + x * (colHigh - colLow);
						}
					}
					else {
						if (colorMode == 0) {
							col = alpha * c[0] + beta * c[0] + gamma * c[0];
						}
						else if (colorMode == 1) {
							col = alpha * c[0] + beta * c[1] + gamma * c[2];
						}
						else if (colorMode == 2) {
							glm::vec3 normZ[3];
							normZ[0] = glm::vec3(0.0f, 0.0f, (v[0].z - minZ) / (maxZ - minZ));
							normZ[1] = glm::vec3(0.0f, 0.0f, (v[1].z - minZ) / (maxZ - minZ));
							normZ[2] = glm::vec3(0.0f, 0.0f, (v[2].z - minZ) / (maxZ - minZ));
						
							col = alpha * normZ[0] + beta * normZ[1] + gamma * normZ[2];
						}
					}
					color[y][x][0] = col.x;
					color[y][x][1] = col.y;
					color[y][x][2] = col.z;
				}
			}
		}
	}
}



void Triangle::color(glm::vec3& c0, glm::vec3& c1, glm::vec3& c2) {
	c[0] = c0;
	c[1] = c1;
	c[2] = c2;
}

void Triangle::baryCoords(glm::vec4 v4[3], int x, int y, float& alpha, float& beta, float& gamma) {
	alpha = (-(x - v4[1].x) * (v4[2].y - v4[1].y) + (y - v4[1].y) * (v4[2].x - v4[1].x)) / (-(v4[0].x - v4[1].x) * (v4[2].y - v4[1].y) + (v4[0].y - v4[1].y) * (v4[2].x - v4[1].x));
	beta = (-(x - v4[2].x) * (v4[0].y - v4[2].y) + (y - v4[2].y) * (v4[0].x - v4[2].x)) / (-(v4[1].x - v4[2].x) * (v4[0].y - v4[2].y) + (v4[1].y - v4[2].y) * (v4[0].x - v4[2].x));
	gamma = 1 - alpha - beta;
}

void Triangle::interpTex(glm::vec2& tCoord, glm::mat4& modelViewMatrix, float a, float b, float g, int texWidth, int texHeight) {
	glm::vec2 tSca[3];
	float zInv[3];
	for (int i = 0; i < 3; i++) {
		glm::vec4 temp = modelViewMatrix * glm::vec4(v[i], 1.0f);
		zInv[i] = 1.0f / temp.z;
		tSca[i] = t[i] / temp.z;
	}
	glm::vec2 tInt = a * tSca[0] + b * tSca[1] + g * tSca[2];
	float zInt = a * zInv[0] + b * zInv[1] + g * zInv[2];
	tCoord = tInt / zInt;
	tCoord.x *= texWidth;
	tCoord.y *= texHeight;
}

void Triangle::texColor(glm::vec3& color, std::vector<float*> texture, int index, glm::vec2 tCoord, int texWidth) {
	color.x = texture[index][(int)tCoord.y * texWidth * 3 + (int)tCoord.x * 3 + 0];
	color.y = texture[index][(int)tCoord.y * texWidth * 3 + (int)tCoord.x * 3 + 1];
	color.z = texture[index][(int)tCoord.y * texWidth * 3 + (int)tCoord.x * 3 + 2];
}

void Triangle::wrapTex(glm::vec2& textCoord, int texWidth, int texHeight) {
	if (textCoord.x >= texWidth)
		textCoord.x -= texWidth;
	else if (textCoord.x < 0)
		textCoord.x += texWidth;

	if (textCoord.y >= texHeight)
		textCoord.y -= texHeight;
	else if (textCoord.y < 0)
		textCoord.y += texHeight;
}

void Triangle::bilinearInterp(glm::vec3& color, glm::vec2 tCoord, std::vector<float*> texture, int index, int texWidth, int texHeight) {
	glm::vec2 u[4];
	u[0] = floor(tCoord);
	u[1] = glm::vec2(floor(tCoord.x), ceil(tCoord.y));
	u[2] = glm::vec2(ceil(tCoord.x), floor(tCoord.y));
	u[3] = ceil(tCoord);

	glm::vec3 cols[4];
	for (int i = 0; i < 4; i++) {
		wrapTex(u[i], texWidth, texHeight);
		texColor(cols[i], texture, index, u[i], texWidth);
	}
	wrapTex(tCoord, texWidth, texHeight);

	float t = tCoord.x - u[0].x;
	float s = tCoord.y - u[2].y;
	glm::vec3 c0 = cols[0] + s * (cols[1] - cols[0]);
	glm::vec3 c1 = cols[2] + s * (cols[3] - cols[2]);
	color = c0 + t * (c1 - c0);
}
