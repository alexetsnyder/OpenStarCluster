#include "AstroObject.h"

//AstroObject Base Class
AstroObject::AstroObject()
{

}

AstroObject::AstroObject(sf::Vector2f position, float radius)
{
	_body.setPosition(position);
	_body.setRadius(radius);
}

AstroObject::AstroObject(sf::Vector2f position, float radius, sf::Color color)
{
	_body.setPosition(position);
	_body.setRadius(radius);
	_body.setFillColor(color);
}
//End of AstroObject

//Planet Class
Planet::Planet()
{

}

Planet::Planet(sf::Vector2f position, float radius, sf::Color color)
	: AstroObject(position, radius, color)
{

}

void Planet::CalculateNewPosition()
{
	sf::Time time = GetSGC().TIME.getElapsedTime();
	_orbit.CalculateNextOrbitAngle(time);
	SetPosition(_orbit.GetOrbitPosition());
}

void Planet::CalculatePosition(sf::Vector2f orbitPos, float orbitRadius)
{
	_orbit.SetRadius(orbitRadius);
	_orbit.SetPosition(sf::Vector2f(orbitPos.x, orbitPos.y));
	_orbit.SetVelocity(0.00005f);
	sf::Vector2f planetPos = _orbit.GetOrbitPosition();
	float xPos = planetPos.x - GetRadius();
	float yPos = planetPos.y - GetRadius();
	SetPosition(sf::Vector2f(xPos, yPos));
}
//End Planet Class

//Star Class
Star::Star()
{

}

Star::Star(sf::Vector2f position, float radius, sf::Color color)
	: AstroObject(position, radius, color)
{

}
//End Star Class