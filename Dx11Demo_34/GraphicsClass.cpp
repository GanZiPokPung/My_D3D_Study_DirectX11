#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라 포지션을 설정한다
	m_Camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -10.0f));
	m_Camera->SetRotation(XMFLOAT3(0.0f, 45.0f, 0.0f));

	// 텍스처 쉐이더 객체를 생성한다.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// 텍스처 쉐이더 객체를 초기화한다.
	if(!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// 바닥 모델 객체를 만듭니다.
	m_FloorModel = new ModelClass;
	if(!m_FloorModel)
	{
		return false;
	}

	// 바닥 모델 객체를 초기화합니다.
	if(!m_FloorModel->Initialize(m_Direct3D->GetDevice(), "../Dx11Demo_34/data/floor.txt", L"../Dx11Demo_34/data/grid01.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the floor model object.", L"Error", MB_OK);
		return false;
	}

	// 빌보드 모델 객체를 만듭니다.
	m_BillboardModel = new ModelClass;
	if(!m_BillboardModel)
	{
		return false;
	}

	// 빌보드 모델 객체를 초기화합니다.
	if(!m_BillboardModel->Initialize(m_Direct3D->GetDevice(), "../Dx11Demo_34/data/square.txt", L"../Dx11Demo_34/data/seafloor.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the billboard model object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// 빌보드 모델 객체를 해제합니다.
	if(m_BillboardModel)
	{
		m_BillboardModel->Shutdown();
		delete m_BillboardModel;
		m_BillboardModel = 0;
	}

	// 바닥 모델 객체를 해제합니다.
	if(m_FloorModel)
	{
		m_FloorModel->Shutdown();
		delete m_FloorModel;
		m_FloorModel = 0;
	}

	// 텍스처 쉐이더 객체를 해제합니다.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// 카메라 객체를 해제합니다.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체를 해제합니다.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}


bool GraphicsClass::Frame(XMFLOAT3& position)
{
	// 카메라 위치를 업데이트합니다.
	m_Camera->SetPosition(position);

	// 그래픽 장면을 렌더링합니다.
	return Render();
}


bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	XMFLOAT3 cameraPosition, modelPosition;

	// 장면을 시작할 버퍼를 지운다.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 오쏘 (ortho) 행렬을 가져옵니다.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// 드로잉을 준비하기 위해 바닥 파이프 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치합니다.
	m_FloorModel->Render(m_Direct3D->GetDeviceContext());

	// 텍스처 쉐이더를 사용하여 바닥 모델을 렌더링합니다.
	if(!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FloorModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_FloorModel->GetTexture()))
	{
		return false;
	}

	// 카메라 위치를 얻는다.
	cameraPosition = m_Camera->GetPosition();

	// 빌보드 모델의 위치를 ​​설정합니다.
	modelPosition.x = 0.0f;
	modelPosition.y = 1.5f;
	modelPosition.z = 0.0f;

	// 아크 탄젠트 함수를 사용하여 현재 카메라 위치를 향하도록 빌보드 모델에 적용해야하는 회전을 계산합니다.
	double angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / XM_PI);

	// 회전을 라디안으로 변환합니다.
	float rotation = (float)angle * 0.0174532925f;

	// 세계 행렬을 사용하여 원점에서 빌보드 회전을 설정합니다.
	worldMatrix = XMMatrixRotationY(rotation);

	// 빌보드를 할 모델의 위치값
	// 빌보드 모델에서 번역 행렬을 설정합니다.
	translateMatrix = XMMatrixTranslation(modelPosition.x, modelPosition.y, modelPosition.z);

	// 마지막으로 회전 및 변환 행렬을 결합하여 빌보드 모델의 최종 행렬을 만듭니다.
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 드로잉을 준비합니다.
	m_BillboardModel->Render(m_Direct3D->GetDeviceContext());

	// 텍스처 셰이더를 사용하여 모델을 렌더링합니다.
	if(!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_BillboardModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_BillboardModel->GetTexture()))
	{
		return false;
	}

	// 렌더링 된 장면을 화면에 표시합니다.
	m_Direct3D->EndScene();

	return true;
}