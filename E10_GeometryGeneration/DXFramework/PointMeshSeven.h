#pragma once
#include "BaseMesh.h"
using namespace DirectX;

class PointMeshSeven :
	public BaseMesh
{
public:
	PointMeshSeven(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~PointMeshSeven();

	void sendData(ID3D11DeviceContext*);

protected:
	void initBuffers(ID3D11Device* device);
};

