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

	// 이전시간보다 1초 이상 차이가 난다면
	if (timeGetTime() >= (m_startTime + 1000))
	{
		// 그 시간까지 증가연산한 count가 fps가 된다.
		m_fps = m_count;
		m_count = 0;

		// 시간을 다시 갱신
		m_startTime = timeGetTime();
	}
}


int FpsClass::GetFps()
{
	return m_fps;
}