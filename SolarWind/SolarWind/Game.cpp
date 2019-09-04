#include "Game.h"
#include "MathHelper.h"
#include <chrono>
#include <thread>

Game::Game()
{

}

void Game::Start()
{
	std::random_device device;
	unsigned int seed = device();
	_engine.seed(seed);

	printf("---------------------------------------------\n");
	printf("Seed: %u\n", seed);
	printf("---------------------------------------------\n");

	_sgc.WINDOW_WIDTH = 800;
	_sgc.WINDOW_HEIGHT = 800;
	_sgc.SEED = seed;

	_window.create(sf::VideoMode(_sgc.WINDOW_WIDTH, _sgc.WINDOW_HEIGHT), "Solar Wind!");

	_ssc.PlanetPadding = 0.0f;
	_ssc.TotalPlanetCount = 12;
	_ssc.StarMinPercOfMaxSize = 0.05f;
	_ssc.StarMaxPercOfMaxSize = 0.10f;
	_ssc.OrbitMinPercGrowth = 0.01f;
	_ssc.OrbitMaxPercGrowth = 0.05f;
	_ssc.OrbitMinPercOfStarRadius = 0.05f;
	_ssc.OrbitMaxPercOfStarRadius = 0.20f;
	_ssc.PlanetMinPercOfStarRadius = 0.20f;
	_ssc.PlanetMaxPercOfStarRadius = 0.40f;

	_isWorldGen = false;
	_isGreyScale = false;
	_isMoveWorld = false;

	float radius = GetInscribedCircleRadius(_sgc.WINDOW_WIDTH, _sgc.WINDOW_HEIGHT);
	sf::Vector2f center = sf::Vector2f(_sgc.WINDOW_WIDTH / 2.0f, _sgc.WINDOW_HEIGHT / 2.0f);
	_solarSystem.SetCenter(center);
	_solarSystem.SetMaxRadius(radius);
	_solarSystem.SetSSC(_ssc);
	_solarSystem.Init(_sgc);
	_solarSystem.GenerateSolarSystem(_engine);

	_world.Init(_sgc, sf::Vector2f(_sgc.WINDOW_WIDTH / 2, _sgc.WINDOW_HEIGHT / 2), 3);
	_world.Generate(_isGreyScale);

	_worldView.reset(sf::FloatRect(0, 0, 800, 800));
	_solarSystemView.reset(sf::FloatRect(0.0f, 0.0f, 800, 800));

	_mousePos = sf::Vector2i(0, 0);
	_prvMousePos = sf::Vector2i(0, 0);

	std::thread loadChunksThread(&Game::LoadChunks, this);

	_clock.restart();
	Loop();

	Pipe::SendMessage(MessageType::STOP, MessageDestination::LOADCHUNKS);
	loadChunksThread.join();
}

void Game::Loop()
{
	while (_window.isOpen())
	{
		Update();
		Draw();
	}
}

void Game::Update()
{
	PollEvents();

	_mousePos = sf::Mouse::getPosition(_window);

	MoveWorld();

	if (_isWorldGen)
	{
		Message message;
		if (Pipe::Listen(MessageDestination::GRAPHICS, message))
		{
			if (message.GetMessageType() == MessageType::LOADED)
			{
				_world.MergeStagingToChunks();
				_world.UnloadChunks();
			}
		}
	}

	if (_isWorldGen)
	{
		_window.setView(_worldView);
		_world.Update(_worldView.getCenter());
	}
	else
	{
		_window.setView(_solarSystemView);
		_solarSystem.Update(_mousePos, _clock.restart());
		_solarSystem.CreateTexture();
	}

	_window.clear(sf::Color::Black);

	if (_isWorldGen)
	{
		_world.Draw(&_window);
	}
	else
	{
		_solarSystem.Draw(&_window);
	}

	_window.display();
}

void Game::MoveWorld()
{
	if (_isWorldGen && _isMoveWorld)
	{
		float x = _prvMousePos.x - _mousePos.x;
		float y = _prvMousePos.y - _mousePos.y;
		_worldView.move(0.5*x, 0.5*y);

		_prvMousePos.x = _mousePos.x;
		_prvMousePos.y = _mousePos.y;

		if (_world.IsNewChunkInView(_worldView.getCenter()))
		{
			Pipe::SendMessage(MessageType::LOAD, MessageDestination::LOADCHUNKS);
		}
	}
}

void Game::PollEvents()
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			_window.close();
		}
		if (event.type == sf::Event::Resized)
		{
			if (_isWorldGen)
			{
				_worldView.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));
			}
			else
			{
				_solarSystemView.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));
			}
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::R)
			{
				if (_isWorldGen)
				{
					_world.Generate(_isGreyScale);
				}
				else
				{
					_solarSystem.GenerateSolarSystem(_engine);
				}
			}
			if (event.key.code == sf::Keyboard::S)
			{
				_isWorldGen = !_isWorldGen;
				if (!_isWorldGen)
				{
					if (!_solarSystem.IsGenerated())
					{
						_solarSystem.GenerateSolarSystem(_engine);
					}
				}
				else
				{
					if (!_world.IsGenerated())
					{
						_world.Generate(_isGreyScale);
					}
				}
			}
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			/*sf::Vector2f worldPos = _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			printf("x: %.0f y: %.0f\n", worldPos.x, worldPos.y);*/
			_prvMousePos.x = event.mouseButton.x;
			_prvMousePos.y = event.mouseButton.y;
			_isMoveWorld = true;

			sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
			if (!_isWorldGen && _solarSystem.IsPointWithinPlanet(mousePos))
			{
				_isWorldGen = true;
				if (!_world.IsGenerated())
				{
					_world.Generate(_isGreyScale);
				}
			}
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			/*sf::Vector2f worldPos = _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			printf("x: %.0f y: %.0f\n", worldPos.x, worldPos.y);*/
			_isMoveWorld = false;
		}
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (_isWorldGen)
			{
				if (event.mouseWheelScroll.delta > 0)
				{
					_worldView.zoom(0.5f);
				}
				else
				{
					_worldView.zoom(2.0f);
				}
			}
			else
			{
				if (event.mouseWheelScroll.delta > 0)
				{
					_solarSystemView.zoom(0.5f);
				}
				else
				{
					_solarSystemView.zoom(2.0f);
				}
			}
		}
	}
}

void Game::Draw()
{
	if (_isWorldGen)
	{
		_world.Draw(&_window);
	}
	else
	{
		_solarSystem.Draw(&_window);
	}
}

void Game::LoadChunks()
{
	bool isRunning = true;
	while (isRunning)
	{
		Message message;
		if (Pipe::Listen(MessageDestination::LOADCHUNKS, message))
		{
			switch (message.GetMessageType())
			{
				case MessageType::LOAD:
					_world.LoadChunksToStaging(_isGreyScale);
					Pipe::SendMessage(MessageType::LOADED, MessageDestination::GRAPHICS);
					break;
				case MessageType::STOP:
					isRunning = false;
					break;
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	printf("Load Chunks Thread Ending.");
}

