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
			float height = (_fastNoise.GetNoise(i, j) + 1) / 2.0f * 255.0f;
			int index = 4 * (i * _height + j);
			pixels[index] = height;
			pixels[index + 1] = height;
			pixels[index + 2] = height;
			pixels[index + 3] = 255;
		}
	}
	
	_generated.update(pixels);
	delete[] pixels;

	_isGenerated = true;
}
