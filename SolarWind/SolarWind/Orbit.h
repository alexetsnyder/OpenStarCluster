#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "SGC.h"

class Orbit
{
	public:
		Orbit();
		Orbit(SGC sgc, sf::Vector2f position, float radius, sf::Color color = sf::Color::White);
		void Init(SGC sgc);

		void Draw(sf::RenderTarget* target);

		void CalculateOrbitAngle(std::mt19937& engine);
		void CalculateNextOrbitAngle(sf::Time elapsed);
		sf::Vector2f GetOrbitPosition();

		void SetPosition(sf::Vector2f position) { _orbit.setPosition(position); }
		void SetRadius(float radius) { _orbit.setRadius(radius); }
		void SetColor(sf::Color color) { _orbit.setOutlineColor(color); }
		void SetVelocity(float velocity) { _velocity = velocity; }

		sf::Vector2f GetPosition() { return _orbit.getPosition(); }
		float GetRadius() { return _orbit.getRadius(); }
		sf::Color GetColor() { return _orbit.getOutlineColor(); }
		

	private:
		SGC _sgc;
		float _angle;
		float _velocity;
		std::mt19937 _engine;
		sf::CircleShape _orbit;
		std::uniform_int_distribution<int> _degreeUID;
};