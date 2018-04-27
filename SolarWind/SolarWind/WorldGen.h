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

		sf::Vector2f GetVector2f() { return _pair; }

		friend bool operator< (const Pair& lhs, const Pair& rhs);
		friend bool operator> (const Pair& lhs, const Pair& rhs);

	private:
		sf::Vector2f _pair;
};

class WorldGen
{
	public:
		WorldGen();
		WorldGen(SGC sgc, sf::Vector2f origin, int chunkCount = 1, int width = 400, int height = 400);
		~WorldGen();

		void Init(SGC sgc, sf::Vector2f origin, int chunkCount = 1, int width = 400, int height = 400);

		void Update(sf::Vector2f viewCenter);	
		void Draw(sf::RenderTarget* target);

		void Generate(bool greyScale);
		void LoadChunks(bool greyScale);
		void UnloadChunks();

		bool IsNewChunkInView(sf::Vector2f newViewCenter);
		void LoadChunksToStaging(bool greyScale);
		void MergeStagingToChunks();

		bool IsGenerated() { return _isGenerated; }

		void SetWorldSize(float width, float height) { _chunkWidth = width; _chunkHeight = height; }
		void SetSGC(SGC sgc) { _sgc = sgc; }
		void SetChunkCount(int value) { _chunkCount = value; }
		

	private:
		bool IsViewCenterInChunk(Chunk* chunk, sf::Vector2f viewCenter);
		bool IsChunkWithinDistanceToCenter(Chunk* chunk);
		bool Contains(Pair key);
		void FreeChunks();

		SGC _sgc;
		int _chunkWidth;
		int _chunkHeight;
		sf::Vector2f _origin;
		int _chunkCount;

		bool _isGenerated;
		Pair _centerChunk;
		std::mutex _staging;
		std::map<Pair, Chunk*> _chunks;
		std::map<Pair, Chunk*> _stagingChunks;	
};