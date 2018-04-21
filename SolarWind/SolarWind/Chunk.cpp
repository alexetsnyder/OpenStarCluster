#include "Chunk.h"
#include "Color.h"

Chunk::Chunk()
{

}

Chunk::Chunk(SGC sgc, sf::Vector2f center, int width, int height)
{
	_sgc = sgc;
	_center = center;
	_width = width;
	_height = height;

	_hasLeftNeighbor = false;
	_hasRightNeighbor = false;
	_hasTopNeighbor = false;
	_hasBottomNeighbor = false;

	Init();
}

void Chunk::Init()
{
	_heightNoise.SetSeed(_sgc.SEED);
	_heightNoise.SetNoiseType(FastNoise::SimplexFractal);
	_heightNoise.SetFractalOctaves(34);

	if (!_chunk.create(_width, _height))
	{
		printf("Could not create chunk render texture.");
	}
	_chunkSprite.setTexture(_chunk.getTexture());
	_chunkSprite.setPosition(_center.x - _width / 2, _center.y - _height / 2);

	CreateChunk();
}

bool Chunk::HasNeighbor(Neighbor which)
{
	bool hasNeighbor = false;

	switch (which)
	{
		case Neighbor::LEFT:
			hasNeighbor = _hasLeftNeighbor;
			break;
		case Neighbor::RIGHT:
			hasNeighbor = _hasRightNeighbor;
			break;
		case Neighbor::TOP:
			hasNeighbor = _hasTopNeighbor;
			break;
		case Neighbor::BOTTOM:
			hasNeighbor = _hasBottomNeighbor;
			break;
		case Neighbor::LEFTTOP:
			hasNeighbor = _hasLTNeighbor;
			break;
		case Neighbor::LEFTBOTTOM:
			hasNeighbor = _hasLBNeighbor;
			break;
		case Neighbor::RIGHTTOP:
			hasNeighbor = _hasRTNeighbor;
			break;
		case Neighbor::RIGHTBOTTOM:
			hasNeighbor = _hasRBNeighbor;
			break;
	}

	return hasNeighbor;
}

void Chunk::SetNeighbor(Neighbor which, bool value)
{
	switch (which)
	{
	case Neighbor::LEFT:
		_hasLeftNeighbor = value;
		break;
	case Neighbor::RIGHT:
		_hasRightNeighbor = value;
		break;
	case Neighbor::TOP:
		_hasTopNeighbor = value;
		break;
	case Neighbor::BOTTOM:
		_hasBottomNeighbor = value;
		break;
	case Neighbor::LEFTTOP:
		_hasLTNeighbor = value;
		break;
	case Neighbor::LEFTBOTTOM:
		_hasLBNeighbor = value;
		break;
	case Neighbor::RIGHTTOP:
		_hasRTNeighbor = value;
		break;
	case Neighbor::RIGHTBOTTOM:
		_hasRBNeighbor = value;
		break;
	}
}

void Chunk::CreateChunk()
{
	sf::RectangleShape gridSquare;

	_chunk.clear(sf::Color::White);
	int x = _center.x - _width / 2;
	int y = _center.y - _height / 2;
	for (int i = 0; i < _width / 4; ++i)
	{
		for (int j = 0; j < _height / 4; ++j)
		{
			sf::Uint8 height = Clamp((_heightNoise.GetNoise(x + 4 * i, y + 4 * j) + 1) / 2);
			gridSquare.setPosition(4 * i, 4 * j);
			gridSquare.setSize(sf::Vector2f(4.0f, 4.0f));
			gridSquare.setFillColor(GenerateBiomeColor(height));
			_chunk.draw(gridSquare);
		}
	}

	_chunk.display();
}

sf::Color Chunk::GenerateBiomeColor(sf::Uint8 noiseValue)
{
	sf::Color biomeColor;

	if (noiseValue <= 70.0f)
	{
		biomeColor = Color::ClearWaterBlue;
	}
	else if (noiseValue <= 80.0f)
	{
		biomeColor = Color::Sand;
	}
	else if (noiseValue <= 120.0f)
	{
		biomeColor = Color::Forest;
	}
	else if (noiseValue <= 160.0f)
	{
		biomeColor = Color::PineForest;
	}
	else
	{
		biomeColor = Color::Mountain;
	}

	return biomeColor;
}

sf::Uint8 Chunk::Clamp(float rawNoise)
{
	return (rawNoise >= 1.0f) ? 255 : (rawNoise <= 0.0f) ? 0 : static_cast<sf::Uint8>(rawNoise * 255.0f + 0.5f);
}

void Chunk::Update()
{

}

void Chunk::Draw(sf::RenderTarget* target)
{
	target->draw(_chunkSprite);
}