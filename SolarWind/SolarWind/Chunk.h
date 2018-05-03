#pragma once
#include <SFML\Graphics.hpp>
#include "FastNoise.h"
#include "SGC.h"

class Chunk
{
	public:
		Chunk();
		Chunk(SGC sgc, sf::Vector2f chunkCenter, sf::Vector2f noiseCenter, int width, int height, bool greyScale);

		void Init();
		void CreateChunk();

		void Update();
		void Draw(sf::RenderTarget* target);

		sf::Vector2f GetChunkCenter() { return _chunkCenter; }	
		sf::Vector2f GetNoiseCenter() { return _noiseCenter; }
		int GetChunkWidth() { return _chunkWidth; }
		int GetChunkHeight() { return _chunkHeight; }	

		void SetSGC(SGC sgc) { _sgc = sgc; }

	private:
		sf::Color GenerateBiomeColor(sf::Uint8 noiseValue);
		sf::Uint8 Clamp(float rawNoise);

		SGC _sgc;
		int _chunkWidth;
		int _chunkHeight;
		sf::Vector2f _chunkCenter;
		sf::Vector2f _noiseCenter;

		bool _isGreyScale;
		bool _isGenerated;

		FastNoise _heightNoise;		
		sf::Sprite _chunkSprite;
		sf::RenderTexture _chunk;

};