#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN // For Winsock2

#include <windows.h> // For Windows

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") 

#include <glew.h>
#include <glfw3.h>
#include <GL\GL.h>
#include <GL\GLU.h> // OpenGL

#include "Model.h"
#include "MatrixStack.h" 
#include "shader.hpp"
#include "Light.h"

#include "InputHandler.h"

#include "MeshBuilder.h"
#include "LoadTGA.h"

#include "Gui.h"

class Branch;

#define LIGHTS_ENABLED true

enum VIEW_STATE
{
	VIEW_ORTHO = 0,
	VIEW_PERSPECTIVE,
};

enum VIEWPORT_INFO
{
	VIEWPORT_WIDTH = 2,
	VIEWPORT_HEIGHT,
};

enum UNIFORM_TYPE
{
	U_MVP = 0,
	U_MODELVIEW,
	U_MODELVIEW_INVERSE_TRANSPOSE,
	U_MATERIAL_AMBIENT,
	U_MATERIAL_DIFFUSE,
	U_MATERIAL_SPECULAR,
	U_MATERIAL_SHININESS,
	U_LIGHTENABLED,
	U_NUMLIGHTS,
	U_LIGHT0_TYPE,
	U_LIGHT0_POSITION,
	U_LIGHT0_COLOR,
	U_LIGHT0_POWER,
	U_LIGHT0_KC,
	U_LIGHT0_KL,
	U_LIGHT0_KQ,
	U_LIGHT0_SPOTDIRECTION,
	U_LIGHT0_COSCUTOFF,
	U_LIGHT0_COSINNER,
	U_LIGHT0_EXPONENT,

	U_COLOR_TEXTURE_ENABLED,
	U_COLOR_TEXTURE_ENABLED1,
	U_COLOR_TEXTURE,
	U_COLOR_TEXTURE1,

	U_TEXT_ENABLED,
	U_TEXT_COLOR,

	U_FOG_COLOR,
	U_FOG_START,
	U_FOG_END,
	U_FOG_DENSITY,
	U_FOG_TYPE,
	U_FOG_ENABLE,

	U_OBJECT_ALPHA,

	U_TOTAL,
};

class View
{
public:
	View(Model * theModel);
	virtual ~View(void);

	BOOL CreateGLWindow(const char * title, int width, int height, int bits);
	BOOL InitGL();
	BOOL InitProgramInfo();
	BOOL InitLightsInfo();

	void setInputHandler(InputHandler * ptrToHandler);
	InputHandler * getInputHandler();

	void SwapBuffers();

	virtual void Update(double dt);
	Model * getModel();

	// Rendering Functions
	void RenderMesh(Mesh * mesh, bool enableLight, bool enableFog);
	void Render2DMesh(Mesh *mesh, bool enableLight, bool enableFog, float sizeX, float sizeY, float transX, float transY, float rotateValue = 0.f);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderGui(Gui * gui, Mesh * textMesh);
	void RenderBuffer(CharBuffer * buffer, Mesh * textMesh);
	void RenderTreeBranch(Branch * branch);
	void RenderSprites(Mesh* mesh, int ID, const float size, const float x, const float y);

	int getWindowWidth();
	int getWindowHeight();
	GLFWwindow * getWindow();

	void LoadOrthoCamera();
	void LoadPerspectiveCamera(double fov);

	MS projectionStack;
	MS viewStack;
	MS modelStack;
private:
	GLFWwindow * m_window;
	Model * theModel;
	InputHandler * m_cInputHandler;

	bool m_bActive;
	bool m_bFullScreen;

	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	int m_iWindow_Width;
	int m_iWindow_Height;
	std::string m_sWindow_Title;

	Light lights[1];

	GLint m_viewPort[4];

	VIEW_STATE m_eViewState;

	float FPS;
};

#endif