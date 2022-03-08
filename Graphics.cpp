/**
 * @author <code@tythos.net>
 */

#include "Graphics.h"

/**
 * Helper method for reading a std::string from file contents
 *
 * @param {std::string} filePath - Indicates the path of the resource to read
 * @returns {std::string} - Contents of file indicated by input
 */
std::string readFile(std::string filePath) {
	std::ifstream f;
	std::ostringstream sstr;
	f.open(filePath);
	sstr << f.rdbuf();
	return sstr.str();
}

/**
 * Construct a new Graphics object. This includes initialization (compile,
 * link) actions for the shader program (defined from external source
 * files). It also defines the initial buffer contents (vertex and index data).
 * Finally, it declares bindings between shader variables and vertex
 * attributes.
 */
Graphics::Graphics() {
	// define graphics program
	this->programs["basic"] = glCreateProgram();

	// load and compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexString = readFile("../basic.v.glsl");
	const char* vertexRaw = vertexString.c_str();
	glShaderSource(vertexShader, 1, &vertexRaw, NULL);
	glCompileShader(vertexShader);
	GLint vsCompileStatus = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vsCompileStatus);
	if (vsCompileStatus != GL_TRUE) {
		printShaderLog(vertexShader);
		throw std::runtime_error("Vertex shader failed to compile");
	}
	glAttachShader(this->programs["basic"], vertexShader);

	// load and compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentString = readFile("../basic.f.glsl");
	const char* fragmentRaw = fragmentString.c_str();
	glShaderSource(fragmentShader, 1, &fragmentRaw, NULL);
	glCompileShader(fragmentShader);
	GLint fsCompileStatus = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fsCompileStatus);
	if (fsCompileStatus != GL_TRUE) {
		this->printShaderLog(fragmentShader);
		throw std::runtime_error("Fragment shader failed to compile");
	}
	glAttachShader(this->programs["basic"], fragmentShader);

	// link shader program and bind program variables
	glLinkProgram(this->programs["basic"]);
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(this->programs["basic"], GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE) {
		this->printProgramLog(this->programs["basic"]);
		throw std::runtime_error("Shader program failed to link");
	}
	this->attribs["position"] = glGetAttribLocation(this->programs["basic"], "position");
	this->attribs["color"] = glGetAttribLocation(this->programs["basic"], "color");
	if (this->attribs["position"] < 0 || this->attribs["color"] < 0) {
		throw std::runtime_error("Shader program failed to expose attributes");
	}

	// populate buffer objects
	this->buffers["vertex"] = 0;
	this->buffers["index"] = 0;
	glClearColor(0.f, 0.f, 0.f, 1.f);
	BasicVertex vertexData[] = {
		BasicVertex(-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f),
		BasicVertex(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
		BasicVertex(0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f)
	};
	GLuint indexData[] = { 0, 1, 2 };
	glGenBuffers(1, &this->buffers["vertex"]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers["vertex"]);
	glBufferData(GL_ARRAY_BUFFER, this->numAttributes * this->numVertices * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &this->buffers["index"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers["index"]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numVertices * sizeof(GLuint), indexData, GL_STATIC_DRAW);

	// now we can bind specific attributes from the vertex array/format and program variables
	glVertexAttribPointer(this->attribs["position"], 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), NULL);
	glVertexAttribPointer(this->attribs["color"], 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (void*)sizeof(float[3]));
}

/**
 * Destroys the Graphics object. This frees the GL program declared in the
 * constructor, but does not free the window or GL context (which are managed
 * by the top-level App object).
 */
Graphics::~Graphics() {
	glDeleteProgram(this->programs["basic"]);
}

/**
 * Helper method for printing the info log of a specific GL program to STDOUT.
 * 
 * @param {GLuint} program - Identifier for a unique GL program
 */
void Graphics::printProgramLog(GLuint program) {
	if (glIsProgram(program)) {
		int bufferLength = 0;
		int infoLogLength = bufferLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, &bufferLength, infoLog);
		if (bufferLength > 0) {
			std::cout << infoLog << std::endl;
		}
		delete[] infoLog;
	}
	else {
		throw std::runtime_error("Invalid GL program ID");
	}
}

/**
 * Helper method for printing the info log of a specific GL shader to STDOUT.
 * 
 * @param {GLuint} shader - Identifier for a unique GL shader
 */
void Graphics::printShaderLog(GLuint shader) {
	if (glIsShader(shader)) {
		int bufferLength = 0;
		int infoLogLength = bufferLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, &bufferLength, infoLog);
		if (bufferLength > 0) {
			std::cout << infoLog << std::endl;
		}
		delete[] infoLog;
	} 
	else {
		throw std::runtime_error("Invalid GL shader ID");
	}
}

/**
 * Primary rendering routine for low-level GL calls. (Assumes parent caller
 * will swap graphics buffers for the active windowed context). Loads program,
 * enables vertex attributes, binds array buffers, and makes draw call. Once
 * draw is finished, attributes are disabled and program is unloaded.
 */
void Graphics::render() {
	glUseProgram(this->programs["basic"]);
	glEnableVertexAttribArray(this->attribs["position"]);
	glEnableVertexAttribArray(this->attribs["color"]);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers["vertex"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers["index"]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(this->attribs["position"]);
	glDisableVertexAttribArray(this->attribs["color"]);
	glUseProgram(NULL);
}
