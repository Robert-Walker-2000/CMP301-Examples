// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

#define NUM_LIGHTS 3	//Ensure the number of lights here is the same as in LightShader.h

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambient[NUM_LIGHTS];
	float4 diffuse[NUM_LIGHTS];
	float4 position[NUM_LIGHTS];
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 ldiffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(ldiffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 averageAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		averageAmbient += ambient[i];
	}
	averageAmbient /= NUM_LIGHTS;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	float4 textureColour = texture0.Sample(sampler0, input.tex);

	float4 lightColour = float4(0.0f, 0.0f, 0.0f, 0.0f);

	lightColour += averageAmbient;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float3 lightVector;

		if (position[i].w == 1)
		{
			lightVector = normalize(position[i].xyz);
		}
		else
		{
			lightVector = normalize(position[i] - input.worldPosition);
		}

		lightColour += calculateLighting(lightVector, input.normal, diffuse[i]);
	}

	return lightColour * textureColour;
}



