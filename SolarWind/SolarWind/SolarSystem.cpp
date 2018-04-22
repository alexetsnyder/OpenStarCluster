#include "SolarSystem.h"
#include <random>
#include <cstdio>

SolarSystem::SolarSystem()
{
	
}

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

void SolarSystem::GenerateSolarSystem(std::mt19937& engine)
{
	if (!_planets.empty())
	{
		_planets.clear();
	}

	bool success = true;
	int index = 0;

	std::uniform_int_distribution<int> starUID(_ssc.StarMinPercOfMaxSize * _maxRadius, _ssc.StarMaxPercOfMaxSize * _maxRadius);
	
	_star = Star();
	_star.Init();
	_star.SetSGC(_sgc);
	_star.SetRadius(starUID(engine));
	_star.SetPosition(sf::Vector2f(_center.x - _star.GetRadius(), _center.y - _star.GetRadius()));
	_star.SetColor(sf::Color::Red);

	float currentRadius = _star.GetRadius();
	float minPerc = _ssc.OrbitMinPercOfStarRadius;
	float maxPerc = _ssc.OrbitMaxPercOfStarRadius;
	float prvPlanetRadius = 0.0f;

	std::uniform_int_distribution<int> planetCountUID(1, _ssc.TotalPlanetCount);
	int planetCount = planetCountUID(engine);

	while (planetCount-- > 0)
	{
		std::uniform_int_distribution<int> planetUID(_ssc.PlanetMinPercOfStarRadius * _star.GetRadius(), _ssc.PlanetMaxPercOfStarRadius * _star.GetRadius());
		std::uniform_int_distribution<int> orbitUID(minPerc * currentRadius, maxPerc * currentRadius);

		_planets.push_back(Planet());
		_planets[index].Init();
		_planets[index].SetSGC(_sgc);
		_planets[index].SetRadius(planetUID(engine));

		float orbitRadius = orbitUID(engine);
		orbitRadius += currentRadius + prvPlanetRadius + _planets[index].GetRadius() + _ssc.PlanetPadding;
		sf::Vector2f orbitPos(_center.x - orbitRadius, _center.y - orbitRadius);
		if (2 * orbitRadius > _maxRadius)
		{
			_planets.pop_back();
			if (_planets.empty())
				success = false;
			break;
		}

		_planets[index].CreateOrbit(engine, orbitPos, orbitRadius);
		_planets[index].CalculatePosition();
		_planets[index].SetColor(sf::Color::Cyan);
		prvPlanetRadius = _planets[index].GetRadius();

		std::uniform_int_distribution<int> percUID((int)(_ssc.OrbitMinPercGrowth * 100), (int)(_ssc.OrbitMaxPercGrowth * 100));
		float percGrowth = percUID(engine) / 100.0f;
		minPerc += percGrowth;
		maxPerc += percGrowth;

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