#include "SolarSystem.h"
#include <random>
#include <cstdio>
#include "MathHelpers.h"

SolarSystem::SolarSystem()
{
	
}

//SolarSystem::SolarSystem(SGC sgc)
//{
//	Init(sgc);
//}

SolarSystem::SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius)
{
	_center = center;
	_maxRadius = maxRadius;
	Init(sgc);
}

SolarSystem::SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius, SolarSystemConstants ssc)
{
	_center = center;
	_maxRadius = maxRadius;
	_ssc = ssc;
	std::random_device device;
	_seed = device();
	std::mt19937 mTwistRand(_seed);
	Init(sgc);
}

void SolarSystem::Init(SGC sgc)
{
	_sgc = sgc;
	std::string fontPath = "Fonts\\SimSun.ttf";
	_font.loadFromFile(fontPath);
	_nameText.setFont(_font);
	_nameText.setStyle(sf::Text::Bold);
	_nameText.setCharacterSize(22);
	_isGenerated = false;

	if (!_system.create(_maxRadius, _maxRadius))
	{
		printf("Error creating render texture in SolarSystem.");
	}
	_spriteSystem.setTexture(_system.getTexture());
}

void SolarSystem::GenerateSolarSystem()
{
	if (!_planets.empty())
	{
		_planets.clear();
	}

	bool success = true;
	int index = 0;

	int randModForStar;
	if (!GetRandMod(_maxRadius, _ssc.StarMinPercOfMaxSize, _ssc.StarMaxPercOfMaxSize, randModForStar))
	{
		printf("Problem when generating solar system. randMod: %d", randModForStar);
		return;
	}
	float randMinForStar = GetRandMin(_maxRadius, _ssc.StarMinPercOfMaxSize);

	_star = Star();
	_star.Init();
	_star.SetSGC(_sgc);
	_star.SetRadius((rand() % randModForStar) + randMinForStar);
	_star.SetPosition(sf::Vector2f(_center.x - _star.GetRadius(), _center.y - _star.GetRadius()));
	_star.SetColor(sf::Color::Red);
	float currentRadius = _star.GetRadius();

	int randModForOrbit;
	if (!GetRandMod(_star.GetRadius(), _ssc.OrbitMinPercOfStarRadius, _ssc.OrbitMaxPercOfMaxStarRadius, randModForOrbit))
	{
		printf("Problem when generating solar system. randMod: %d", randModForOrbit);
		success = false;
	}
	float randMinForOrbit = GetRandMin(_star.GetRadius(), _ssc.OrbitMinPercOfStarRadius);

	int randMaxMod = (int)((_ssc.OrbitMaxPercGrowth * 100 - _ssc.OrbitMinPercGrowth * 100) + 1);
	float randPercGrowth = ((rand() % randMaxMod) + _ssc.OrbitMinPercGrowth * 100) / 100.0f;
	float minPerc = _ssc.OrbitMinPercOfStarRadius + randPercGrowth;

	float prvPlanetRadius = 0.0f;
	int planetCount = (rand() % _ssc.TotalPlanetCount) + 1;
	while (planetCount-- > 0)
	{
		_planets.push_back(Planet());
		_planets[index].Init();
		_planets[index].SetSGC(_sgc);
		int randModForPlanet;
		if (!GetRandMod(_star.GetRadius(), _ssc.PlanetMinPercOfStarRadius, _ssc.PlanetMaxPercOfStarRadius, randModForPlanet))
		{
			printf("Problem when generating solar system. randMod: %d", randModForPlanet);
			success = false;
			break;
		}
		float randMinForPlanet = GetRandMin(_star.GetRadius(), _ssc.PlanetMinPercOfStarRadius);
		_planets[index].SetRadius((rand() % randModForPlanet) + randMinForPlanet);

		float orbitRadius = (rand() % randModForOrbit) + randMinForOrbit;
		orbitRadius += currentRadius + prvPlanetRadius + _planets[index].GetRadius() + _ssc.PlanetPadding;
		sf::Vector2f orbitPos(_center.x - orbitRadius, _center.y - orbitRadius);

		if (2 * orbitRadius > _maxRadius)
		{
			_planets.pop_back();
			if (_planets.empty())
				success = false;
			break;
		}
		_planets[index].CreateOrbit(orbitPos, orbitRadius);
		_planets[index].CalculatePosition();
		_planets[index].SetColor(sf::Color::Cyan);
		prvPlanetRadius = _planets[index].GetRadius();

		randMaxMod = (int)((_ssc.OrbitMaxPercGrowth * 100 - _ssc.OrbitMinPercGrowth * 100) + 1);
		randPercGrowth = ((rand() % randMaxMod) + _ssc.OrbitMinPercGrowth * 100) / 100.0f;
		minPerc += randPercGrowth;
		if (!GetRandMod(orbitRadius, _ssc.OrbitMinPercOfStarRadius, _ssc.OrbitMaxPercOfMaxStarRadius, randModForOrbit))
		{
			printf("Problem when generating solar system. randMod: %d", randModForOrbit);
			success = false;
			break;
		}
		randMinForOrbit = GetRandMin(orbitRadius, _ssc.OrbitMinPercOfStarRadius);

		currentRadius = orbitRadius;
		++index;
	}

	if (!success)
	{
		printf("Solar System Failed to Generate.");
	}
	_isGenerated = success;

	printf("\n----------------------------------------------------------------\n");

}

void SolarSystem::CreateTexture()
{
	_system.clear(sf::Color::Black);
	_star.Draw(&_system);
	for (Planet& planet : _planets)
	{
		planet.Draw(&_system);
	}
	_system.draw(_nameText);	
	_system.display();
	
}

void SolarSystem::Draw(sf::RenderTarget* target)
{
	target->draw(_spriteSystem);
}

void SolarSystem::Update(sf::Vector2i mousePos)
{
	_nameText.setFillColor(sf::Color::Transparent);
	_star.SetColor(sf::Color::Red);
	for (Planet& planet : _planets)
	{
		planet.SetColor(sf::Color::Cyan);
	}

	if (_star.IsWithin(mousePos))
	{
		_star.SetColor(sf::Color::Blue);
		_nameText.setString(_star.GetName());		
		_nameText.setPosition(GetTextPosition(mousePos));
		_nameText.setFillColor((_star.IsNewName()) ? sf::Color::Green : sf::Color::Yellow);
	}
	else
	{
		for (Planet& planet : _planets)
		{
			if (planet.IsWithin(mousePos))
			{
				planet.SetColor(sf::Color::Blue);
				_nameText.setString(planet.GetName());
				_nameText.setPosition(GetTextPosition(mousePos));
				_nameText.setFillColor((planet.IsNewName()) ? sf::Color::Green : sf::Color::Yellow);
			}
		}
	}

	//SimulatePlanets();
}

void SolarSystem::SimulatePlanets()
{
	for (Planet& planet : _planets)
	{
		planet.UpdateOrbit();
		planet.CalculatePosition();
	}
}

sf::Vector2f SolarSystem::GetTextPosition(sf::Vector2i mousePos)
{
	sf::Vector2f retVector(mousePos.x + 5.0f, mousePos.y - 5.0f);

	if (mousePos.x + _nameText.getLocalBounds().width > _maxRadius)
	{
		retVector.x += -5.0f - _nameText.getLocalBounds().width;
	}

	if (mousePos.y + _nameText.getLocalBounds().height > _maxRadius)
	{
		retVector.y += -5.0f - _nameText.getLocalBounds().height;
	}

	return retVector;
}