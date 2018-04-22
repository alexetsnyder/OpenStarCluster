#include "WorldGen.h"

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
	for (Chunk* chunk : _chunks)
	{
		delete chunk;
	}
	_chunks.clear();
}

void WorldGen::Init(SGC sgc, int width, int height)
{
	_sgc = sgc;
	_viewWidth = width;
	_viewHeight = height;
	_centerChunkIndex = 0;
	_isGenerated = false;
}

void WorldGen::Generate(bool greyScale)
{
	if (!_chunks.empty())
	{
		FreeChunks();
	}

	LoadChunks(sf::Vector2f(_viewWidth / 2, _viewHeight / 2), greyScale);
	_isGenerated = true;
}

void WorldGen::UpdateChunks(sf::Vector2f viewCenter, bool greyScale)
{
	if (!IsViewCenterInChunk(_chunks[_centerChunkIndex], viewCenter))
	{
		printf("Old Chunk Index: %d Center: (%f, %f)\n", _centerChunkIndex, _chunks[_centerChunkIndex]->GetCenter().x, _chunks[_centerChunkIndex]->GetCenter().y);
		for (int i = 0; i < _chunks.size(); ++i)
		{
			if (IsViewCenterInChunk(_chunks[i], viewCenter))
			{
				_centerChunkIndex = i;
				break;
			}
		}

		printf("New Chunk Index: %d Center: (%f, %f)\n", _centerChunkIndex, _chunks[_centerChunkIndex]->GetCenter().x, _chunks[_centerChunkIndex]->GetCenter().y);

		sf::Vector2f chunkCenter(_chunks[_centerChunkIndex]->GetCenter());
		float chunkWidth = _chunks[_centerChunkIndex]->GetWidth();
		float chunkHeight = _chunks[_centerChunkIndex]->GetHeight();

		sf::Vector2f newChunkCenter;
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::LEFT))
		{
			printf("Left Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFT, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::TOP, true);
			_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::RIGHT))
		{
			printf("Right Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHT, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::TOP, true);
			_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::TOP))
		{
			printf("Top Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x, chunkCenter.y - chunkHeight);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::TOP, true);
			_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::BOTTOM))
		{
			printf("Bottom Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x, chunkCenter.y + chunkHeight);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::BOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::TOP, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::LEFTTOP))
		{
			printf("Left Top Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y - chunkHeight);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
			_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::LEFTBOTTOM))
		{
			printf("Left Bottom Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y + chunkHeight);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::TOP, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::RIGHTTOP))
		{
			printf("Right Top Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y - chunkHeight);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
			_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::RIGHTBOTTOM))
		{
			printf("Right Bottom Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y + chunkHeight);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
			_chunks.back()->SetNeighbor(Neighbor::TOP, true);
		}
	}
}

void WorldGen::LoadChunks(sf::Vector2f viewCenter, bool greyScale)
{
	_chunks.push_back(new Chunk(_sgc, sf::Vector2f(viewCenter.x, viewCenter.y), _viewWidth, _viewHeight, greyScale));

	sf::Vector2f chunkCenter(_chunks[_centerChunkIndex]->GetCenter());
	float chunkWidth = _chunks[_centerChunkIndex]->GetWidth();
	float chunkHeight = _chunks[_centerChunkIndex]->GetHeight();

	//LEFT
	sf::Vector2f newChunkCenter(chunkCenter.x - chunkWidth, chunkCenter.y);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);

	//RIGHT 
	newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);

	//TOP
	newChunkCenter = sf::Vector2f(chunkCenter.x, chunkCenter.y - chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);

	//BOTTOM
	newChunkCenter = sf::Vector2f(chunkCenter.x, chunkCenter.y + chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::BOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);

	//LEFTTOP
	newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y - chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);

	//LEFTBOTTOM
	newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y + chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);

	//RIGHTTOP
	newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y - chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);

	//RIGHTBOTTOM
	newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y + chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight, greyScale));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);

}

void WorldGen::UnloadChunks()
{
	for (Chunk* chunk : _chunks)
	{

	}
}

bool WorldGen::IsViewCenterInChunk(Chunk* chunk, sf::Vector2f viewCenter)
{
	float deltaX = chunk->GetCenter().x - viewCenter.x;
	float deltaY = chunk->GetCenter().y - viewCenter.y;
	deltaX = (deltaX < 0) ? -1 * deltaX : deltaX;
	deltaY = (deltaY < 0) ? -1 * deltaY : deltaY;

	return (deltaX <= chunk->GetWidth() && deltaY <= chunk->GetHeight());

}

bool WorldGen::IsChunkWithinView(sf::Vector2f chunkCenter, int width, int height, sf::Vector2f viewCenter)
{
	float deltaX = chunkCenter.x - viewCenter.x;
	float deltaY = chunkCenter.y - viewCenter.y;
	float totalWidth = 0.5f * (width + _viewWidth);
	float totalHeight = 0.5f * (height + _viewHeight);

	deltaX = (deltaX < 0) ? -1 * deltaX : deltaX;
	deltaY = (deltaY < 0) ? -1 * deltaY : deltaY;

	return (deltaX <= totalWidth && deltaY <= totalHeight);
}

void WorldGen::Update(sf::Vector2f viewCenter)
{
	
}

void WorldGen::Draw(sf::RenderTarget* target)
{
	for (Chunk* chunk : _chunks)
	{
		chunk->Draw(target);
	}
}
