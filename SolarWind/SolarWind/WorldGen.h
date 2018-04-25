#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <mutex>
#include "FastNoise.h"
#include "SGC.h"
#include "Chunk.h"

class Pair
{
	public:
		Pair() {}
		Pair(float x, float y) { _pair.x = x; _pair.y = y; }
		Pair(sf::Vector2f pair) { _pair = pair; }

		sf::Vector2f pair() { return _pair; }
		const sf::Vector2f constPair() { return _pair; }

		friend bool operator< (const Pair& lhs, const Pair& rhs);
		friend bool operator> (const Pair& lhs, const Pair& rhs);

	private:
		sf::Vector2f _pair;
};

class WorldGen
{
	public:
		WorldGen();
		WorldGen(SGC sgc, int width = 1000, int height = 800);
		~WorldGen();
		void Init(SGC sgc, int width = 1000, int height = 800);

		void Update(sf::Vector2f viewCenter);	
		void UpdateChunks(sf::Vector2f viewCenter, bool greyScale);
		void UnloadChunks();

		bool UpdateCenterChunk(sf::Vector2f newViewCenter);
		void UpdateChunksToStaging(bool greyScale);
		void MergeStagingToChunks();

		void Generate(bool greyScale);
		void Draw(sf::RenderTarget* target);

		bool IsGenerated() { return _isGenerated; }

		void SetWorldSize(float width, float height) { _viewWidth = width; _viewHeight = height; }
		void SetSGC(SGC sgc) { _sgc = sgc; }

	private:
		void LoadChunks(bool greyScale);
		bool IsViewCenterInChunk(Chunk* chunk, sf::Vector2f viewCenter);
		bool IsChunkNextToCenter(Chunk* chunk);
		bool Contains(Pair key);
		void FreeChunks();

		SGC _sgc;
		int _viewWidth;
		int _viewHeight;

		bool _isGenerated;
		Pair _centerChunk;
		std::mutex _staging;
		std::map<Pair, Chunk*> _chunks;
		std::map<Pair, Chunk*> _stagingChunks;	
};