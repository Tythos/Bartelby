/**
 * @author <code@tythos.net
 */

#include <fstream>
#include <sstream>
#include <map>
#include <iostream>
#include <gl\glew.h>

#ifndef GRAPHICS_H
#define GRAPHICS_H

 /**
  * Defines the basic vertex structure at a byte level
  *
  * @property {float[3]} position - Triplet of x/y/z coordinates
  * @property {float[3]} color - Triplet of r/g/b color values
 */
struct BasicVertex {
	float position[3];
	float color[3];

	/**
	 * Defines a constructor to streamline basic vertex array data instantiation
	 *
	 * @param {float} x - First axis coordinate of vertex
	 * @param {float} y - Second axis coordinate of vertex
	 * @param {float} z - Third axis coordinate of vertex
	 * @param {float} r - Red value [0=black, 1=red] at vertex
	 * @param {float} g - Green value [0=black, 1=green] at vertex
	 * @param {float} b - Blue value [0=black, 1=blue] at vertex
	 */
	BasicVertex(float x, float y, float z, float r, float g, float b) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}
};

/**
 * Primary mid-level graphics manager. Responsible for organizing,
 * initializing, and cleaning up GL variables and bindings, including maps for
 * programs, attributes, and buffers. Uses *BasicVertex* model to compute GL
 * binding sizes for attribute arrays, etc.
 * 
 * @property {int} numVertices - Number of vertices in the attribute buffer; defaults to 3
 * @property {int} numAttributes - Number of attributes per vertex, as dynamically computed from the BasicVertex format (assuming all floats)
 * @property {std::map<std::string, GLuint>} programs - Maps program names to specific GL program identifiers
 * @property {std::map<std::string, GLint>} attribs - Maps attribute names to specific GL vertex attribute bindings
 * @property {std::map<std::string, GLuint>} buffers - Maps buffer names ("vertex", "index", etc.) to specific GL buffer identifiers
 */
class Graphics {
private:
protected:
	int numVertices = 3;
	int numAttributes = sizeof(BasicVertex) / sizeof(float);
	std::map<std::string, GLuint> programs;
	std::map<std::string, GLint> attribs;
	std::map<std::string, GLuint> buffers;

public:
	Graphics();
	~Graphics();
	void printProgramLog(GLuint program);
	void printShaderLog(GLuint shader);
	void render();
};

#endif
