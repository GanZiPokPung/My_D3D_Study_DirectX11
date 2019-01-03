#pragma once

// warning C4316 ó����
// Ư�� ���μ��� ĳ�ö��� ũ��� �����Ͽ� �Ѳ�����
// ����Ͽ� CPU ĳ�� ó�� ������ �����ֱ� ���� ����.
// ��� ���� ��� ó��
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