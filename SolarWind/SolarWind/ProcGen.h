#pragma once
#include <SFML/Graphics.hpp>
#include "FastNoise.h"
#include "SGC.h"

class ProcGen
{
	public:
		ProcGen();
		ProcGen(SGC sgc, int width = 1000, int height = 800);
		void Init(SGC sgc, int width = 1000, int height = 800);

		void Generate();
		const sf::Texture& GetTexture() { return _generated; }

		bool IsGenerated() { return _isGenerated; }

		void SetSGC(SGC sgc) { _sgc = sgc; }
		void NewSeed(unsigned int seed) { _fastNoise.SetSeed(seed); }

	private:
		SGC _sgc;
		int _width;
		int _height;
		bool _isGenerated;
		sf::Texture _generated;
		FastNoise _fastNoise;
};