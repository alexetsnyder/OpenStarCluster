#pragma once
#include <SFML/Graphics.hpp>
#include "Orbit.h"
#include "SGC.h"
#include "NameGenerator.h"

class AstroObject
{
	public:
		AstroObject();
		AstroObject(sf::Vector2f position, float radius);
		AstroObject(sf::Vector2f position, float radius, sf::Color color);

		bool IsWithin(sf::Vector2i mousePos);

		void SetPosition(sf::Vector2f position) { _body.setPosition(position); }
		void SetRadius(float radius) { _body.setRadius(radius); }
		void SetColor(sf::Color color) { _body.setFillColor(color); }
		void SetSGC(SGC sgc) { _sgc = sgc; }
		void SetName(std::string name) { _name = name; }

		sf::Vector2f GetPosition() { return _body.getPosition(); }
		float GetRadius() { return _body.getRadius(); }
		sf::CircleShape GetObject() { return _body; }
		SGC& GetSGC() { return _sgc; }
		std::string GetName() { return _name; }

		bool& IsNewName() { return _isNewName; }

	private:
		bool _isNewName;
		sf::CircleShape _body;
		std::string _name;
		SGC _sgc;
};

class Planet : public AstroObject
{
	public:
		Planet();
		Planet(sf::Vector2f position, float radius, sf::Color color = sf::Color::Cyan);

		void Init();

		void CreateOrbit(sf::Vector2f orbitPos, float orbitRadius);
		void UpdateOrbit();
		void CalculatePosition();

		sf::CircleShape GetOrbit() { return _orbit.GetOrbit(); }

		void SetOrbitRadius(float radius) { _orbit.SetRadius(radius); }
		void SetOrbitPosition(sf::Vector2f position) { _orbit.SetPosition(position); }

	private:
		void GenerateName();

		Orbit _orbit;
		static NameGenerator planetNameGen;
};

class Star : public AstroObject
{
	public:
		Star();
		Star(sf::Vector2f position, float radius, sf::Color color);

		void Init();

	private:
		void GenerateName();
		static NameGenerator starNameGen;

};