#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include "SGC.h"
#include "NameGenerator.h"
#include "SolarSystem.h"
#include "MathHelpers.h"
#include "WorldGen.h"

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

	bool isWorldGen = true;

	float radius = GetInscribedCircleRadius(sgc.WINDOW_WIDTH, sgc.WINDOW_HEIGHT);
	sf::Vector2f center = sf::Vector2f(sgc.WINDOW_WIDTH / 2.0f, sgc.WINDOW_HEIGHT / 2.0f);
	SolarSystem solarSystem(sgc, center, radius, ssc);

	WorldGen world(sgc, sgc.WINDOW_WIDTH, sgc.WINDOW_HEIGHT);

	if (isWorldGen)
	{
		world.Generate();
	}
	else
	{
		solarSystem.GenerateSolarSystem(engine);
	}

	sf::View worldView(sf::FloatRect(0, 0, 800, 800));

	sf::Vector2i mousePos;

	bool MoveWorld = false;
	sf::Vector2f prvMousePos(0.0f, 0.0f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				worldView.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::R)
				{
					if (isWorldGen)
					{
						//world.Generate();
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
						if (!world.IsGenerated())
						{
							//world.Generate();
						}
					}
				}
				if (event.key.code == sf::Keyboard::Tab)
				{
					world.ToggleGreyScale();
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				printf("x: %.0f y: %.0f\n", worldPos.x, worldPos.y);
				prvMousePos.x = event.mouseButton.x;
				prvMousePos.y = event.mouseButton.y;
				MoveWorld = true;
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (isWorldGen)
				{
					//world.LoadChunks(worldView.getCenter());
				}
				MoveWorld = false;
			}
		}

		mousePos = sf::Mouse::getPosition(window);

		if (MoveWorld)
		{
			float x = prvMousePos.x - mousePos.x;
			float y = prvMousePos.y - mousePos.y;
			worldView.move(x, y);

			prvMousePos.x = mousePos.x;
			prvMousePos.y = mousePos.y;

			if (isWorldGen)
			{
				world.UpdateChunks(worldView.getCenter());
			}
		}

		window.setView(worldView);

		if (isWorldGen)
		{
			world.Update(worldView.getCenter());
		}
		else 
		{
			solarSystem.Update(mousePos);
			solarSystem.CreateTexture();
		}

		window.clear(sf::Color::Black);

		if (isWorldGen)
		{
			world.Draw(&window);
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