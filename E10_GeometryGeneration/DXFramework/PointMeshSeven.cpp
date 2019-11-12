#include "PointMeshSeven.h"

PointMeshSeven::PointMeshSeven(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}

PointMeshSeven::~PointMeshSeven()
{
	BaseMesh::~BaseMesh();
}

void PointMeshSeven::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = 7;
	indexCount = 7;


	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);  // Top.
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	for (int i = 0; i < 3; i++)
	{
		vertices[i + 1].position = XMFLOAT3(-2.0f + 2.0f * i, 1.0f, 0.0f);
		vertices[i + 1].texture = XMFLOAT2(0.0f, 1.0f);
		vertices[i + 1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	}

	for (int i = 0; i < 3; i++)
	{
		vertices[i + 4].position = XMFLOAT3(-2.0f + 2.0f * i, -1.0f, 0.0f);
		vertices[i + 4].texture = XMFLOAT2(0.0f, 1.0f);
		vertices[i + 4].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	}

	// Load the index array with data.
	indices[0] = 0; //Middle
	indices[1] = 1; //Top Left
	indices[2] = 2;	//Top Mid
	indices[3] = 3;	//Top Right
	indices[4] = 4;	//Bottom Left
	indices[5] = 5;	//Bottom Mid
	indices[6] = 6;	//Bottom Right

					 // Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void PointMeshSeven::sendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}