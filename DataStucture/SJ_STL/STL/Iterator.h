#pragma once
#include "Node.h"
template<typename T>
class vectorIterator
{
public:
	vectorIterator() : _ptr(nullptr)
	{

	}

	vectorIterator(T* ptr) : _ptr(ptr)
	{

	}

	vectorIterator& operator++()
	{
		_ptr++;
		return *this;
	}

	vectorIterator operator++(int)
	{
		vectorIterator temp = *this;
		_ptr++;
		return temp;
	}

	vectorIterator& operator--()
	{
		_ptr--;
		return *this;
	}

	vectorIterator operator--(int)
	{
		vectorIterator temp = *this;
		_ptr--;
		return temp;
	}

	vectorIterator operator+(const int count)
	{
		vectorIterator temp = *this;
		temp._ptr += count;
		return temp;
	}

	vectorIterator operator-(const int count)
	{
		vectorIterator temp = *this;
		temp._ptr -= count;
		return temp;
	}

	bool operator==(const vectorIterator& right)
	{
		return _ptr == right._ptr;
	}

	bool operator!=(const vectorIterator& right)
	{
		return !(*this == right);
	}

	T& operator*()
	{
		return *_ptr;
	}

public:
	T* _ptr;

};

template<typename T>
class listIterator
{
public:
	listIterator() : _node(nullptr)
	{

	}
	listIterator(Node<T>* node) : _node(node)
	{

	}

	listIterator& operator++()
	{
		_node = _node->_next;
		return *this;
	}

	listIterator operator++(int)
	{
		listIterator<T> temp = *this;
		_node = _node->_next;
		return temp;
	}

	listIterator& operator--()
	{
		_node = _node->_prev;
		return *this;
	}

	listIterator operator--(int)
	{
		listIterator<T> temp = *this;
		_node = _node->_prev;
		return temp;
	}

	T& operator*()
	{
		return _node->_data;
	}

	bool operator==(const listIterator& right)
	{
		return _node == right._node;
	}

	bool operator!=(const listIterator& right)
	{
		return _node != right._node;
	}
public:
	Node<T>* _node;
};

