#pragma once
#include <string>
#include "MarkovChain.h"

class NameGenerator
{
	public:
		NameGenerator(int order=1);
		NameGenerator(std::string fileName, int order=1);

		void LoadDataFromFile(std::string fileName);
		bool GenerateWord(int length, std::string& outString);

		bool IsNewName(std::string name);

	private:
		MarkovChain<std::string, char> _markovChain;
		std::vector<std::string> _nameHistory;
};
