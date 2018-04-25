#pragma once
#include <SFML\Graphics.hpp>
#include "FastNoise.h"
#include "SGC.h"

class Chunk
{
	public:
		Chunk();
		Chunk(SGC sgc, sf::Vector2f center, int width, int height, bool greyScale);

		void Init();
		void CreateChunk();

		void Update();
		void Draw(sf::RenderTarget* target);

		sf::Vector2f GetCenter() { return _center; }	
		int GetWidth() { return _width; }
		int GetHeight() { return _height; }	

		void SetSGC(SGC sgc) { _sgc = sgc; }

	private:
		sf::Color GenerateBiomeColor(sf::Uint8 noiseValue);
		sf::Uint8 Clamp(float rawNoise);

		SGC _sgc;
		int _width;
		int _height;
		sf::Vector2f _center;
		bool _isGreyScale;
		bool _isGenerated;

		FastNoise _heightNoise;		
		sf::Sprite _chunkSprite;
		sf::RenderTexture _chunk;

};