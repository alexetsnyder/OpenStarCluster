#include "SolarSystem.h"
#include <random>
#include <cstdio>
#include "MathHelpers.h"

SolarSystem::SolarSystem()
{

}

SolarSystem::SolarSystem(SGC sgc)
{
	_sgc = sgc;
}

SolarSystem::SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius)
{
	_sgc = sgc;
	_center = center;
	_maxRadius = maxRadius;
}

void SolarSystem::GenerateSolarSystem()
{
	if (!_system.create(_sgc.WINDOW_WIDTH, _sgc.WINDOW_HEIGHT))
	{
		printf("Error creating render texture in SolarSystem.");
	}

	if (!_planets.empty())
	{
		_planets.clear();
	}

	bool ranOnce = false;
	int index = 0;
	float currentRadius = 0.0f;

	float minPerc = 0.05f;
	float maxPerc = 0.10f;
	int randModForStarAndOrbit;
	if (!GetRandMod(_maxRadius, minPerc, maxPerc, randModForStarAndOrbit))
	{
		printf("Problem when generating solar system. randMod: %d", randModForStarAndOrbit);
		return;
	}
	float randMinForStarAndOrbit = GetRandMin(_maxRadius, minPerc);

	_star.SetSGC(_sgc);
	_star.SetRadius((rand() % randModForStarAndOrbit) + randMinForStarAndOrbit);
	_star.SetPosition(sf::Vector2f(_center.x - _star.GetRadius(), _center.y - _star.GetRadius()));
	currentRadius += _star.GetRadius();

	int planetCount = (rand() % 12) + 1;
	while (currentRadius < _maxRadius - 20.0f && planetCount-- > 0)
	{
		ranOnce = true;

		minPerc += 0.10f;
		randMinForStarAndOrbit = GetRandMin(_maxRadius, minPerc);
		
		float orbitRadius = (rand() % randModForStarAndOrbit) + randMinForStarAndOrbit;
		sf::Vector2f orbitPos(_center.x - orbitRadius, _center.y -orbitRadius);

		_planets.push_back(Planet());
		_planets[index].SetSGC(_sgc);
		int randModForPlanet;
		if (!GetRandMod(_star.GetRadius(), 0.20f, 0.40f, randModForPlanet))
		{
			printf("Problem when generating solar system. randMod: %d", randModForPlanet);
			return;
		}
		float randMinForPlanet = GetRandMin(_star.GetRadius(), 0.20);
		_planets[index].SetRadius((rand() % randModForPlanet) + randMinForPlanet);
		_planets[index].CalculatePosition(orbitPos, orbitRadius);

		++index;
		currentRadius += orbitRadius;
	}

	if (!ranOnce)
	{
		printf("No Planets were generated. Try increasing max radius.");
	}
}

const sf::Texture& SolarSystem::DrawTexture()
{
	_system.clear(sf::Color::Black);
	_system.draw(_star.GetObject());
	for (Planet& planet : _planets)
	{
		_system.draw(planet.GetOrbit());
		_system.draw(planet.GetObject());
	}
	_system.display();
	return _system.getTexture();
}

void SolarSystem::Update()
{
	//SimulatePlanets();
}

void SolarSystem::SimulatePlanets()
{
	for (Planet& planet : _planets)
	{
		planet.CalculateNewPosition();
	}
}