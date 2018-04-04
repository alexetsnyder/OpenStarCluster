#pragma once
#include <map>
#include <vector>
#include <deque>
#include <random>
#include <time.h>
#include "Chain.h"

template <class chunkT, class keyT>
class MarkovChain
{
	public:
		MarkovChain(int order=1);
		
		bool CreateFrequencyTable(std::vector<chunkT> chunks);
		bool GenerateNewChunk(int length, chunkT& outChunk);
		bool GetNextKey(std::deque<keyT> keys, keyT& outKey);

	private:
		bool AddToFrequencyTable(chunkT chunk);
		bool AddKeys(std::deque<keyT> keys);

		int _order;
		Chain<keyT> _chain;
		std::vector<keyT> _keys;
};

template <class chunkT, class keyT>
MarkovChain<chunkT, keyT>::MarkovChain(int order)
{
	std::srand(time(0));

	_order = order;
	_chain = Chain<keyT>(0, order);
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
bool MarkovChain<chunkT, keyT>::GenerateNewChunk(int length, chunkT& outChunk)
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
				break;
			}
			else
			{
				count = 0;
				outChunk.push_back(key);
				if (outChunk.size() == length)
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
		int metric = (rand() % total) + 1;

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
