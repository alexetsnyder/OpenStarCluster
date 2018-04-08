#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include "SGC.h"
#include "NameGenerator.h"
#include "SolarSystem.h"
#include "MathHelpers.h"

int main(int argc, char* args[])
{
	srand(time(0));

	SGC sgc;
	sgc.WINDOW_WIDTH = 1920; // 1000;
	sgc.WINDOW_HEIGHT = 980; // 800;
	sgc.TIME = sf::Clock();

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
			}
		}

		solarSystem.Update();

		window.clear(sf::Color::Black);

		window.draw(vertexes1, 2, sf::Lines);
		window.draw(vertexes2, 2, sf::Lines);

		sf::Sprite sprite(solarSystem.DrawTexture());
		window.draw(sprite);

		window.display();

		sgc.TIME.restart();
	}

	/*NameGenerator nameGenerator("C:\\Users\\alexe\\Google Drive\\root\\names.txt", 3);
	printf("Order 3 Markov Chain:\n");
	std::string name;
	for (int i = 0; i < 20; ++i)
	{
		if (nameGenerator.GenerateWord((rand() % 9) + 4, name))
			printf("%s %s\n", (nameGenerator.IsNewName(name)) ? "" : ">>", name.c_str());
	}*/

	std::cin.ignore();

	return 0;
}