#pragma once
#include <SFML/Graphics.hpp>
#include "Orbit.h"
#include "SGC.h"

class AstroObject
{
	public:
		AstroObject();
		AstroObject(sf::Vector2f position, float radius);
		AstroObject(sf::Vector2f position, float radius, sf::Color color);

		void SetPosition(sf::Vector2f position) { _body.setPosition(position); }
		void SetRadius(float radius) { _body.setRadius(radius); }
		void SetColor(sf::Color color) { _body.setFillColor(color); }
		void SetSGC(SGC sgc) { _sgc = sgc; }

		sf::Vector2f GetPosition() { return _body.getPosition(); }
		float GetRadius() { return _body.getRadius(); }
		sf::CircleShape GetObject() { return _body; }
		SGC& GetSGC() { return _sgc; }

	private:
		sf::CircleShape _body;
		SGC _sgc;
};

class Planet : public AstroObject
{
	public:
		Planet();
		Planet(sf::Vector2f position, float radius, sf::Color color = sf::Color::Green);

		void CreateOrbit(sf::Vector2f orbitPos, float orbitRadius);
		void UpdateOrbit();
		void CalculatePosition();

		sf::CircleShape GetOrbit() { return _orbit.GetOrbit(); }

		void SetOrbitRadius(float radius) { _orbit.SetRadius(radius); }
		void SetOrbitPosition(sf::Vector2f position) { _orbit.SetPosition(position); }

	private:
		Orbit _orbit;
};

class Star : public AstroObject
{
	public:
		Star();
		Star(sf::Vector2f position, float radius, sf::Color color);
	private:

};