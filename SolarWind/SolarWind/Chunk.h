#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "FastNoise.h"
#include "SGC.h"

class Chunk
{
	public:
		Chunk();
		Chunk(SGC sgc, sf::Vector2f chunkCenter, sf::Vector2f noiseCenter, 
			  int width, int height, std::vector<unsigned int> seeds, bool greyScale);
		void Init(SGC sgc, sf::Vector2f chunkCenter, sf::Vector2f noiseCenter,
			int width, int height, std::vector<unsigned int> seeds, bool greyScale);

		void CreateChunk();
		void GenerateChunk();
		void GenerateHeightMap();
		void GenerateTemperatureMap();

		void Update();
		void Draw(sf::RenderTarget* target);

		sf::Vector2f GetChunkCenter() { return _chunkCenter; }	
		sf::Vector2f GetNoiseCenter() { return _noiseCenter; }
		int GetChunkWidth() { return _chunkWidth; }
		int GetChunkHeight() { return _chunkHeight; }	

		void SetSGC(SGC sgc) { _sgc = sgc; }

	private:
		sf::Color GenerateBiomeColor(int i, int j);
		sf::Uint8 Clamp(float rawNoise);

		SGC _sgc;

		int _chunkWidth;
		int _chunkHeight;
		sf::Vector2f _chunkCenter;
		sf::Vector2f _noiseCenter;

		unsigned int _heightSeed;
		unsigned int _temperatureSeed;
		std::vector<std::vector<sf::Uint8> > _heightMap;
		std::vector<std::vector<sf::Uint8> > _temperatureMap;

		FastNoise _heightNoise;	
		FastNoise _temperatureNoise;

		bool _isGreyScale;
		bool _isGenerated;
		sf::Sprite _chunkSprite;
		sf::RenderTexture _chunk;

};