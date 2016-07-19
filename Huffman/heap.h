#pragma once;
#include<iostream>
#include<vector>

using namespace std;

template<class T>
struct Less{
	bool operator()(T i, T j)
	{
		return i<j;
	}
};

template<class T, class com = Less<T> >
class Heap{
public:
	Heap()
		:_size(0)
	{}
	Heap(T *heap, size_t size)
		:_size(size)
	{
		for (int i = 0; i<_size; i++)
		{
			_heap.push_back(heap[i]);
		}
		for (int i = (_size - 2) / 2; i >= 0; --i)
		{
			heapdown(i);
		}
	}
void push(T w)
{
	_heap.push_back(w);
	_size += 1;
	heapout();
}
int size()
{
	return _size;
}
T top()
{
	return  _heap[0];
}
void Pop()
{
	swap(_heap[_size-1], _heap[0]);
	_heap.pop_back();
	--_size;
	heapdown(0);
}
void Display()
{
	for (int i = 0; i<_size; i++)
	{
		cout << _heap[i] << endl;
	}
}
protected:
	void heapout()
	{
		size_t child = _size - 1;
		size_t parent = (child - 1) / 2;
		while (child>0)
		{
			if (_com(_heap[child], _heap[parent]))
			{
				swap(_heap[child], _heap[parent]);
				child = parent;
				parent = (parent - 1) / 2;
			}
			else
			{
				break;
			}
		}
}
void heapdown(size_t parent)
{
	int child = 2 * parent + 1;
	while (child<_size)
	{
		if ((child + 1)<_size&&_com(_heap[child + 1], _heap[child]))
		{
			child++;
		}
		if (_com(_heap[child], _heap[parent]))
		{
			swap(_heap[child], _heap[parent]);
		}
		parent = child;
		child = parent * 2 + 1;
	}
}
	private:
		vector<T> _heap;
		size_t _size;
		com _com;
};

