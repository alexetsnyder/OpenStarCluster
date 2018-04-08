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
	sgc.WINDOW_HEIGHT = 800;
	sgc.WINDOW_WIDTH = 1000;
	sgc.TIME = sf::Clock();

	sf::RenderWindow window(sf::VideoMode(sgc.WINDOW_WIDTH, sgc.WINDOW_HEIGHT), "Solar Wind!");
	
	float radius = GetInscribedCircleRadius(sgc.WINDOW_WIDTH, sgc.WINDOW_HEIGHT);
	SolarSystem solarSystem(sgc, sf::Vector2f(sgc.WINDOW_WIDTH / 2.0f, sgc.WINDOW_HEIGHT / 2.0f), radius);
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
		sf::Sprite sprite(solarSystem.DrawTexture());
		window.draw(sprite);
		window.draw(vertexes1, 2, sf::Lines);
		window.draw(vertexes2, 2, sf::Lines);
		window.display();

		sgc.TIME.restart();
	}

	NameGenerator nameGenerator("C:\\Users\\alexe\\Google Drive\\root\\names.txt", 3);
	printf("Order 3 Markov Chain:\n");
	std::string name;
	for (int i = 0; i < 20; ++i)
	{
		if (nameGenerator.GenerateWord((rand() % 9) + 4, name))
			printf("%s %s\n", (nameGenerator.IsNewName(name)) ? "" : ">>", name.c_str());
	}

	std::cin.ignore();

	return 0;
}