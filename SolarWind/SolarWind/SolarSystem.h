#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AstroObject.h"
#include "SGC.h"

class SolarSystem
{
	public:
		SolarSystem();
		SolarSystem(SGC sgc);
		SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius);

		void GenerateSolarSystem();

		const sf::Texture& DrawTexture();
		void Update();

		void SetSGC(SGC sgc) { _sgc = sgc; }

	private:
		void SimulatePlanets();

		SGC _sgc;
		float _maxRadius;
		sf::Vector2f _center;
		Star _star;
		std::vector<Planet> _planets;
		sf::RenderTexture _system;
};