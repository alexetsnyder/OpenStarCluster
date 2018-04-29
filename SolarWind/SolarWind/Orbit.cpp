#include "Orbit.h"
#include <cmath>
#include <random>

#define PI 3.14159265f

Orbit::Orbit()
{
	
}

Orbit::Orbit(SGC sgc, sf::Vector2f position, float radius, sf::Color color)
{
	_orbit.setPosition(position);
	_orbit.setOutlineColor(color);
	_orbit.setRadius(radius);
	Init(sgc);
}

void Orbit::Init(SGC sgc)
{
	_sgc = sgc;
	_degreeUID = std::uniform_int_distribution<int>(0, 360);
	_orbit.setPointCount(100);
	_orbit.setFillColor(sf::Color::Transparent);
	_orbit.setOutlineThickness(-1.0f);	

}

void Orbit::GenerateRandomVelocity(std::mt19937& engine)
{
	std::uniform_real_distribution<float> velocities(0.009f, 0.1f);
	SetVelocity(velocities(engine));
}

void Orbit::GenerateRandomOrbitAngle(std::mt19937& engine)
{
	_angle = _degreeUID(engine) * (2 * PI / 360.0f);
	//_angle = PI / 2.0f;
}

void Orbit::CalculateNextOrbitAngle(sf::Time elapsed)
{
	_angle += _velocity * elapsed.asSeconds();
}

sf::Vector2f Orbit::GetOrbitPosition()
{
	sf::Vector2f orbitPos = GetPosition();
	float xPos = GetRadius() * std::cos(_angle) + orbitPos.x + GetRadius();
	float yPos = GetRadius() * std::sin(_angle) + orbitPos.y + GetRadius();
	return sf::Vector2f(xPos, yPos);
}

void Orbit::Draw(sf::RenderTarget* target)
{
	target->draw(_orbit);
}