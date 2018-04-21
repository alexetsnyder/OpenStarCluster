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
	_width = width;
	_height = height;
	_isGreyScale = false;

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
	_chunks.push_back(new Chunk(_sgc, sf::Vector2f(_width / 2, _height / 2), _width, _height));
	LoadChunks(sf::Vector2f(_width / 2, _height / 2));
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

void WorldGen::UnloadChunks()
{
	for (Chunk* chunk : _chunks)
	{

	}
}
bool WorldGen::IsChunkWithinView(Chunk chunk, sf::Vector2f viewCenter)
{
	float deltaX = chunk.GetCenter().x - viewCenter.x;
	float deltaY = chunk.GetCenter().y - viewCenter.y;

	return false;
}

void WorldGen::LoadChunks(sf::Vector2f viewCenter)
{
	for (int i = 0; i < _chunks.size(); ++i)
	{	
		float totalWidth = 0.5f * (_chunks[i]->GetWidth() + _width);
		float totalHeight = 0.5f * (_chunks[i]->GetHeight() + _height);

		if (!_chunks[i]->HasLeftN())
		{
			sf::Vector2f point(_chunks[i]->GetCenter().x - _chunks[i]->GetWidth() / 2, _chunks[i]->GetCenter().y);
			if (IsWithinView(point, _chunks[i]->GetWidth(), _chunks[i]->GetHeight(), viewCenter))
			{
				_chunks.push_back(new Chunk(_sgc, sf::Vector2f(point.x - _chunks[i]->GetWidth() / 2, point.y), _chunks[i]->GetWidth(), _chunks[i]->GetHeight()));
				_chunks[i]->SetLeftN(true);
				_chunks.back()->SetRightN(true);
			}
		}
		if (!_chunks[i]->HasRightN())
		{
			sf::Vector2f point(_chunks[i]->GetCenter().x + _chunks[i]->GetWidth() / 2, _chunks[i]->GetCenter().y);
			if (IsWithinView(point, _chunks[i]->GetWidth(), _chunks[i]->GetHeight(), viewCenter))
			{
				_chunks.push_back(new Chunk(_sgc, sf::Vector2f(point.x + _chunks[i]->GetWidth() / 2, point.y), _chunks[i]->GetWidth(), _chunks[i]->GetHeight()));
				_chunks[i]->SetRightN(true);
				_chunks.back()->SetLeftN(true);
			}
		}
		if (!_chunks[i]->HasTopN())
		{
			sf::Vector2f point(_chunks[i]->GetCenter().x, _chunks[i]->GetCenter().y - _chunks[i]->GetHeight() / 2);
			if (IsWithinView(point, _chunks[i]->GetWidth(), _chunks[i]->GetHeight(), viewCenter))
			{
				_chunks.push_back(new Chunk(_sgc, sf::Vector2f(point.x, point.y - _chunks[i]->GetHeight() / 2), _chunks[i]->GetWidth(), _chunks[i]->GetHeight()));
				_chunks[i]->SetTopN(true);
				_chunks.back()->SetBottomN(true);
			}
		}
		if (!_chunks[i]->HasBottomN())
		{
			sf::Vector2f point(_chunks[i]->GetCenter().x, _chunks[i]->GetCenter().y + _chunks[i]->GetHeight() / 2);
			if (IsWithinView(point, _chunks[i]->GetWidth(), _chunks[i]->GetHeight(), viewCenter))
			{
				_chunks.push_back(new Chunk(_sgc, sf::Vector2f(point.x, point.y + _chunks[i]->GetHeight() / 2), _chunks[i]->GetWidth(), _chunks[i]->GetHeight()));
				_chunks[i]->SetBottomN(true);
				_chunks.back()->SetTopN(true);
			}
		}
	}
}

bool WorldGen::IsWithinView(sf::Vector2f point, int width, int height, sf::Vector2f viewCenter)
{
	float deltaX = point.x - viewCenter.x;
	float deltaY = point.y - viewCenter.y;

	deltaX = (deltaX < 0) ? -1 * deltaX : deltaX;
	deltaY = (deltaY < 0) ? -1 * deltaY : deltaY;

	return (deltaX <= _width / 2 && deltaY  <= _height / 2);
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
