#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <queue>
#include "SolarSystem.h"
#include "WorldGen.h"

class Game
{
	public:
		Game();

		void Start();
		void Loop();

		void Update();
		void Draw();

		void PollEvents();
		void MoveWorld();

	private:
		sf::RenderWindow _window;

		std::mt19937 _engine;

		sf::Vector2i _mousePos;
		sf::Vector2i _prvMousePos;

		bool _isWorldGen;
		bool _isGreyScale;
		bool _isMoveWorld;
		WorldGen _world;
		sf::View _worldView;

		SolarSystem _solarSystem;
		sf::View _solarSystemView;

};