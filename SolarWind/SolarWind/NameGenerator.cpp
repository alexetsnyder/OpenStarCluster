#include "NameGenerator.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

NameGenerator::NameGenerator()
{
	
}

NameGenerator::NameGenerator(string fileName, int order)
{
	_markovChain.Init(order);
	LoadDataFromFile(fileName);
}

void NameGenerator::Init(int order)
{
	_markovChain.Init(order);
}

void NameGenerator::LoadDataFromFile(string fileName)
{
	vector<string> chunks;

	ifstream fin(fileName);
	if (fin.is_open())
	{
		string line;
		while (getline(fin, line))
		{
			int start = 0;
			int end; 
			while ((end = line.find(',', start)) != -1)
			{
				string word = line.substr(start, end-start);
				chunks.push_back(word);
				_nameHistory.push_back(word);
				start = end+1;
			}
			chunks.push_back(line.substr(start+1));
			_nameHistory.push_back(line.substr(start + 1));
		}
		if (_markovChain.CreateFrequencyTable(chunks))
		{
			_isReady = true;
		}

		fin.close();

	}
}

bool NameGenerator::GenerateWord(int minLength, int maxLength, string& outString)
{
	if (_markovChain.GenerateNewChunk(minLength, maxLength, outString))
		return true;
	return false;
}

bool NameGenerator::IsNewName(std::string name)
{
	for (string& hName : _nameHistory)
	{
		if (hName == name)
			return false;
	}
	return true;
}
