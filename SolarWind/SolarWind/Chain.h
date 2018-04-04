#pragma once
#include <map>
#include <vector>

template <class Type>
class Chain
{
	public:
		Chain(int depth=0, int maxDepth=1);

		bool Contains(Type key);
		bool IsLastLink();

		bool Add(Type key);
		bool Update(Type key);
		Chain<Type>* WalkTheChain(Type key);
		std::map<Type, int> Distribution();

	private:
		int _depth;
		int _maxDepth;
		std::map<Type, Chain<Type>> _chainLink;
		std::map<Type, int> _dist;
};

template <class Type>
Chain<Type>::Chain(int depth, int maxDepth)
{
	_depth = depth;
	_maxDepth = maxDepth;
	if (IsLastLink())
	{
		_dist = std::map<Type, int>();
	}
	else
	{
		_chainLink = std::map<Type, Chain<Type>>();
	}
}

template <class Type>
bool Chain<Type>::Contains(Type key)
{
	if (IsLastLink())
		return _dist.count(key) == 1;
	return _chainLink.count(key) == 1;
}

template <class Type> 
bool Chain<Type>::Add(Type key)
{
	bool success = false;

	if (IsLastLink())
	{
		if (!Contains(key))
		{
			_dist.insert(std::make_pair(key, 1));
			success = true;
		}
	}
	else
	{	
		if (!Contains(key))
		{
			_chainLink.insert(std::make_pair(key, Chain<Type>(_depth + 1, _maxDepth)));
			success = true;
		}
	}

	return success;
}

template <class Type>
bool Chain<Type>::Update(Type key)
{
	if (IsLastLink() && Contains(key))
	{
		_dist[key] += 1;
		return true;
	}
	return false;
}

template <class Type>
Chain<Type>* Chain<Type>::WalkTheChain(Type key)
{
	return &_chainLink[key];
}

template <class Type>
bool Chain<Type>::IsLastLink()
{
	return _depth == _maxDepth;
}

template <class Type>
std::map<Type, int> Chain<Type>::Distribution()
{
	return _dist;
}
