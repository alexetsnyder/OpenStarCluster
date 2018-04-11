#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AstroObject.h"
#include "SGC.h"

struct SolarSystemConstants
{
	float RadiusPadding;
	float PlanetPadding;
	int TotalPlanetCount;

	float StarMinPercOfMaxSize;
	float StarMaxPercOfMaxSize;

	float OrbitMinPercOfStarRadius;
	float OrbitMaxPercOfMaxStarRadius;
	float OrbitMinPercGrowth;
	float OrbitMaxPercGrowth;

	float PlanetMinPercOfStarRadius;
	float PlanetMaxPercOfStarRadius;
};

class SolarSystem
{
	public:
		SolarSystem();
		SolarSystem(SGC sgc);
		SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius);
		SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius, SolarSystemConstants ssc);

		void GenerateSolarSystem();

		void DrawTexture();
		const sf::Texture& GetTexture();
		void Update(sf::Vector2i mousePos);

		void SetSGC(SGC sgc) { _sgc = sgc; }
		void SetSSC(SolarSystemConstants ssc) { _ssc = ssc; }

	private:
		void SimulatePlanets();

		SGC _sgc;
		SolarSystemConstants _ssc;
		sf::Text _nameText;
		sf::Font _arial;
		float _maxRadius;
		sf::Vector2f _center;
		Star _star;
		std::vector<Planet> _planets;
		sf::RenderTexture _system;
};

