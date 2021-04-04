#pragma once
#include "stdafx.h"

#define SIZE 10

template<typename T>
class Queue
{
public:
	Queue()
	{
		queue = new T[SIZE];
	}
	~Queue()
	{
		SafeDelete(queue);
	}

	void Enqueue(T data)
	{
		queue[back] = data;
		back++;
		back %= SIZE;
	}

	T Dequeue()
	{
		T result = T();
		if (Count() > 0)
		{
			result = queue[front];
			front++;
			front %= SIZE;
		}
		return result;
	}

	int Count()
	{
		return back - front;
	}

	bool IsEmpty()
	{
		return front == back;
	}

	void Clear()
	{
		front = 0;
		back = 0;
	}

private:
	T* queue;
	UINT front = 0;
	UINT back = 0;
};