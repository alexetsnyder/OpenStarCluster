#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AstroObject.h"
#include "SGC.h"

struct SolarSystemConstants
{
	float PlanetPadding;
	int TotalPlanetCount;

	float StarMinPercOfMaxSize;
	float StarMaxPercOfMaxSize;

	float OrbitMinPercOfStarRadius;
	float OrbitMaxPercOfStarRadius;
	float OrbitMinPercGrowth;
	float OrbitMaxPercGrowth;

	float PlanetMinPercOfStarRadius;
	float PlanetMaxPercOfStarRadius;
};

class SolarSystem
{
	public:
		SolarSystem();
		SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius);
		SolarSystem(SGC sgc, sf::Vector2f center, float maxRadius, SolarSystemConstants ssc);
		void Init(SGC sgc);

		void GenerateSolarSystem(std::mt19937& engine);

		void CreateTexture();
		bool IsPointWithinPlanet(sf::Vector2i mousePos);
		void Update(sf::Vector2i mousePos, sf::Time elapsed);
		void Draw(sf::RenderTarget* target);

		bool IsGenerated() { return _isGenerated; }

		void SetSGC(SGC sgc) { _sgc = sgc; }
		void SetSSC(SolarSystemConstants ssc) { _ssc = ssc; }
		void SetCenter(sf::Vector2f center) { _center = center; }
		void SetMaxRadius(float maxRadius) { _maxRadius = maxRadius; }

	private:
		void SimulatePlanets(sf::Time elapsed);
		sf::Vector2f GetTextPosition(sf::Vector2i mousePos);

		SGC _sgc;
		SolarSystemConstants _ssc;

		sf::Font _font;
		sf::Text _nameText;

		bool _isGenerated;
		float _maxRadius;
		sf::Vector2f _center;

		Star _star;
		std::vector<Planet> _planets;
		sf::Sprite _spriteSystem;
		sf::RenderTexture _system;
};

