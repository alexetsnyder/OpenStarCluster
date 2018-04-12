#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include "SGC.h"
#include "NameGenerator.h"
#include "SolarSystem.h"
#include "MathHelpers.h"
#include "ProcGen.h"

int main(int argc, char* args[])
{
	unsigned int seed = time(0);
	srand(seed);

	SGC sgc;
	sgc.WINDOW_WIDTH = 800;
	sgc.WINDOW_HEIGHT = 800;
	sgc.SEED = seed;
	sgc.TIME = sf::Clock();

	printf("---------------------------------------------\n");
	printf("Seed: %d\n", seed);
	printf("---------------------------------------------\n");

	sf::RenderWindow window(sf::VideoMode(sgc.WINDOW_WIDTH, sgc.WINDOW_HEIGHT), "Solar Wind!");
	
	SolarSystemConstants ssc;
	ssc.RadiusPadding = 0.0f;
	ssc.PlanetPadding = 0.0f;
	ssc.TotalPlanetCount = 12;
	ssc.StarMinPercOfMaxSize = 0.01f;
	ssc.StarMaxPercOfMaxSize = 0.10f;
	ssc.OrbitMinPercGrowth = 0.10f;
	ssc.OrbitMaxPercGrowth = 0.60f;
	ssc.OrbitMinPercOfStarRadius = 0.01f;
	ssc.OrbitMaxPercOfMaxStarRadius = 0.40f;
	ssc.PlanetMinPercOfStarRadius = 0.10f;
	ssc.PlanetMaxPercOfStarRadius = 0.40f;

	bool isWorldGen = false;

	float radius = GetInscribedCircleRadius(sgc.WINDOW_WIDTH, sgc.WINDOW_HEIGHT);
	sf::Vector2f center = sf::Vector2f(sgc.WINDOW_WIDTH / 2.0f, sgc.WINDOW_HEIGHT / 2.0f);
	SolarSystem solarSystem(sgc, center, radius, ssc);

	ProcGen procGen(sgc, sgc.WINDOW_WIDTH, sgc.WINDOW_HEIGHT);

	if (isWorldGen)
	{
		procGen.Generate();
	}
	else
	{
		solarSystem.GenerateSolarSystem();
	}

	sf::Vector2i mousePos;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::R)
				{
					if (isWorldGen)
					{
						procGen.NewSeed(time(0));
						procGen.Generate();
					}
					else
					{
						solarSystem.GenerateSolarSystem();
					}
				}
				if (event.key.code == sf::Keyboard::S)
				{
					isWorldGen = !isWorldGen;
					if (!isWorldGen)
					{
						if (!solarSystem.IsGenerated())
						{
							solarSystem.GenerateSolarSystem();
						}
					}
					else
					{
						if (!procGen.IsGenerated())
						{
							procGen.Generate();
						}
					}
				}
			}
		}

		mousePos = sf::Mouse::getPosition(window);

		if (!isWorldGen)
		{
			solarSystem.Update(mousePos);
			solarSystem.DrawTexture();
		}

		window.clear(sf::Color::Black);

		sf::Sprite sprite;
		if (isWorldGen)
		{
			sprite.setTexture(procGen.GetTexture());
		}
		else
		{
			sprite.setTexture(solarSystem.GetTexture());
		}
		window.draw(sprite);

		window.display();

		sgc.TIME.restart();
	}

	std::cin.ignore();

	return 0;
}