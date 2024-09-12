#pragma once
#include "Iterator.h"
#include "iostream"

template<typename T>
class Vector
{
public:
	Vector() : _data(nullptr), _size(0), _capacity(0)
	{

	}
	~Vector()
	{
		if (_data)
			delete[] _data;
	}

	void push_back(const T& val)
	{
		if (_size == _capacity)
		{
			int newCapacity = static_cast<int>(_capacity * 1.5);

			if (newCapacity == _capacity)
				newCapacity++;

			reserve(newCapacity);
		}

		_data[_size] = val;

		_size++;
	}

	void reserve(int capacity)
	{
		if (_capacity >= capacity)
			return;

		_capacity = capacity;

		T* newData = new T[_capacity];

		//for (int i = 0; i < _size; i++)
		//	newData[i] = _data[i];

		if (_data)
		{
			std::copy(_data, _data + _size, newData);
		}
		
		
		if (_data)
			delete[] _data;

		_data = newData;
	}

	T& operator[] (const int pos) { return _data[pos]; }

	int size() { return _size; }
	int capacity() { return _capacity; }
	void clear()
	{
		// 소멸자 호출 해야함
		if (_data)
		{
			delete[] _data;
			_data = new T[_capacity];
		}

		_size = 0;
	}

public:
	using iterator = vectorIterator<T>;

	iterator begin() { return iterator(&_data[0]); }
	iterator end() { return begin() + _size; }
	

private:
	T* _data;
	int _size;
	int _capacity;
};

