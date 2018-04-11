#pragma once
#include <SFML/Graphics.hpp>
#include "SGC.h"

class ProcGen
{
	public:
		ProcGen();
		void Init();

		void DrawTexure();
		const sf::Texture& GetTexture();

		void SetSGC(SGC sgc) { _sgc = sgc; }

	private:
		SGC _sgc;
		sf::RenderTexture _texture;
};