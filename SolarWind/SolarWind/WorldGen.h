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
	void LoadChunks(sf::Vector2f viewCenter, bool greyScale);
	void UpdateChunks(sf::Vector2f viewCenter, bool greyScale);

	void Generate(bool greyScale);
	void Draw(sf::RenderTarget* target);

	bool IsGenerated() { return _isGenerated; }

	void SetWorldSize(float width, float height) { _viewWidth = width; _viewHeight = height; }
	void SetSGC(SGC sgc) { _sgc = sgc; }

private:
	bool IsChunkWithinView(sf::Vector2f chunkCenter, int width, int height, sf::Vector2f viewCenter);
	bool IsViewCenterInChunk(Chunk* chunk, sf::Vector2f viewCenter);
	void FreeChunks();

	SGC _sgc;
	int _viewWidth;
	int _viewHeight;
	int _centerChunkIndex;

	bool _isGenerated;
	std::vector<Chunk*> _chunks;
};