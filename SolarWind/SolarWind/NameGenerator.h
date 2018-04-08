#pragma once
#include <string>
#include "MarkovChain.h"

class NameGenerator
{
	public:
		NameGenerator();
		NameGenerator(std::string fileName, int order=1);
		void Init(int order);

		void LoadDataFromFile(std::string fileName);
		bool GenerateWord(int minLength, int maxLength, std::string& outString);

		bool IsNewName(std::string name);

		bool IsReady() { return _isReady; }

	private:
		bool _isReady = false;
		MarkovChain<std::string, char> _markovChain;
		std::vector<std::string> _nameHistory;
};
