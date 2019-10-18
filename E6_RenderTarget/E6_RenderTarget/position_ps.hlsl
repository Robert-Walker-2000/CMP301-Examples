//Position Pixel Shader
//Draws the players position onto a texture

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer PositionBuffer : register(b0)
{
	float3 playerPosition;
	float padding;
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
	float2 centre = float2(800, 400);
	float2 pixelPos = float2(input.position.x, input.position.y);
	float2 distanceVector;
	int distance;
	distanceVector = centre - pixelPos;
	distance = length(distanceVector);
	distance = abs(distance);
	

	//Sample the texture
	if (distance >= 10)
		textureColour = texture0.Sample(sampler0, input.tex);
	else
		textureColour = float4(0.0f, 1.0f, 0.0f, 1.0f);
	

	return textureColour;
}