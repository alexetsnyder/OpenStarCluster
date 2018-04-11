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
	sgc.WINDOW_WIDTH = 1000;
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

	float radius = GetInscribedCircleRadius(sgc.WINDOW_WIDTH, sgc.WINDOW_HEIGHT);
	SolarSystem solarSystem(sgc, sf::Vector2f(sgc.WINDOW_WIDTH / 2.0f, sgc.WINDOW_HEIGHT / 2.0f), radius, ssc);
	solarSystem.GenerateSolarSystem();

	sf::Vertex vertexes1[2] =
	{
		sf::Vertex(sf::Vector2f(sgc.WINDOW_WIDTH / 2, 0)),
		sf::Vertex(sf::Vector2f(sgc.WINDOW_WIDTH / 2, sgc.WINDOW_HEIGHT))
	};

	sf::Vertex vertexes2[2] =
	{
		sf::Vertex(sf::Vector2f(0, sgc.WINDOW_HEIGHT / 2)),
		sf::Vertex(sf::Vector2f(sgc.WINDOW_WIDTH, sgc.WINDOW_HEIGHT / 2))
	};

	sf::Vector2i mousePos;

	bool procGenScreen = false;
	ProcGen procGen;
	procGen.Init();
	procGen.SetSGC(sgc);

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
					solarSystem.GenerateSolarSystem();
				if (event.key.code == sf::Keyboard::S)
					procGenScreen = !procGenScreen;
			}
		}

		mousePos = sf::Mouse::getPosition(window);

		if (procGenScreen)
		{
			procGen.DrawTexure();
		}
		else
		{
			solarSystem.Update(mousePos);
			solarSystem.DrawTexture();
		}

		window.clear(sf::Color::Black);

		window.draw(vertexes1, 2, sf::Lines);
		window.draw(vertexes2, 2, sf::Lines);

		sf::Sprite sprite;
		if (procGenScreen)
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