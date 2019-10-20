#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class PositionShader : public BaseShader
{
private:
	struct PositionBufferType
	{
		XMFLOAT2 playerPosition;
		XMFLOAT2 padding;
	};

public:
	PositionShader(ID3D11Device* device, HWND hwnd);
	~PositionShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, const XMMATRIX &orthoView, const XMMATRIX &ortho, 
							int screenWidth, int screenHeight, ID3D11ShaderResourceView* texture, XMFLOAT3 playerPosition);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* positionBuffer;
};
