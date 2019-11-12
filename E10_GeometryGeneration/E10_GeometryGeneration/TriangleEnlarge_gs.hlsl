// triangle_gs
// Geometry shader that generates a triangle for every vertex.

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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

[maxvertexcount(3)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	float sX, sY, sZ;	//Scaling values for each axis
	sX = 2.0f;
	sY = 2.0f;
	sZ = 1.0f;

	matrix scaleMatrix = 
	{ 
		sX, 0.0f, 0.0f, 0.0f,
		0.0f, sY, 0.0f, 0.0f,
		0.0f, 0.0f, sZ, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};

	matrix scaledWorld = mul(worldMatrix, scaleMatrix);

	//Top
	output.position = input[0].position;
	output.position = mul(output.position, scaledWorld);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = float2(0.5f, 0.0f);
	output.normal = mul(input[0].normal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

	//Bottom Left
	output.position = input[1].position;
	output.position = mul(output.position, scaledWorld);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = float2(0.0f, 1.0f);
	output.normal = mul(input[1].normal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

	//Bottom Right
	output.position = input[2].position;
	output.position = mul(output.position, scaledWorld);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = float2(1.0f, 1.0f);
	output.normal = mul(input[2].normal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

	triStream.RestartStrip();
}