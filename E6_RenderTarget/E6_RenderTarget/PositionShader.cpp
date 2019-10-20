#include "PositionShader.h"

PositionShader::PositionShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"light_vs.cso", L"position_ps.cso");
}


PositionShader::~PositionShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the light constant buffer.
	if (positionBuffer)
	{
		positionBuffer->Release();
		positionBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void PositionShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(PositionBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightBufferDesc, NULL, &positionBuffer);

}


void PositionShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, const XMMATRIX &orthoViewMatrix, const XMMATRIX &orthoMatrix, int screenWidth, int screenHeight,
											ID3D11ShaderResourceView* texture, XMFLOAT3 playerPosition)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(orthoViewMatrix);
	tproj = XMMatrixTranspose(orthoMatrix);
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	//Multiply player position by matrices to map it to screen space
	XMFLOAT4 positionFloat = XMFLOAT4(playerPosition.x, playerPosition.y, playerPosition.z, 1.0f);
	XMVECTOR positionVector = XMLoadFloat4(&positionFloat);		//Load the float4 into a vector for matrix multiplication
	positionVector = XMVector4Transform(positionVector, worldMatrix);
	positionVector = XMVector4Transform(positionVector, viewMatrix);
	positionVector = XMVector4Transform(positionVector, projectionMatrix);
	positionVector /= XMVectorGetByIndex(positionVector, 3);

	//Set up a float2 using the values in the vector and finish converting to screenspace
	XMFLOAT2 position2D;
	position2D.x = XMVectorGetByIndex(positionVector, 0);
	position2D.y = XMVectorGetByIndex(positionVector, 1);

	position2D.x *= 0.5f;
	position2D.y *= -0.5f;

	position2D.x += 0.5f;
	position2D.y += 0.5f;

	position2D.x *= screenWidth;
	position2D.y *= screenHeight;


	//Additional
	// Send position data to pixel shader
	PositionBufferType* positionPtr;
	deviceContext->Map(positionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	positionPtr = (PositionBufferType*)mappedResource.pData;
	positionPtr->playerPosition = position2D;
	deviceContext->Unmap(positionBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &positionBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}

