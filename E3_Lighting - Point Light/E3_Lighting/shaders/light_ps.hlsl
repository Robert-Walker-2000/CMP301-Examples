// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 diffuseColour;
	float4 ambientColour;
	float3 lightPosition;
	float padding;
	float3 spotlightDirection;
	float cutoffAngle;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 rayDirection, float3 spotDirection, float3 normal, float4 diffuse)
{
	float4 colour = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float cosOfAngle = dot(-rayDirection, spotDirection);
	float cosOfCutoff = cos(cutoffAngle);


	if (cosOfAngle > cosOfCutoff)
	{
		float intensity = saturate(dot(normal, rayDirection));
		colour = saturate(diffuse * intensity);
	}

	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour;
	float3 lightVector;
	float distance;

	lightVector = lightPosition - input.worldPosition;
	distance = length(lightVector);

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(sampler0, input.tex);
	lightVector = normalize(lightVector);
	lightColour = ambientColour + (calculateLighting(lightVector, spotlightDirection, input.normal, diffuseColour));

	//Calculate attenuation (linear attenuation only)
	lightColour /= (1 + (0.01 * distance));
	
	return lightColour * textureColour;
}



