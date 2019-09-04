#include "Chunk.h"
#include "Color.h"

Chunk::Chunk()
{

}

Chunk::Chunk(SGC sgc, sf::Vector2f chunkCenter, sf::Vector2f noiseCenter,
	         int width, int height, std::vector<unsigned int> seeds, bool greyScale)
{
	Init(sgc, chunkCenter, noiseCenter, width, height, seeds, greyScale);
}

void Chunk::Init(SGC sgc, sf::Vector2f chunkCenter, sf::Vector2f noiseCenter,
	             int width, int height, std::vector<unsigned int> seeds, bool greyScale)
{
	_sgc = sgc;
	_chunkCenter = chunkCenter;
	_noiseCenter = noiseCenter;
	_chunkWidth = width;
	_chunkHeight = height;
	_isGreyScale = greyScale;
	_isGenerated = false;

	_heightNoise.SetSeed(seeds[0]);
	_heightNoise.SetNoiseType(FastNoise::SimplexFractal);
	_heightNoise.SetFractalOctaves(34);

	_temperatureNoise.SetSeed(seeds[1]);
	_temperatureNoise.SetNoiseType(FastNoise::SimplexFractal);
	_temperatureNoise.SetFractalOctaves(34);

	if (!_chunk.create(_chunkWidth, _chunkHeight))
	{
		printf("Could not create chunk render texture.");
	}
	_chunkSprite.setTexture(_chunk.getTexture());
	_chunkSprite.setPosition(_chunkCenter.x - _chunkWidth / 2, _chunkCenter.y - _chunkHeight / 2);
}

void Chunk::GenerateHeightMap()
{	
	int x = _noiseCenter.x - _chunkWidth / 4;
	int y = _noiseCenter.y - _chunkHeight / 4;
	for (int i = 0; i < _chunkWidth / 4; ++i)
	{
		_heightMap.push_back(std::vector<sf::Uint8>());
		for (int j = 0; j < _chunkHeight / 4; ++j)
		{
			sf::Uint8 height = Clamp((_heightNoise.GetNoise(x + i, y + j) + 1) / 2);
			_heightMap[i].push_back(height);
		}
	}
}

void Chunk::GenerateTemperatureMap()
{
	int x = _noiseCenter.x - _chunkWidth / 4;
	int y = _noiseCenter.y - _chunkHeight / 4;
	for (int i = 0; i < _chunkWidth / 4; ++i)
	{
		_temperatureMap.push_back(std::vector<sf::Uint8>());
		for (int j = 0; j < _chunkHeight / 4; ++j)
		{
			sf::Uint8 height = Clamp((_temperatureNoise.GetNoise(x + i, y + j) + 1) / 2);
			_temperatureMap[i].push_back(height);
		}
	}
}

void Chunk::GenerateChunk()
{
	GenerateHeightMap();
	GenerateTemperatureMap();

	sf::RectangleShape gridSquare;
	_chunk.clear(sf::Color::White);

	for (int i = 0; i < _heightMap.size(); ++i)
	{
		for (int j = 0; j < _heightMap[i].size(); ++j)
		{
			gridSquare.setPosition(4 * i, 4 * j);
			gridSquare.setSize(sf::Vector2f(4.0f, 4.0f));
			sf::Color tileColor = GenerateBiomeColor(i, j);
			gridSquare.setFillColor(tileColor);
			_chunk.draw(gridSquare);
		}
	}

	_chunk.display();
	_isGenerated = true;
}

void Chunk::CreateChunk()
{
	/*sf::RectangleShape gridSquare;
	_chunk.clear(sf::Color::White);

	if (!_isGreyScale)
	{	
		int x = _noiseCenter.x - _chunkWidth / 4;
		int y = _noiseCenter.y - _chunkHeight / 4;
		for (int i = 0; i < _chunkWidth / 4; ++i)
		{
			for (int j = 0; j < _chunkHeight / 4; ++j)
			{
				sf::Uint8 height = Clamp((_heightNoise.GetNoise(x + i, y + j) + 1) / 2);
				gridSquare.setPosition(4 * i, 4 * j);
				gridSquare.setSize(sf::Vector2f(4.0f, 4.0f));
				gridSquare.setFillColor(GenerateBiomeColor(height));
				_chunk.draw(gridSquare);
			}
		}
	}
	else
	{
		int x = _noiseCenter.x - _chunkWidth / 2;
		int y = _noiseCenter.y - _chunkHeight / 2;
		for (int i = 0; i < _chunkWidth / 4; ++i)
		{
			for (int j = 0; j < _chunkHeight / 4; ++j)
			{
				sf::Uint8 height = Clamp((_heightNoise.GetNoise(x + i, y + j) + 1) / 2);
				gridSquare.setPosition(4 * i, 4 * j);
				gridSquare.setSize(sf::Vector2f(4.0f, 4.0f));
				gridSquare.setFillColor(sf::Color(height, height, height, 0xFF));
				_chunk.draw(gridSquare);
			}
		}
	}

	_chunk.display();
	_isGenerated = true;*/
}

sf::Color Chunk::GenerateBiomeColor(int i, int j)
{
	sf::Uint8 height = _heightMap[i][j];
	sf::Uint8 temperature = _temperatureMap[i][j];

	sf::Color biomeColor;
	if (height <= 70.0f)
	{
		biomeColor = Color::ClearWaterBlue;
	}
	else if (height <= 80.0f)
	{
		biomeColor = Color::Sand;
	}
	else if (height <= 120.0f)
	{
		if (temperature <= 85)
		{
			biomeColor = Color::Snow;
		}
		else if (temperature <= 170)
		{
			biomeColor = Color::GrassLand;
		}
		else
		{
			biomeColor = Color::Savannah;
		}
	}
	else if (height <= 160.0f)
	{
		if (temperature <= 85)
		{
			biomeColor = Color::PineForest;
		}
		else if (temperature <= 170)
		{
			biomeColor = Color::Forest;
		}
		else
		{
			biomeColor = Color::Jungle;
		}	
	}
	else if (height <= 200.0f)
	{
		biomeColor = Color::Hills;
	}
	else if (height <= 220.0f)
	{
		biomeColor = Color::Mountain;
	}
	else
	{
		biomeColor = Color::Snow;
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