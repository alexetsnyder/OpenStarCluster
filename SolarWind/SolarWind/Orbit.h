#pragma once
#include <SFML/Graphics.hpp>

class Orbit
{
	public:
		Orbit();
		Orbit(sf::Vector2f position, float radius, sf::Color color = sf::Color::White);

		void SetPosition(sf::Vector2f position) { _orbit.setPosition(position); }
		void SetRadius(float radius) { _orbit.setRadius(radius); }
		void SetColor(sf::Color color) { _orbit.setOutlineColor(color); }
		void SetVelocity(float velocity) { _velocity = velocity; }

		sf::Vector2f GetPosition() { return _orbit.getPosition(); }
		float GetRadius() { return _orbit.getRadius(); }
		sf::Color GetColor() { return _orbit.getOutlineColor(); }

		void CalculateNextOrbitAngle(sf::Time elapsed);
		sf::Vector2f GetOrbitPosition();
		sf::CircleShape GetOrbit() { return _orbit; }

	private:
		float _angle;
		sf::CircleShape _orbit;
		float _velocity;
};