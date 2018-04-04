#include <SFML/Graphics.hpp>
#include <iostream>
#include "NameGenerator.h"

int main(int argc, char* args[])
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Hello World!");
	sf::CircleShape shape(100.0f);
	shape.setFillColor(sf::Color::Blue);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
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