// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TextureShader.h"
#include "PositionShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void firstPass();
	void secondPass();
	void drawPlayerLocation();
	void finalPass();
	void gui();

private:
	PlaneMesh* planeMesh;
	OrthoMesh* orthoMesh1;
	OrthoMesh *orthoMesh2;
	LightShader* lightShader;
	TextureShader* textureShader;
	PositionShader* positionShader;

	RenderTexture* renderTexture1;
	RenderTexture *renderTexture2;

	Light* light;
	Camera *rttCam1;
	Camera *rttCam2;
	float doTextures;	//Set this value to 0 to turn off lighting --- Set this value to any positive value to turn on lighting
};

#endif