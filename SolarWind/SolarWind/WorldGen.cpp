#include "WorldGen.h"

bool operator< (const Pair& lhs, const Pair& rhs)
{
	return (lhs._pair.x < rhs._pair.x || (!(rhs._pair.x < lhs._pair.x) && lhs._pair.y < rhs._pair.y));
}

bool operator> (const Pair& lhs, const Pair& rhs)
{
	return rhs < lhs;
}

WorldGen::WorldGen()
{

}

WorldGen::WorldGen(SGC sgc, int width, int height)
{
	Init(sgc, width, height);
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

void WorldGen::Init(SGC sgc, int width, int height)
{
	_sgc = sgc;
	_viewWidth = width;
	_viewHeight = height;
	_isGenerated = false;
	_centerChunk = Pair(_viewWidth / 2, _viewHeight / 2);
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
	sf::Vector2f chunkCenter = _centerChunk.pair();
	float chunkWidth = _viewWidth;
	float chunkHeight = _viewHeight;

	_chunks.insert(std::make_pair(_centerChunk, new Chunk(_sgc, chunkCenter, _viewWidth, _viewHeight, greyScale)));

	//LEFT
	sf::Vector2f newChunkCenter(chunkCenter.x - chunkWidth, chunkCenter.y);
	_chunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale)));

	//RIGHT 
	newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y);
	_chunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale)));

	//TOP
	newChunkCenter = sf::Vector2f(chunkCenter.x, chunkCenter.y - chunkHeight);
	_chunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale)));

	//BOTTOM
	newChunkCenter = sf::Vector2f(chunkCenter.x, chunkCenter.y + chunkHeight);
	_chunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale)));

	//LEFTTOP
	newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y - chunkHeight);
	_chunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale)));

	//LEFTBOTTOM
	newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y + chunkHeight);
	_chunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale)));

	//RIGHTTOP
	newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y - chunkHeight);
	_chunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale)));

	//RIGHTBOTTOM
	newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y + chunkHeight);
	_chunks.insert(std::make_pair(newChunkCenter, new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale)));
}

void WorldGen::UpdateChunks(sf::Vector2f viewCenter, bool greyScale)
{
	if (!IsViewCenterInChunk(_chunks[_centerChunk], viewCenter))
	{
		for (std::map<Pair, Chunk*>::value_type& chunk : _chunks)
		{
			if (IsViewCenterInChunk(chunk.second, viewCenter))
			{
				_centerChunk = chunk.first;
				break;
			}
		}

		sf::Vector2f chunkCenter(_centerChunk.pair());
		float chunkWidth = _viewWidth;
		float chunkHeight = _viewHeight;
		
		sf::Vector2f leftChunkCenter(chunkCenter.x - chunkWidth, chunkCenter.y);
		if (!Contains(leftChunkCenter))
		{
			_chunks.insert(std::make_pair(leftChunkCenter, new Chunk(_sgc, leftChunkCenter, chunkWidth, chunkHeight, greyScale)));
		}
		
		sf::Vector2f rightChunkCenter(chunkCenter.x + chunkWidth, chunkCenter.y);
		if (!Contains(rightChunkCenter))
		{
			_chunks.insert(std::make_pair(rightChunkCenter, new Chunk(_sgc, rightChunkCenter, chunkWidth, chunkHeight, greyScale)));
		}

		sf::Vector2f topChunkCenter(chunkCenter.x, chunkCenter.y - chunkHeight);
		if (!Contains(topChunkCenter))
		{
			_chunks.insert(std::make_pair(topChunkCenter, new Chunk(_sgc, topChunkCenter, chunkWidth, chunkHeight, greyScale)));
		}

		sf::Vector2f bottomChunkCenter(chunkCenter.x, chunkCenter.y + chunkHeight);
		if (!Contains(bottomChunkCenter))
		{
			_chunks.insert(std::make_pair(bottomChunkCenter, new Chunk(_sgc, bottomChunkCenter, chunkWidth, chunkHeight, greyScale)));
		}

		sf::Vector2f leftTopChunkCenter(chunkCenter.x - chunkWidth, chunkCenter.y - chunkHeight);
		if (!Contains(leftTopChunkCenter))
		{
			_chunks.insert(std::make_pair(leftTopChunkCenter, new Chunk(_sgc, leftTopChunkCenter, chunkWidth, chunkHeight, greyScale)));
		}

		sf::Vector2f leftBottomChunkCenter(chunkCenter.x - chunkWidth, chunkCenter.y + chunkHeight);
		if (!Contains(leftBottomChunkCenter))
		{
			_chunks.insert(std::make_pair(leftBottomChunkCenter, new Chunk(_sgc, leftBottomChunkCenter, chunkWidth, chunkHeight, greyScale)));
		}

		sf::Vector2f rightTopChunkCenter(chunkCenter.x + chunkWidth, chunkCenter.y - chunkHeight);
		if (!Contains(rightTopChunkCenter))
		{
			_chunks.insert(std::make_pair(rightTopChunkCenter, new Chunk(_sgc, rightTopChunkCenter, chunkWidth, chunkHeight, greyScale)));
		}

		sf::Vector2f rightBottomChunkCenter(chunkCenter.x + chunkWidth, chunkCenter.y + chunkHeight);
		if (!Contains(rightBottomChunkCenter))
		{
			_chunks.insert(std::make_pair(rightBottomChunkCenter, new Chunk(_sgc, rightBottomChunkCenter, chunkWidth, chunkHeight, greyScale)));
		}

		UnloadChunks();
	}
}

bool WorldGen::UpdateCenterChunk(sf::Vector2f newViewCenter)
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

void WorldGen::UpdateChunksToStaging(bool greyScale)
{
	_staging.lock();
	sf::Vector2f chunkCenter(_centerChunk.pair());
	float chunkWidth = _viewWidth;
	float chunkHeight = _viewHeight;

	sf::Vector2f leftChunkCenter(chunkCenter.x - chunkWidth, chunkCenter.y);
	if (!Contains(leftChunkCenter))
	{
		_stagingChunks.insert(std::make_pair(leftChunkCenter, new Chunk(_sgc, leftChunkCenter, chunkWidth, chunkHeight, greyScale)));
	}

	sf::Vector2f rightChunkCenter(chunkCenter.x + chunkWidth, chunkCenter.y);
	if (!Contains(rightChunkCenter))
	{
		_stagingChunks.insert(std::make_pair(rightChunkCenter, new Chunk(_sgc, rightChunkCenter, chunkWidth, chunkHeight, greyScale)));
	}

	sf::Vector2f topChunkCenter(chunkCenter.x, chunkCenter.y - chunkHeight);
	if (!Contains(topChunkCenter))
	{
		_stagingChunks.insert(std::make_pair(topChunkCenter, new Chunk(_sgc, topChunkCenter, chunkWidth, chunkHeight, greyScale)));
	}

	sf::Vector2f bottomChunkCenter(chunkCenter.x, chunkCenter.y + chunkHeight);
	if (!Contains(bottomChunkCenter))
	{
		_stagingChunks.insert(std::make_pair(bottomChunkCenter, new Chunk(_sgc, bottomChunkCenter, chunkWidth, chunkHeight, greyScale)));
	}

	sf::Vector2f leftTopChunkCenter(chunkCenter.x - chunkWidth, chunkCenter.y - chunkHeight);
	if (!Contains(leftTopChunkCenter))
	{
		_stagingChunks.insert(std::make_pair(leftTopChunkCenter, new Chunk(_sgc, leftTopChunkCenter, chunkWidth, chunkHeight, greyScale)));
	}

	sf::Vector2f leftBottomChunkCenter(chunkCenter.x - chunkWidth, chunkCenter.y + chunkHeight);
	if (!Contains(leftBottomChunkCenter))
	{
		_stagingChunks.insert(std::make_pair(leftBottomChunkCenter, new Chunk(_sgc, leftBottomChunkCenter, chunkWidth, chunkHeight, greyScale)));
	}

	sf::Vector2f rightTopChunkCenter(chunkCenter.x + chunkWidth, chunkCenter.y - chunkHeight);
	if (!Contains(rightTopChunkCenter))
	{
		_stagingChunks.insert(std::make_pair(rightTopChunkCenter, new Chunk(_sgc, rightTopChunkCenter, chunkWidth, chunkHeight, greyScale)));
	}

	sf::Vector2f rightBottomChunkCenter(chunkCenter.x + chunkWidth, chunkCenter.y + chunkHeight);
	if (!Contains(rightBottomChunkCenter))
	{
		_stagingChunks.insert(std::make_pair(rightBottomChunkCenter, new Chunk(_sgc, rightBottomChunkCenter, chunkWidth, chunkHeight, greyScale)));
	}
	_staging.unlock();
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
		if (!IsChunkNextToCenter(chunk.second))
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
	float deltaX = chunk->GetCenter().x - viewCenter.x;
	float deltaY = chunk->GetCenter().y - viewCenter.y;
	deltaX = (deltaX < 0) ? -1 * deltaX : deltaX;
	deltaY = (deltaY < 0) ? -1 * deltaY : deltaY;

	return (deltaX <= chunk->GetWidth() && deltaY <= chunk->GetHeight());

}

bool WorldGen::IsChunkNextToCenter(Chunk* chunk)
{
	sf::Vector2f chunkCenter = _centerChunk.pair();
	float deltaX = chunk->GetCenter().x - chunkCenter.x;
	float deltaY = chunk->GetCenter().y - chunkCenter.y;
	deltaX = (deltaX < 0) ? -1 * deltaX : deltaX;
	deltaY = (deltaY < 0) ? -1 * deltaY : deltaY;

	return (deltaX <= _viewWidth && deltaY <= _viewHeight);
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
