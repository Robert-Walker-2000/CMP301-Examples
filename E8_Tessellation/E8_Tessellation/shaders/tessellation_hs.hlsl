// Tessellation Hull Shader
// Prepares control points for tessellation
cbuffer TessFactorBuffer : register(b0)
{
	float edgeFactor1;
	float edgeFactor2;
	float edgeFactor3;
	float edgeFactor4;
	float insideFactor1;
	float insideFactor2;
	float2 padding;
};

struct InputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;


    // Set the tessellation factors for the three edges of the triangle.
	output.edges[0] = edgeFactor1;
	output.edges[1] = edgeFactor2;
	output.edges[2] = edgeFactor3;
	output.edges[3] = edgeFactor4;

    // Set the tessellation factor for tessallating inside the triangle.
	output.inside[0] = insideFactor1;
	output.inside[1] = insideFactor2;

    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;


    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    // Set the input colour as the output colour.
    output.colour = patch[pointId].colour;

    return output;
}