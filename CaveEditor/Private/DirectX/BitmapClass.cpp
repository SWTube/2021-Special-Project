#include "pch.h"
#include "framework.h"
#include "TextureClass.h"
#include "BitmapClass.h"


BitmapClass::BitmapClass()
{
}


BitmapClass::BitmapClass(const BitmapClass& other)
{
}


BitmapClass::~BitmapClass()
{
}


bool BitmapClass::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	// ȭ�� ũ�⸦ ��������� ����
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// �������� ��Ʈ���� �ȼ��� ũ�⸦ ����
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// ���� ������ ��ġ�� ������ �ʱ�ȭ�մϴ�.
	m_previousPosX = -1;
	m_previousPosY = -1;

	m_posX = screenWidth / 2;
	m_posY = screenHeight / 2;

	// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
	if (!InitializeBuffers(device))
	{
		return false;
	}
	
	return true;
	// �� ���� �ؽ�ó�� �ε��մϴ�.
	//return LoadTexture(device, textureFilename);	
}


void BitmapClass::Shutdown()
{
	// �� �ؽ��ĸ� ��ȯ�մϴ�.
	ReleaseTexture();

	// ���ؽ� �� �ε��� ���۸� �����մϴ�.
	ShutdownBuffers();
}


bool BitmapClass::Render(ID3D11DeviceContext* deviceContext)
{
	// ȭ���� �ٸ� ��ġ�� �������ϱ� ���� ���� ���� ���۸� �ٽ� �����մϴ�.
	if(!UpdateBuffers(deviceContext, m_posX, m_posY))
	{
		return false;
	}

	// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� �����ϴ�.
	RenderBuffers(deviceContext);

	return true;
}


int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	// ���� �迭�� ���� ���� �ε��� �迭�� �ε��� ���� �����մϴ�.
	m_indexCount = m_vertexCount = 6;

	// ���� �迭�� ����ϴ�.
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// �����ͷ� �ε��� �迭�� �ε��մϴ�.
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// ���� ���� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// ���� �ε��� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� �����մϴ�.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void BitmapClass::ShutdownBuffers()
{
	// �ε��� ���۸� �����մϴ�.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���� ���۸� �����մϴ�.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void BitmapClass::SetPos(float x, float y)
{
	m_posX = x;
	m_posY = y;
}

void BitmapClass::Move(float x, float y)
{
	m_posX += x;
	m_posY += y;
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, float positionX, float positionY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;


	// �� ��Ʈ���� ������ �� ��ġ�� ������� ���� ��� ���� ���۸� ������Ʈ ���� ���ʽÿ�.
	// ���� �ùٸ� �Ű� ������ �ֽ��ϴ�.
	if(!isNeedUpdate && (positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}
	
	// ����� ��� �������Ǵ� ��ġ�� ������Ʈ�մϴ�.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// ��Ʈ �� ������ ȭ�� ��ǥ�� ����մϴ�.
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX - (float)m_bitmapWidth/2;

	// ��Ʈ �� �������� ȭ�� ��ǥ�� ����մϴ�.
	right = left + (float)m_bitmapWidth;

	// ��Ʈ �� ����� ȭ�� ��ǥ�� ����մϴ�.
	top = (float)(m_screenHeight / 2) - (float)positionY + (float)m_bitmapHeight/2;

	// ��Ʈ �� �Ʒ����� ȭ�� ��ǥ�� ����մϴ�.
	bottom = top - (float)m_bitmapHeight;

	// ���� �迭�� ����ϴ�.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// ���� �迭�� �����͸��ε��մϴ�.
	// ù ��° �ﰢ��
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// �� ��° �ﰢ��
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	// ���ؽ� ���۸� �� �� �ֵ��� ��޴ϴ�.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	verticesPtr = (VertexType*)mappedResource.pData;

	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// ���� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// �� �̻� �ʿ����� ���� ������ �迭�� �����մϴ�.
	delete [] vertices;
	vertices = 0;

	isNeedUpdate = false;
	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ������ ������ �������� �����մϴ�.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// ������ �� �� �ֵ��� �Է� ����������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ����������� �ε��� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	// �ؽ�ó ������Ʈ�� �����Ѵ�.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}
	
	// �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
	return m_Texture->Initialize(device, filename);
}


void BitmapClass::ReleaseTexture()
{
	// �ؽ�ó ������Ʈ�� �������Ѵ�.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}