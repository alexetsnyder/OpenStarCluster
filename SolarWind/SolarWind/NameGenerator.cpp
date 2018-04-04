#include "NameGenerator.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

NameGenerator::NameGenerator(int order)
{
	_markovChain = MarkovChain<string, char>(order);
}

NameGenerator::NameGenerator(string fileName, int order)
{
	_markovChain = MarkovChain<string, char>(order);
	LoadDataFromFile(fileName);
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
			string word = line.substr(0, line.find(" "));
			chunks.push_back(word);
			_nameHistory.push_back(word);
		}
		_markovChain.CreateFrequencyTable(chunks);

		fin.close();
	}
}

bool NameGenerator::GenerateWord(int length, string& outString)
{
	if (_markovChain.GenerateNewChunk(length, outString))
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
