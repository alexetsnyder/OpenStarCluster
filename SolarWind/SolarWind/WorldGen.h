#pragma once
#include <SFML/Graphics.hpp>
#include "FastNoise.h"
#include "SGC.h"

class WorldGen
{
public:
	WorldGen();
	WorldGen(SGC sgc, int width = 1000, int height = 800);
	void Init(SGC sgc, int width = 1000, int height = 800);

	void ToggleGreyScale();
	void Generate();
	void GenerateGreyScale();
	void Draw(sf::RenderTarget* target);

	bool IsGenerated() { return _isGenerated; }
	sf::Uint8 Clamp(float noise);

	void SetSGC(SGC sgc) { _sgc = sgc; }
	void NewSeed(unsigned int seed) { _fastNoise.SetSeed(seed); }

private:
	sf::Color GenerateBiomeColor(float noiseValue);

	SGC _sgc;
	int _width;
	int _height;
	bool _isGenerated;
	bool _isGreyScale;
	FastNoise _fastNoise;
	sf::Sprite _greySpr;
	sf::Texture _greyGen;
	sf::Sprite _spriteGen;
	sf::RenderTexture _generated;
};