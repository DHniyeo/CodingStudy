#pragma once
#include "Node.h"
#include "Iterator.h"
template<typename T>
class List
{
public:
	List() : _size(0)
	{
		_header = new Node<T>();
		_header->_next = _header;
		_header->_prev = _header;
	}

	~List()
	{
		while (_size > 0)
			pop_back();

		delete _header;
	}

	void push_back(const T& value)
	{
		AddNode(_header, value);
	}

	void pop_back()
	{
		if (_header->_prev == _header)
			return;

		RemoveNode(_header->_prev);
	}

	int size() { return _size; }

private:

	Node<T>* AddNode(Node<T>* before, const T& value)
	{
		Node<T>* node = new Node<T>(value);

		Node<T>* prevNode = before->_prev;
		prevNode->_next = node;
		node->_prev = prevNode;

		node->_next = before;
		before->_prev = node;

		_size++;

		return node;
	}

	Node<T>* RemoveNode(Node<T>* node)
	{
		Node<T>* nextNode = node->_next;
		Node<T>* prevNode = node->_prev;

		prevNode->_next = nextNode;
		nextNode->_prev = prevNode;
		
		if (node)
			delete node;

		_size--;

		return nextNode;
	}

	

public:
	using iterator = listIterator<T>;

	iterator begin() { return iterator(_header->_next); }
	iterator end() { return iterator(_header); }

	iterator insert(iterator it, const T& value)
	{
		Node<T>* node = AddNode(it._node, value);
		return iterator(node);
	}

	iterator erase(iterator it)
	{
		Node<T>* node = RemoveNode(it._node);
		return iterator(node);
	}

public:
	Node<T>* _header;
	int _size;
};

