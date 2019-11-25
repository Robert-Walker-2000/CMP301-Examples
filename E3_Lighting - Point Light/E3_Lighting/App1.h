// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"


class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

private:
	LightShader* shader;
	PlaneMesh* mesh;
	Spotlight* light;

	int lightx = 50;
	int lighty = 10;
	int lightz = 50;

	float lightR = 0.5f;
	float lightG = 0.5f;
	float lightB = 0.5f;
	
	float cutoff = 30.0f;
};

#endif