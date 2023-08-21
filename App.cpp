/**
 * @author <code@tythos.net>
 */

#include "App.h"

/**
 * Construct a new App object, including initialization of SDL, GL, and GLEW
 * subsystems.
 */
App::App() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::runtime_error("SDL initialization failed");
	}
	this->window = SDL_CreateWindow("Artemis - Alphonse", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (this->window == NULL) {
		throw std::runtime_error("Window initialization failed");
	}
	this->context = SDL_GL_CreateContext(this->window);
	if (this->context == NULL) {
		throw std::runtime_error("Context initialization failed");
	}
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		throw std::runtime_error("GLEW initialization failed");
	}
	SDL_StartTextInput();
	this->graphics = new Graphics();
}

/**
 * Destroy the App object. This frees graphics resources and window handles, as
 * well as other SDL-specific cleanup.
 */
App::~App() {
	delete this->graphics;
	SDL_StopTextInput();
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

/**
 * Clears color buffer and (if input is true) invokes the graphic system's
 * "render()" routine. Once finished, swaps graphics buffers for this window's
 * context.
 * 
 * @param {bool} isMeshVisible - If true, forwards a render() call to the graphics system
 */
void App::render(bool isMeshVisible) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*)&camera);
	glMatrixMode(GL_MODELVIEW);
	if (isMeshVisible) {
		this->graphics->render();
	}
	SDL_GL_SwapWindow(this->window);
}

void App::setCamPos(float x, float y, float z) {
	this->camera[0][3] = x;
	this->camera[1][3] = y;
	this->camera[2][3] = z;
}

void App::setCamLook(float x, float y, float z) {
	// 3x3 of camera is RUD frame (row vectors)
	
	// compute look and mag
	float dx = x - this->camera[0][3];
	float dy = y - this->camera[1][3];
	float dz = z - this->camera[2][3];
	float D = sqrt(dx * dx + dy * dy + dz * dz);
	dx /= D;
	dy /= D;
	dz /= D;

	// assuming up is normalized already
	float ux = this->camera[1][0];
	float uy = this->camera[1][1];
	float uz = this->camera[1][2];

	// compute cross (right)
	float rx = uy * dz - dy * uz;
	float ry = uz * dx - dz * ux;
	float rz = ux * dy - dx * uy;

	// set RIGHT unit vector
	this->camera[0][0] = rx;
	this->camera[0][1] = ry;
	this->camera[0][2] = rz;

	// set DIRECTION unit vector
	this->camera[2][0] = dx;
	this->camera[2][1] = dy;
	this->camera[2][2] = dz;
}
