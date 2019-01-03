#pragma once

// warning C4316 처리용
// 특정 프로세서 캐시라인 크기로 정렬하여 한꺼번에
// 계산하여 CPU 캐시 처리 성능을 높혀주기 위해 설정.
// 행렬 연산 고속 처리
template<size_t Alignment>
class AlignedAllocationPolicy
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, Alignment);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};