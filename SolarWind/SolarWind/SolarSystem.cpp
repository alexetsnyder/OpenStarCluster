#include "SolarSystem.h"
#include <random>
#include <cstdio>
#include "MathHelpers.h"

SolarSystem::SolarSystem()
{
	
}

SolarSystem::SolarSystem(SGC sgc)
{
	Init(sgc);
}

SolarSystem::SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius)
{
	Init(sgc);
	_center = center;
	_maxRadius = maxRadius;
}

SolarSystem::SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius, SolarSystemConstants ssc)
{
	Init(sgc);
	_center = center;
	_maxRadius = maxRadius;
	_ssc = ssc;
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
}

void SolarSystem::GenerateSolarSystem()
{
	if (!_planets.empty())
	{
		_planets.clear();
	}

	if (!_system.create(_sgc.WINDOW_WIDTH, _sgc.WINDOW_HEIGHT))
	{
		printf("Error creating render texture in SolarSystem.");
		return;
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

void SolarSystem::DrawTexture()
{
	_system.clear(sf::Color::Black);
	_system.draw(_star.GetObject());
	for (Planet& planet : _planets)
	{
		_system.draw(planet.GetOrbit());
		_system.draw(planet.GetObject());
	}
	_system.draw(_nameText);	
	_system.display();
}

const sf::Texture& SolarSystem::GetTexture()
{
	return _system.getTexture();
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
		_nameText.setPosition(mousePos.x + 5.0f, mousePos.y);
		_nameText.setFillColor((_star.IsNewName()) ? sf::Color::Green : sf::Color::Yellow);
		_nameText.setString(_star.GetName());
	}
	else
	{
		for (Planet& planet : _planets)
		{
			if (planet.IsWithin(mousePos))
			{
				planet.SetColor(sf::Color::Blue);
				_nameText.setPosition(mousePos.x + 5.0f, mousePos.y);
				_nameText.setFillColor((planet.IsNewName()) ? sf::Color::Green : sf::Color::Yellow);
				_nameText.setString(planet.GetName());
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