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
	for (Chunk* chunk : _chunks)
	{
		delete chunk;
	}
}

void WorldGen::Init(SGC sgc, int width, int height)
{
	_sgc = sgc;
	_viewWidth = width;
	_viewHeight = height;
	_isGreyScale = false;
	_centerChunkIndex = 0;

	/*_heightNoise.SetSeed(_sgc.SEED);
	_heightNoise.SetNoiseType(FastNoise::SimplexFractal);
	_heightNoise.SetFractalOctaves(34);

	_world.create(_width, _height);
	_worldSprite.setTexture(_world.getTexture());*/
}

void WorldGen::ToggleGreyScale()
{
	_isGreyScale = !_isGreyScale;

	if (_isGreyScale)
	{
		GenerateGreyScale();
	}
	else
	{
		//Generate();
	}
}

void WorldGen::Generate()
{
	LoadChunks(sf::Vector2f(_viewWidth / 2, _viewHeight / 2));
	/*sf::RectangleShape gridSquare;

	_world.clear(sf::Color::White);
	for (int i = viewCenter.x - _width / 2; i < viewCenter.x + _width * 3 / 4; ++i)
	{
		for (int j = viewCenter.y - _height / 2; j < viewCenter.y + _height * 3 / 4; ++j)
		{
			sf::Uint8 height = Clamp((_heightNoise.GetNoise(i, j) + 1) / 2);
			gridSquare.setPosition(4 * i, 4 * j);
			gridSquare.setSize(sf::Vector2f(4.0f, 4.0f));
			gridSquare.setFillColor(GenerateBiomeColor(height));
			_world.draw(gridSquare);
		}
	}

	_world.display();
	_worldSprite.setPosition(viewCenter.x - _width / 2, viewCenter.y - _height / 2);*/
}

void WorldGen::GenerateGreyScale()
{
	/*sf::Uint8* pixels = new sf::Uint8[_width * _height * 4];

	for (int i = 0; i < _width; ++i)
	{
		for (int j = 0; j < _height; ++j)
		{
			sf::Uint8 color = Clamp((_fastNoise.GetNoise(i, j) + 1) / 2);
			int index = 4 * (i * _height + j);
			pixels[index] = color;
			pixels[index + 1] = color;
			pixels[index + 2] = color;
			pixels[index + 3] = 255;
		}
	}

	_greyGen.update(pixels);
	delete[] pixels;

	sf::RectangleShape gridSquare;

	_generated.clear(sf::Color::White);
	for (int i = 0; i < _width / 4; ++i)
	{
		for (int j = 0; j < _height / 4; ++j)
		{
			sf::Uint8 color = Clamp((_fastNoise.GetNoise(i, j) + 1) / 2);
			gridSquare.setPosition(4 * i, 4 * j);
			gridSquare.setSize(sf::Vector2f(4.0f, 4.0f));
			gridSquare.setFillColor(sf::Color(color, color, color, 0xFF));;
			_generated.draw(gridSquare);
		}
	}

	_generated.display();

	_isGenerated = true;*/
}

void WorldGen::UpdateChunks(sf::Vector2f viewCenter)
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
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
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
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
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
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
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
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
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
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
			_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::LEFTBOTTOM))
		{
			printf("Left Bottom Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y + chunkHeight);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::TOP, true);
			_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::RIGHTTOP))
		{
			printf("Right Top Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y - chunkHeight);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
			_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);
		}
		if (!_chunks[_centerChunkIndex]->HasNeighbor(Neighbor::RIGHTBOTTOM))
		{
			printf("Right Bottom Chunk Added.");
			newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y + chunkHeight);
			_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
			_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
			_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
			_chunks.back()->SetNeighbor(Neighbor::TOP, true);
		}
	}
}

void WorldGen::UnloadChunks()
{
	for (Chunk* chunk : _chunks)
	{

	}
}

void WorldGen::LoadChunks(sf::Vector2f viewCenter)
{
	_chunks.push_back(new Chunk(_sgc, sf::Vector2f(viewCenter.x, viewCenter.y), _viewWidth, _viewHeight));

	sf::Vector2f chunkCenter(_chunks[_centerChunkIndex]->GetCenter());
	float chunkWidth = _chunks[_centerChunkIndex]->GetWidth();
	float chunkHeight = _chunks[_centerChunkIndex]->GetHeight();

	//LEFT
	sf::Vector2f newChunkCenter(chunkCenter.x - chunkWidth, chunkCenter.y);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);

	//RIGHT 
	newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);

	//TOP
	newChunkCenter = sf::Vector2f(chunkCenter.x, chunkCenter.y - chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);

	//BOTTOM
	newChunkCenter = sf::Vector2f(chunkCenter.x, chunkCenter.y + chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::BOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);

	//LEFTTOP
	newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y - chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);

	//LEFTBOTTOM
	newChunkCenter = sf::Vector2f(chunkCenter.x - chunkWidth, chunkCenter.y + chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::LEFTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);
	_chunks.back()->SetNeighbor(Neighbor::RIGHT, true);

	//RIGHTTOP
	newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y - chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::BOTTOM, true);

	//RIGHTBOTTOM
	newChunkCenter = sf::Vector2f(chunkCenter.x + chunkWidth, chunkCenter.y + chunkHeight);
	_chunks.push_back(new Chunk(_sgc, newChunkCenter, chunkWidth, chunkHeight));
	_chunks[_centerChunkIndex]->SetNeighbor(Neighbor::RIGHTBOTTOM, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFTTOP, true);
	_chunks.back()->SetNeighbor(Neighbor::LEFT, true);
	_chunks.back()->SetNeighbor(Neighbor::TOP, true);

}

bool WorldGen::IsViewCenterInChunk(Chunk* chunk, sf::Vector2f viewCenter)
{
	float deltaX = chunk->GetCenter().x - viewCenter.x;
	float deltaY = chunk->GetCenter().y - viewCenter.y;
	deltaX = (deltaX < 0) ? -1 * deltaX : deltaX;
	deltaY = (deltaY < 0) ? -1 * deltaY : deltaY;

	return (deltaX <= chunk->GetWidth() && deltaY <= chunk->GetHeight());

}

bool WorldGen::IsWithinView(sf::Vector2f point, int width, int height, sf::Vector2f viewCenter)
{
	float deltaX = point.x - viewCenter.x;
	float deltaY = point.y - viewCenter.y;

	deltaX = (deltaX < 0) ? -1 * deltaX : deltaX;
	deltaY = (deltaY < 0) ? -1 * deltaY : deltaY;

	return (deltaX <= _viewWidth / 2 && deltaY  <= _viewHeight / 2);
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
	//UnloadChunks();
	//LoadChunks(viewCenter);
}

void WorldGen::Draw(sf::RenderTarget* target)
{
	for (Chunk* chunk : _chunks)
	{
		chunk->Draw(target);
	}
	//target->draw(_worldSprite);
}
