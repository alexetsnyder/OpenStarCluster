#include "ProcGen.h"

ProcGen::ProcGen()
{

}

ProcGen::ProcGen(SGC sgc, int width, int height)
{
	Init(sgc, width, height);
}

void ProcGen::Init(SGC sgc, int width, int height)
{
	_sgc = sgc;
	_fastNoise.SetSeed(_sgc.SEED);
	_fastNoise.SetNoiseType(FastNoise::SimplexFractal);
	_fastNoise.SetFractalOctaves(18);
	_width = width;
	_height = height;
}

void ProcGen::Generate()
{
	if (!_generated.create(_width, _height))
	{
		printf("Error creating texture in ProcGen.");
		return;
	}

	sf::Uint8* pixels = new sf::Uint8[_width * _height * 4];

	for (int i = 0; i < _width; ++i)
	{
		for (int j = 0; j < _height; ++j)
		{
			sf::Uint8 color = Clamp((_fastNoise.GetNoise(i, j) + 1) /2);
			int index = 4 * (i * _height + j);
			pixels[index] = color;
			pixels[index + 1] = color;
			pixels[index + 2] = color;
			pixels[index + 3] = 255;
		}
	}
	
	_generated.update(pixels);
	_spriteGen.setTexture(_generated);
	delete[] pixels;

	_isGenerated = true;
}

sf::Uint8 ProcGen::Clamp(float noise)
{
	return (noise >= 1.0f) ? 255 : (noise <= 0.0f) ? 0 : static_cast<sf::Uint8>(noise * 255.0f + 0.5f);
}

void ProcGen::Draw(sf::RenderTarget* target)
{
	target->draw(_spriteGen);
}
