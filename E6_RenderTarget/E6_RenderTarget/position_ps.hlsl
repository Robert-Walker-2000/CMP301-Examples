//Position Pixel Shader
//Draws the players position onto a texture

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer PositionBuffer : register(b0)
{
	float2 playerPosition;
	float2 padding;
}

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float2 centre = playerPosition;
	float2 pixelPos = float2(input.position.x, input.position.y);
	float2 distanceVector;
	int distance;

	//Calculate the distance of the pixel from the players position. Allows for a circle to be drawn around the player on the map
	distanceVector = centre - pixelPos;
	distance = length(distanceVector);
	distance = abs(distance);
	

	//Sample the texture or replace it with a blip for the players location
	if (distance >= 10)
		textureColour = texture0.Sample(sampler0, input.tex);
	else
		textureColour = float4(0.0f, 1.0f, 0.0f, 1.0f);
	
	return textureColour;
}