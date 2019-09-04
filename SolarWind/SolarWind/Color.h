#pragma once
#include <SFML/Graphics.hpp>

class Color
{
	public:
		static const sf::Color Sand;
		static const sf::Color Ocean;
		static const sf::Color ClearWaterBlue;
		static const sf::Color Mountain;
		static const sf::Color Forest;
		static const sf::Color PineForest;
		static const sf::Color Jungle;
		static const sf::Color Savannah;
		static const sf::Color Desert;
		static const sf::Color Snow;
		static const sf::Color Hills;
		static const sf::Color GrassLand;
};

const sf::Color Color::Sand = sf::Color(76, 70, 50);
const sf::Color Color::Ocean = sf::Color(0, 47, 75);
const sf::Color Color::ClearWaterBlue = sf::Color(64, 164, 223);
const sf::Color Color::Mountain = sf::Color(150, 141, 153);
const sf::Color Color::Forest = sf::Color(34, 139, 34);
const sf::Color Color::PineForest = sf::Color(0, 47, 44);
const sf::Color Color::Jungle = sf::Color(16, 67, 53);
const sf::Color Color::Savannah = sf::Color(248, 199, 23);
const sf::Color Color::Desert = sf::Color(93, 79, 69);
const sf::Color Color::Snow = sf::Color(255, 250, 250);
const sf::Color Color::Hills = sf::Color(102, 204, 0);
const sf::Color Color::GrassLand = sf::Color(96, 128, 56);