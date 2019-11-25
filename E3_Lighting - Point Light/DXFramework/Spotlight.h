#pragma once
#include "Light.h"

#define MAXANGLE 90.0f

class Spotlight :
	public Light
{
public:
	Spotlight();
	~Spotlight();

	void setCutoff(float angle);	//Set the cutoff angle of the spotlight
	float getCutoff();				//Get the cutoff angle of spotlight, returns float

protected:
	float cutoff;	//The maximum angle between the spotlight direction vector and the vector to the surface where lighting is still applied
};

