#pragma once
template<typename T>
class Node
{
public:
	Node() : _next(nullptr), _prev(nullptr), _data(T())
	{

	}

	Node(const T& value) : _next(nullptr), _prev(nullptr), _data(value)
	{

	}

public:
	Node* _next;
	Node* _prev;
	T		_data;
};

