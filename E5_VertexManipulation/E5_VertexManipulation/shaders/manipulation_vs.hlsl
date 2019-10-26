// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
Texture2D heightMap : register(t0);
SamplerState heightSampler : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer TimeBuffer : register(b1)
{
	float time;
	float amplitude;
	float frequency;
	float padding;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

OutputType main(InputType input)
{
	OutputType output;

	//Wave calculations
	/*
	//Manipulate the y value to generate a sine wave
	input.position.y = amplitude * sin(input.position.x * frequency + time);
	input.position.y += cos(input.position.z + time);

	//Recalculate the normals after the vertex manipulation
	input.normal.x = 1 - cos(input.position.x * frequency + time);
	input.normal.y = abs(cos(input.position.x + time));
	*/

	float mappedHeight;
	float4 heightColour;

    heightColour = heightMap.SampleLevel(heightSampler, input.tex, 0);
	mappedHeight = 10 * length(heightColour);
	input.position.y = mappedHeight;
	

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	return output;
}