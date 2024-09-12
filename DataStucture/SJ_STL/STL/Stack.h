#pragma once
#include <vector>
#include <list>
using namespace std;
template<typename T, typename Container = vector<T>>
class Stack
{
public:
	void push(const T& value)
	{
		_container.push_back(value);
	}
	void pop()
	{
		_container.pop_back();
	}
	T& top()
	{
		return _container.back();
	}
	bool empty() { return _container.empty(); }
	size_t size() { return _container.size(); }
private:
	//vector<T> _container;
	Container _container;
};

