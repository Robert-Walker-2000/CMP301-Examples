#include "Spotlight.h"

Spotlight::Spotlight()
{
	cutoff = 0.0f;
}

Spotlight::~Spotlight()
{
}

void Spotlight::setCutoff(float angle)
{
	//Take in cutoff angle in degrees
	cutoff = angle;

	//Clamp the min/max values for the cutoff angle in degrees
	if (cutoff > MAXANGLE)
	{
		cutoff = MAXANGLE;
	}
	else if(cutoff < 0.0f)
	{ 
		cutoff = 0.0f;
	}

	//Convert cutoff angle to radians
	cutoff = XMConvertToRadians(cutoff);
}

float Spotlight::getCutoff()
{
	//Return the cutoff angle in radians
	return cutoff;
}