#include "WorldGen.h"
#include "Color.h"

WorldGen::WorldGen()
{

}

WorldGen::WorldGen(SGC sgc, int width, int height)
{
	Init(sgc, width, height);
}

void WorldGen::Init(SGC sgc, int width, int height)
{
	_sgc = sgc;
	_fastNoise.SetSeed(_sgc.SEED);
	_fastNoise.SetNoiseType(FastNoise::SimplexFractal);
	_fastNoise.SetFractalOctaves(34);
	_width = width;
	_height = height;
	_isGreyScale = false;

	if (!_generated.create(_width, _height))
	{
		printf("Error creating texture in WorldGen.");
	}
	_spriteGen.setTexture(_generated.getTexture());

	if (!_greyGen.create(_width, _height))
	{
		printf("Error creating grey scale texture.");
	}
	_greySpr.setTexture(_greyGen);
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
		Generate();
	}
}

void WorldGen::Generate()
{
	sf::RectangleShape gridSquare;

	_generated.clear(sf::Color::White);
	for (int i = 0; i < _width / 4; ++i)
	{
		for (int j = 0; j < _height / 4; ++j)
		{
			sf::Uint8 height = Clamp((_fastNoise.GetNoise(i, j) + 1) / 2);
			gridSquare.setPosition(4 * i, 4 * j);
			gridSquare.setSize(sf::Vector2f(4.0f, 4.0f));
			gridSquare.setFillColor(GenerateBiomeColor(height));
			_generated.draw(gridSquare);
		}
	}

	_generated.display();
	_isGenerated = true;
}

void WorldGen::GenerateGreyScale()
{
	sf::Uint8* pixels = new sf::Uint8[_width * _height * 4];

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

	_isGenerated = true;
}

sf::Color WorldGen::GenerateBiomeColor(float noiseValue)
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

sf::Uint8 WorldGen::Clamp(float noise)
{
	return (noise >= 1.0f) ? 255 : (noise <= 0.0f) ? 0 : static_cast<sf::Uint8>(noise * 255.0f + 0.5f);
}

void WorldGen::Draw(sf::RenderTarget* target)
{
	if (_isGreyScale)
	{
		target->draw(_greySpr);
	}
	else
	{
		target->draw(_spriteGen);
	}
}
