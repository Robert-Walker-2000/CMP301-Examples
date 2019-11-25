// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


float4 main(InputType input) : SV_TARGET
{
	float4 textureColor1;
	float4 textureColor2;
	float4 blendedTextureColor;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor1 = texture0.Sample(Sampler0, input.tex);
	textureColor2 = texture1.Sample(Sampler0, input.tex);
	blendedTextureColor = (textureColor1 + textureColor2) / 2;

	//Invert colours
	//textureColor = float4(1.0f, 1.0f, 1.0f, 2 * textureColor.w) - textureColor;

	return blendedTextureColor;
}