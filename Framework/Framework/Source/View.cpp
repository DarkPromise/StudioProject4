#include "View.h"
#include "LuaReader.h"
#include "Branch.h"

View::View(Model * theModel)
: theModel(theModel)
, m_sWindow_Title("")
, m_iWindow_Height(-1)
, m_iWindow_Width(-1)
, m_bActive(true)
, m_bFullScreen(false)
, FPS(0.f)
{
	std::cout << "View Created" << std::endl;
}

View::~View()
{
	glfwTerminate();
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	//std::cout << xOffset << "," << yOffset << std::endl;
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

BOOL View::InitGL()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.f, 0.f, 0.f, 1.f); //Initialize Background Colour
	glEnable(GL_DEPTH_TEST); //Enable Depth Test
	glDepthFunc(GL_LEQUAL); //If the image is closer to camera, allow it to be ontop
	glEnable(GL_CULL_FACE); //Allow culling of faces
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Allows the interior of polygon to be filled
	glEnable(GL_BLEND); //Allow blending of textures with alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha stuff
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //Nicest Perspective that can be calculated
	glfwSwapInterval(0);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	if (!InitProgramInfo())
	{
		std::cout << "Failed to Load Program Parameters" << std::endl;
	}
	if (!InitLightsInfo())
	{
		std::cout << "Failed to Load Lights" << std::endl;
	}

	this->theModel->Init();

	return TRUE;
}

BOOL View::InitProgramInfo()
{
	m_programID = LoadShaders("Shaders//Shadow.vertexshader", "Shaders//Shadow.fragmentshader");
	//Model View
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	//Material
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	//Lights
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	//Color
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	//Text
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	//Fog *Not yet added
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fog[0].color.r");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fog[0].start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fog[0].end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fog[0].density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fog[0].type");
	m_parameters[U_FOG_ENABLE] = glGetUniformLocation(m_programID, "fogEnabled");
	//Alpha
	m_parameters[U_OBJECT_ALPHA] = glGetUniformLocation(m_programID, "alpha");

	glUseProgram(m_programID); //Use our own shader

	std::cout << "Program Parameters Initialized" << std::endl;

	return TRUE;
}

BOOL View::InitLightsInfo()
{
	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(-20, 30, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	std::cout << "Lights Initialized" << std::endl;

	return TRUE;
}

BOOL View::CreateGLWindow(const char * title, int width, int height, int bits)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	if (m_bFullScreen)
	{
		m_window = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	}

	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_window);

	// For redirecting the callback from InputHandler
	glfwSetWindowUserPointer(m_window, m_cInputHandler);

	// For setting keyboard callback
	auto InputHandler_Key_CallBack = [](GLFWwindow * window, int key, int scancode, int action ,int mods)
	{
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->Key_Callback(key, scancode, action, mods);
	};

	// For setting mouse callback
	auto InputHandler_Mouse_CallBack = [](GLFWwindow * window, int button, int action, int mods)
	{
		static_cast<InputHandler*>(glfwGetWindowUserPointer(window))->Mouse_Callback(button, action, mods);
	};

	// Sets the callbacks
	glfwSetKeyCallback(m_window, InputHandler_Key_CallBack);
	glfwSetMouseButtonCallback(m_window, InputHandler_Mouse_CallBack);
	glfwSetWindowSizeCallback(m_window, resize_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	glGetIntegerv(GL_VIEWPORT, m_viewPort);
	m_iWindow_Width = width;
	m_iWindow_Height = height;
	glViewport(0, 0, m_iWindow_Width, m_iWindow_Height);

	glewExperimental = true;

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	if (!InitGL())
	{
		return FALSE;
	}

	// Move the screen to the middle 
	glfwSetWindowPos(m_window, ((float)(glfwGetVideoMode(glfwGetPrimaryMonitor())->width) * 0.5f) - ((float)(this->m_iWindow_Width) * 0.5f), ((float)(glfwGetVideoMode(glfwGetPrimaryMonitor())->height) * 0.5f) - ((float)(this->m_iWindow_Height) * 0.5f));

	return TRUE;
}

void View::Update(double dt)
{
	FPS = (float)(1.f / dt);

	if (m_cInputHandler->IsKeyPressed(GLFW_KEY_F1))
	{
		if (m_cInputHandler->IsKeyTriggered(GLFW_KEY_F1))
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
		}
	}

	if (m_cInputHandler->IsKeyPressed(GLFW_KEY_F2))
	{
		if (m_cInputHandler->IsKeyTriggered(GLFW_KEY_F2))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	}

	// Resizing Changes
	glGetIntegerv(GL_VIEWPORT, m_viewPort);
	m_iWindow_Width = m_viewPort[VIEWPORT_WIDTH];
	m_iWindow_Height = m_viewPort[VIEWPORT_HEIGHT];
	switch (m_eViewState)
	{
	case VIEW_ORTHO:
		LoadOrthoCamera();
		break;
	case VIEW_PERSPECTIVE:
		LoadPerspectiveCamera(90.f);
		break;
	}
}

void View::SwapBuffers()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void View::setInputHandler(InputHandler * ptrToHandler)
{
	this->m_cInputHandler = ptrToHandler;
}

InputHandler* View::getInputHandler()
{
	return this->m_cInputHandler;
}

void View::RenderMesh(Mesh *mesh, bool enableLight, bool enableFog)
{
	if (mesh == NULL)
		return;

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	// Alpha
	glUniform1f(m_parameters[U_OBJECT_ALPHA], mesh->alpha);

	if (enableLight && LIGHTS_ENABLED)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
		//
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//
		glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
	}

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void View::Render2DMesh(Mesh *mesh, bool enableLight, bool enableFog, float sizeX, float sizeY, float transX, float transY, float rotateValue)
{
	if (mesh == NULL)
		return;

	Mtx44 ortho;
	ortho.SetToOrtho(0, (float)m_viewPort[VIEWPORT_WIDTH], 0, (float)m_viewPort[VIEWPORT_HEIGHT], -10, 100);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();

	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(transX, transY, 0);
	modelStack.Rotate(rotateValue, 0, 0, 1);
	modelStack.Scale(sizeX, sizeY, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();

	// Alpha
	glUniform1f(m_parameters[U_OBJECT_ALPHA], mesh->alpha);

	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
		//
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//
		glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
	}

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void View::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (mesh == NULL)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, (float)m_viewPort[VIEWPORT_WIDTH], 0, (float)m_viewPort[VIEWPORT_HEIGHT], -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	
	// Alpha
	glUniform1f(m_parameters[U_OBJECT_ALPHA], mesh->alpha);

	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation((i*0.4f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void View::RenderGui(Gui * gui, Mesh * textMesh)
{
	Color color;
	if (gui->getHighlighted() == true)
	{
		color = Color(0.f, 0.f, 1.f);
	}
	else
	{
		color = Color(1.f, 0.f, 0.f);
	}
	//Render2DMesh(gui->getMesh(), false, false, 1.f, 1.f, gui->getX(), (m_iWindow_Height - gui->getY())); //Convert to screen coordinates (Bottom Left to Top Right)
	RenderTextOnScreen(textMesh, gui->getText(), color , gui->getSize(), m_iWindow_Width * gui->getWidthOffset() - (gui->getTextOffset()), m_iWindow_Height * (1.f - gui->getHeightOffset()) - (gui->getSize() * 0.5f));
}

void View::RenderBuffer(CharBuffer * buffer, Mesh * textMesh)
{
	Color color;
	if (buffer->getHighlighted() == true)
	{
		glLineWidth(2.f);
		color = Color(1.f, 0.f, 0.f);
	}
	else
	{
		glLineWidth(1.f);
		color = Color(1.f, 1.f, 1.f);
	}
	RenderTextOnScreen(textMesh, buffer->toString(), color, buffer->getFontSize(), m_iWindow_Width * buffer->getWidthOffset() - (buffer->getTextOffset()), m_iWindow_Height * (1.f - buffer->getHeightOffset()) - (buffer->getFontSize() * 0.5f));
}

void View::RenderTreeBranch(Branch * branch)
{
	modelStack.PushMatrix();
	RenderMesh(branch->getMesh(), false, false);
	modelStack.PopMatrix();
}

void View::RenderSprites(Mesh* mesh, int id, const float size, const float x, const float y)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 1024, 0, 800, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	//if (!mesh || mesh->textureID <= 0)
	//	return;

	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	Mtx44 characterSpacing;
	characterSpacing.SetToTranslation(0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render((unsigned)id * 6, 6);

	glBindTexture(GL_TEXTURE_2D, 0);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

int View::getWindowHeight()
{
	return this->m_iWindow_Height;
}

int View::getWindowWidth()
{
	return this->m_iWindow_Width;
}

GLFWwindow * View::getWindow()
{
	return this->m_window;
}

void View::LoadOrthoCamera()
{
	this->m_eViewState = VIEW_ORTHO;
	Mtx44 Projection;
	Projection.SetToOrtho(0, (float)m_viewPort[VIEWPORT_WIDTH], 0, (float)m_viewPort[VIEWPORT_HEIGHT], -10, 100);
	projectionStack.LoadIdentity();
	projectionStack.LoadMatrix(Projection);
}

void View::LoadPerspectiveCamera(double fov)
{
	this->m_eViewState = VIEW_PERSPECTIVE;
	Mtx44 Projection;
	Projection.SetToPerspective(fov, (float)m_viewPort[VIEWPORT_WIDTH] / (float)m_viewPort[VIEWPORT_HEIGHT], 0.1, 15000); // **Note : 10000 should be the standard.
	projectionStack.LoadIdentity();
	projectionStack.LoadMatrix(Projection);
}

