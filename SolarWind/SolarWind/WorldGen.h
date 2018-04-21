#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "FastNoise.h"
#include "SGC.h"
#include "Chunk.h"

class WorldGen
{
public:
	WorldGen();
	WorldGen(SGC sgc, int width = 1000, int height = 800);
	~WorldGen();
	void Init(SGC sgc, int width = 1000, int height = 800);

	void Update(sf::Vector2f viewCenter);
	void UnloadChunks();
	void LoadChunks(sf::Vector2f viewCenter);

	void ToggleGreyScale();
	void Generate();
	void GenerateGreyScale();
	void Draw(sf::RenderTarget* target);

	bool IsGenerated() { return _isGenerated; }

	void SetWorldSize(float width, float height) { _width = width; _height = height; }
	//void SetPosition(sf::Vector2f position) { _position = position; }
	void SetSGC(SGC sgc) { _sgc = sgc; }
	//void NewSeed(unsigned int seed) { _fastNoise.SetSeed(seed); }

private:
	bool IsChunkWithinView(Chunk chunk, sf::Vector2f viewCenter);
	bool IsWithinView(sf::Vector2f point, int width, int height, sf::Vector2f viewCenter);

	SGC _sgc;
	int _width;
	int _height;

	bool _isGenerated;
	bool _isGreyScale;

	/*FastNoise _heightNoise;
	sf::RenderTexture _world;
	sf::Sprite _worldSprite;*/

	std::vector<Chunk*> _chunks;
};