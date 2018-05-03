#include "WorldGen.h"

//Pair Vector2f Wrapper Class
bool operator< (const Pair& lhs, const Pair& rhs)
{
	return (lhs._pair.x < rhs._pair.x || (!(rhs._pair.x < lhs._pair.x) && lhs._pair.y < rhs._pair.y));
}

bool operator> (const Pair& lhs, const Pair& rhs)
{
	return rhs < lhs;
}

//WorldGen Class
WorldGen::WorldGen()
{

}

WorldGen::WorldGen(SGC sgc, sf::Vector2f origin, int chunkCount, int width, int height)
{
	Init(sgc, origin, chunkCount, width, height);
}

WorldGen::~WorldGen()
{
	FreeChunks();
}

void WorldGen::FreeChunks()
{
	for (std::map<Pair, Chunk*>::value_type& chunk : _chunks)
	{
		delete chunk.second;
	}
	_chunks.clear();
}

void WorldGen::Init(SGC sgc, sf::Vector2f origin, int chunkCount, int width, int height)
{
	_sgc = sgc;
	_chunkWidth = width;
	_chunkHeight = height;
	_origin = origin;
	_isGenerated = false;
	_chunkCount = chunkCount;
	_centerChunk = Pair(origin);
}

void WorldGen::Generate(bool greyScale)
{
	if (!_chunks.empty())
	{
		FreeChunks();
	}

	LoadChunks(greyScale);
	_isGenerated = true;
}

void WorldGen::LoadChunks(bool greyScale)
{
	sf::Vector2f chunkCenter = _centerChunk.GetVector2f();
	float chunkWidth = _chunkWidth;
	float chunkHeight = _chunkHeight;
	int count = 0;

	float noiseX = chunkCenter.x - _chunkCount * _chunkWidth / 4;
	for (float i = chunkCenter.x - _chunkCount * chunkWidth; i <= chunkCenter.x + _chunkCount * chunkWidth; i += chunkWidth)
	{
		float noiseY = chunkCenter.y - _chunkCount * _chunkHeight / 4;
		for (float j = chunkCenter.y - _chunkCount * chunkHeight; j <= chunkCenter.y + _chunkCount * chunkHeight; j += chunkHeight)
		{
			sf::Vector2f newChunkCenter(i, j);
			sf::Vector2f newNoiseCenter(noiseX, noiseY);
			if (!Contains(newChunkCenter))
			{
				_chunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, newNoiseCenter, chunkWidth, chunkHeight, greyScale)));
				++count;
			}
			noiseY += chunkHeight / 4;
		}
		noiseX += chunkWidth / 4;
	}

	printf("Total number of Chunks: %d\n", count);
}

bool WorldGen::IsNewChunkInView(sf::Vector2f newViewCenter)
{
	bool updated = false;

	if (!IsViewCenterInChunk(_chunks[_centerChunk], newViewCenter))
	{
		for (std::map<Pair, Chunk*>::value_type& chunk : _chunks)
		{
			if (IsViewCenterInChunk(chunk.second, newViewCenter))
			{
				_centerChunk = chunk.first;
				updated = true;
				break;
			}
		}
	}

	return updated;
}

void WorldGen::LoadChunksToStaging(bool greyScale)
{
	_staging.lock();
	sf::Vector2f chunkCenter = _centerChunk.GetVector2f();
	Chunk* centerChunk = _chunks[chunkCenter]; 
	float chunkWidth = _chunkWidth;
	float chunkHeight = _chunkHeight;
	int count = 0;

	float noiseX = centerChunk->GetNoiseCenter().x - _chunkCount * _chunkWidth / 4;
	for (float i = chunkCenter.x - _chunkCount * chunkWidth; i <= chunkCenter.x + _chunkCount * chunkWidth; i += chunkWidth)
	{
		float noiseY = centerChunk->GetNoiseCenter().y - _chunkCount * _chunkHeight / 4;
		for (float j = chunkCenter.y - _chunkCount * chunkHeight; j <= chunkCenter.y + _chunkCount * chunkHeight; j += chunkHeight)
		{
			sf::Vector2f newChunkCenter(i, j);
			sf::Vector2f newNoiseCenter(noiseX, noiseY);
			if (!Contains(newChunkCenter))
			{
				_stagingChunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, newNoiseCenter, chunkWidth, chunkHeight, greyScale)));
				++count;
			}
			noiseY += chunkHeight / 4;
		}
		noiseX += chunkWidth / 4;
	}
	_staging.unlock();

	centerChunk = nullptr;
	printf("Total number of Chunks To Staging: %d\n", count);
}

void WorldGen::MergeStagingToChunks()
{
	_staging.lock();
	for (std::map<Pair, Chunk*>::value_type chunk : _stagingChunks)
	{
		_chunks.insert(chunk);
		chunk.second = nullptr;
	}

	_stagingChunks.clear();
	_staging.unlock();
}

void WorldGen::UnloadChunks()
{
	std::vector<Pair> markedForDelete;
	for (std::map<Pair, Chunk*>::value_type& chunk : _chunks)
	{
		if (!IsChunkWithinDistanceToCenter(chunk.second))
		{
			markedForDelete.push_back(chunk.first);
			delete chunk.second;
		}
	}

	for (Pair& chunkCenter : markedForDelete)
	{
		_chunks.erase(chunkCenter);
	}
}

bool WorldGen::Contains(Pair key)
{
	return (_chunks.count(key) == 1);
}

bool WorldGen::IsViewCenterInChunk(Chunk* chunk, sf::Vector2f viewCenter)
{
	float deltaX = chunk->GetChunkCenter().x - viewCenter.x;
	float deltaY = chunk->GetChunkCenter().y - viewCenter.y;
	deltaX = (deltaX < 0) ? -1 * deltaX : deltaX;
	deltaY = (deltaY < 0) ? -1 * deltaY : deltaY;

	return (deltaX <= chunk->GetChunkWidth() && deltaY <= chunk->GetChunkHeight());

}

bool WorldGen::IsChunkWithinDistanceToCenter(Chunk* chunk)
{
	sf::Vector2f chunkCenter = _centerChunk.GetVector2f();
	float deltaX = chunk->GetChunkCenter().x - chunkCenter.x;
	float deltaY = chunk->GetChunkCenter().y - chunkCenter.y;
	deltaX = (deltaX < 0) ? -1 * deltaX : deltaX;
	deltaY = (deltaY < 0) ? -1 * deltaY : deltaY;

	return (deltaX <= _chunkCount * _chunkWidth && deltaY <= _chunkCount * _chunkHeight);
}

void WorldGen::Update(sf::Vector2f viewCenter)
{
	
}

void WorldGen::Draw(sf::RenderTarget* target)
{
	for (std::map<Pair, Chunk*>::value_type& chunk : _chunks)
	{
		chunk.second->Draw(target);
	}
}
