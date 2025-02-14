// Lab1.cpp
// Lab 2 example, textured quad
#include "App1.h"

App1::App1()
{
	mesh1 = nullptr;
	mesh2 = nullptr;
	textureShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	textureMgr->loadTexture(L"shapes", L"res/shapes.png");

	// Create Mesh object and shader object
	mesh1 = new TexturedQuad(renderer->getDevice(), renderer->getDeviceContext());
	mesh2 = new TexturedQuad(renderer->getDevice(), renderer->getDeviceContext());
	textureShader = new TextureShader(renderer->getDevice(), hwnd);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (mesh1)
	{
		delete mesh1;
		mesh1 = 0;
	}

	if (mesh2)
	{
		delete mesh2;
		mesh2 = 0;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//Rotate the first quad
	/*rotation1--;
	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, AI_DEG_TO_RAD(rotation1));*/

	// Send geometry data, set shader parameters, render object with shader
	mesh1->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"shapes"), textureMgr->getTexture(L"brick"));
	textureShader->render(renderer->getDeviceContext(), mesh1->getIndexCount());

	//Translate the world matrix to reposition and rotate the second quad
	//worldMatrix = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, AI_DEG_TO_RAD(rotation2)), XMMatrixTranslation(2.0f, 0.0f, 0.0f));

	//Render the second quad
	/*mesh2->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"));
	textureShader->render(renderer->getDeviceContext(), mesh2->getIndexCount());*/

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	//ImGui::SliderInt("mesh2 rotation", &rotation2, 0, 360);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

