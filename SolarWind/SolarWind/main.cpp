#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include "SGC.h"
#include "NameGenerator.h"
#include "SolarSystem.h"
#include "MathHelpers.h"
#include "ProcGen.h"

int main(int argc, char* args[])
{
	std::random_device device;
	unsigned int seed = device();
	std::mt19937 engine(seed);

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
	ssc.PlanetPadding = 0.0f;
	ssc.TotalPlanetCount = 12;
	ssc.StarMinPercOfMaxSize = 0.05f;
	ssc.StarMaxPercOfMaxSize = 0.10f;
	ssc.OrbitMinPercGrowth = 0.01f;
	ssc.OrbitMaxPercGrowth = 0.05f;
	ssc.OrbitMinPercOfStarRadius = 0.05f;
	ssc.OrbitMaxPercOfStarRadius = 0.20f;
	ssc.PlanetMinPercOfStarRadius = 0.20f;
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
		solarSystem.GenerateSolarSystem(engine);
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
						solarSystem.GenerateSolarSystem(engine);
					}
				}
				if (event.key.code == sf::Keyboard::S)
				{
					isWorldGen = !isWorldGen;
					if (!isWorldGen)
					{
						if (!solarSystem.IsGenerated())
						{
							solarSystem.GenerateSolarSystem(engine);
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
			solarSystem.CreateTexture();
		}

		window.clear(sf::Color::Black);

		if (isWorldGen)
		{
			procGen.Draw(&window);
		}
		else
		{
			solarSystem.Draw(&window);
		}

		window.display();

		sgc.TIME.restart();
	}

	std::cin.ignore();

	return 0;
}