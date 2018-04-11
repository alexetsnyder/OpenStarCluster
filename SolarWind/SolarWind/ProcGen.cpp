#include "ProcGen.h"
#include "FastNoise.h"

ProcGen::ProcGen()
{

}

void ProcGen::Init()
{
	if (!_texture.create(1000, 800))
		printf("Texture could not be created.");
}

void ProcGen::DrawTexure()
{
	FastNoise fastNoise;
	fastNoise.SetSeed(_sgc.SEED);
	fastNoise.SetNoiseType(FastNoise::SimplexFractal);
	fastNoise.SetFractalOctaves(18);

	const int M = 250;
	const int N = 200;

	//float heightMap[M][N];
	sf::RectangleShape gridSquare;

	_texture.clear(sf::Color::Black);
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			float height = (fastNoise.GetNoise(i, j) + 1) / 2.0f * 255.0f;
			gridSquare.setFillColor(sf::Color(height, height, height, 0xFF));
			gridSquare.setSize(sf::Vector2f(4.0f, 4.0f));
			gridSquare.setPosition(4 * i, 4 * j);
			_texture.draw(gridSquare);
		}
	}
	
	_texture.display();
}

const sf::Texture& ProcGen::GetTexture()
{
	return _texture.getTexture();
}
