#pragma once
#include <SFML\Graphics.hpp>
#include "FastNoise.h"
#include "SGC.h"

class Chunk
{
	public:
		Chunk();
		Chunk(SGC sgc, sf::Vector2f center, int width, int height);

		void Init();
		void CreateChunk();

		void Update();
		void Draw(sf::RenderTarget* target);

		sf::Vector2f GetCenter() { return _center; }	
		int GetWidth() { return _width; }
		int GetHeight() { return _height; }	

		void SetSGC(SGC sgc) { _sgc = sgc; }

		bool HasLeftN() { return _hasLeftNeighbor; }
		bool HasRightN() { return _hasRightNeighbor; }
		bool HasTopN() { return _hasTopNeighbor; }
		bool HasBottomN() { return _hasBottomNeighbor; }
		bool HasLTN() { return _hasLTNeighbor; }
		bool HasLBN() { return _hasLBNeighbor; }
		bool HasRTN() { return _hasRTNeighbor; }
		bool HasRBN() { return _hasRBNeighbor; }

		void SetLeftN(bool value) { _hasLeftNeighbor = value; }
		void SetRightN(bool value) { _hasRightNeighbor = value; }
		void SetTopN(bool value) { _hasTopNeighbor = value; }
		void SetBottomN(bool value) { _hasBottomNeighbor = value; }


	private:
		sf::Color GenerateBiomeColor(sf::Uint8 noiseValue);
		sf::Uint8 Clamp(float rawNoise);

		SGC _sgc;
		int _width;
		int _height;
		sf::Vector2f _center;

		bool _hasLeftNeighbor;
		bool _hasRightNeighbor;
		bool _hasTopNeighbor;
		bool _hasBottomNeighbor;
		bool _hasLTNeighbor;
		bool _hasLBNeighbor;
		bool _hasRTNeighbor;
		bool _hasRBNeighbor;


		FastNoise _heightNoise;
		
		sf::Sprite _chunkSprite;
		sf::RenderTexture _chunk;

};