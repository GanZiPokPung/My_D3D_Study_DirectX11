#pragma once

/////////////
// GLOBALS //
/////////////
// 전체화면 여부
const bool FULL_SCREEN = false;
// 수직동기화 여부
const bool VSYNC_ENABLED = true;
// 투영에 대한 최소, 최대 거리
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class CameraClass;
class ModelClass;
class TextureShaderClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelClass* m_Model = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
private:
	float m_CamPosZ = -5.f;
};