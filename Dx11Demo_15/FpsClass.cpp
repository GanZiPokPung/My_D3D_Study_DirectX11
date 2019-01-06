#include "stdafx.h"
#include <mmsystem.h>
#include "FpsClass.h"


FpsClass::FpsClass()
{
}


FpsClass::FpsClass(const FpsClass& other)
{
}


FpsClass::~FpsClass()
{
}


void FpsClass::Initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}


void FpsClass::Frame()
{
	m_count++;

	// �����ð����� 1�� �̻� ���̰� ���ٸ�
	if (timeGetTime() >= (m_startTime + 1000))
	{
		// �� �ð����� ���������� count�� fps�� �ȴ�.
		m_fps = m_count;
		m_count = 0;

		// �ð��� �ٽ� ����
		m_startTime = timeGetTime();
	}
}


int FpsClass::GetFps()
{
	return m_fps;
}