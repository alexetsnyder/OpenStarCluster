#pragma once
#include <map>
#include <vector>
#include <deque>
#include <random>
#include "Chain.h"

template <class chunkT, class keyT>
class MarkovChain
{
	public:
		MarkovChain();
		MarkovChain(int order);
		void Init(int order);
		
		bool CreateFrequencyTable(std::vector<chunkT> chunks);
		bool GenerateNewChunk(int minLength, int maxLength, chunkT& outChunk);
		bool GetNextKey(std::deque<keyT> keys, keyT& outKey);

	private:
		bool AddToFrequencyTable(chunkT chunk);
		bool AddKeys(std::deque<keyT> keys);

		int _order;
		std::mt19937 _engine;
		Chain<keyT> _chain;
		std::vector<keyT> _keys;
};

template <class chunkT, class keyT>
MarkovChain<chunkT, keyT>::MarkovChain()
{

}

template <class chunkT, class keyT>
MarkovChain<chunkT, keyT>::MarkovChain(int order)
{
	Init(order);
}

template <class chunkT, class keyT>
void MarkovChain<chunkT, keyT>::Init(int order)
{
	_order = order;
	_chain.Init(0, order);
	std::random_device device;
	_engine = std::mt19937(device());
}

template <class chunkT, class keyT>
bool MarkovChain<chunkT, keyT>::CreateFrequencyTable(std::vector<chunkT> chunks)
{
	bool success = true;

	for (chunkT& chunk : chunks)
	{
		if (!AddToFrequencyTable(chunk))
		{
			success = false;
			break;
		}
	}

	return success;
}

template <class chunkT, class keyT>
bool MarkovChain<chunkT, keyT>::AddToFrequencyTable(chunkT chunk)
{
	int index = -_order;
	std::deque<keyT> keys;
	chunkT::iterator it = chunk.begin();

	while (it != chunk.end())
	{
		keyT key{};
		if (index >= 0)
		{
			key = *it;
			++it;
		}
		keys.push_back(key);

		if (keys.size() == _order + 1)
		{
			if (!AddKeys(keys))
			{
				return false;
			}
			keys.pop_front();
		}
		++index;	
	}
	return true;
}

template <class chunkT, class keyT>
bool MarkovChain<chunkT, keyT>::AddKeys(std::deque<keyT> keys)
{
	bool success = true;
	Chain<keyT>* current = &_chain;

	for (keyT& key : keys)
	{
		if (!current->Add(key))
		{
			if (current->IsLastLink())
			{
				if (!current->Update(key))
				{
					success = false;
					break;
				}
			}
		}

		current = current->WalkTheChain(key);
	}

	return success;
}

template <class chunkT, class keyT>
bool MarkovChain<chunkT, keyT>::GenerateNewChunk(int minLength, int maxLength, chunkT& outChunk)
{
	int MAX_TRIES = 50;
	outChunk = chunkT();
	std::deque<keyT> selectedKeys;

	for (int i = 0; i < _order; ++i)
	{
		selectedKeys.push_back(keyT());
	}

	int count = 0;
	while (count < MAX_TRIES)
	{
		while (true)
		{
			keyT key;
			if (!GetNextKey(selectedKeys, key))
			{
				if (outChunk.size() >= minLength)
				{
					return true;
				}

				outChunk = chunkT();
				selectedKeys.clear();
				for (int i = 0; i < _order; ++i)
				{
					selectedKeys.push_back(keyT());
				}
				break;
			}
			else
			{
				outChunk.push_back(key);
				if (outChunk.size() == maxLength)
				{
					return true;
				}

				selectedKeys.pop_front();
				selectedKeys.push_back(key);
			}
		}
		++count;
	}

	return false;
}

template <class chunkT, class keyT>
bool MarkovChain<chunkT, keyT>::GetNextKey(std::deque<keyT> keys, keyT& outKey)
{
	Chain<keyT>* current = &_chain;
	for (keyT& key : keys)
	{
		current = current->WalkTheChain(key);
	}

	map<keyT, int> distribution = current->Distribution();
	int total = 0;
	for (map<keyT, int>::value_type& value : distribution)
	{
		total += value.second;
	}

	if (total > 0)
	{
		std::uniform_int_distribution<int> uid(1, total);
		int metric = uid(_engine);

		total = 0;
		for (map<keyT, int>::value_type& value : distribution)
		{
			total += value.second;
			if (metric <= total)
			{
				outKey = value.first;
				return true;
			}
		}
	}
	return false;
}
