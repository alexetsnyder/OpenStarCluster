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

bool AstroObject::IsWithin(sf::Vector2i mousePos)
{
	sf::Vector2f circlePos = GetPosition();
	sf::Vector2f circleCenter = sf::Vector2f(circlePos.x + GetRadius() , circlePos.y + GetRadius());
	sf::Vector2f diff = sf::Vector2f(mousePos.x - circleCenter.x, mousePos.y - circleCenter.y);
	/*printf("CircleCenter.x: %f, CircleCenter.y: %f\n", circleCenter.x, circleCenter.y);
	printf("mousePos.x: %d, mousePos.y: %d\n", mousePos.x, mousePos.y);
	printf("diff.x: %f, diff.y: %f\n", diff.x, diff.y);
	printf("Radius Squared: %f\n", GetRadius() * GetRadius());
	printf("diff Length: %f\n\n", diff.x * diff.x + diff.y * diff.y);*/
	return GetRadius() * GetRadius() > diff.x * diff.x + diff.y * diff.y;
}

void AstroObject::Draw(sf::RenderTarget* target)
{
	target->draw(_body);
}
//End of AstroObject

//Planet Class
NameGenerator Planet::planetNameGen;

Planet::Planet()
{

}

Planet::Planet(sf::Vector2f position, float radius, sf::Color color)
	: AstroObject(position, radius, color)
{
	Init();
}

void Planet::Init()
{
	GenerateName();	
}

void Planet::CreateOrbit(std::mt19937 engine, sf::Vector2f orbitPos, float orbitRadius)
{
	_orbit.Init(GetSGC());
	_orbit.SetRadius(orbitRadius);
	_orbit.SetPosition(sf::Vector2f(orbitPos.x, orbitPos.y));
	_orbit.SetVelocity(0.00005f);
	_orbit.CalculateOrbitAngle(engine);
}

void Planet::UpdateOrbit()
{
	sf::Time time = GetSGC().TIME.getElapsedTime();
	_orbit.CalculateNextOrbitAngle(time);
}

void Planet::CalculatePosition()
{
	sf::Vector2f planetPos = _orbit.GetOrbitPosition();
	float xPos = planetPos.x - GetRadius();
	float yPos = planetPos.y - GetRadius();
	SetPosition(sf::Vector2f(xPos, yPos));
}

void Planet::GenerateName()
{
	if (!planetNameGen.IsReady())
	{
		planetNameGen.Init(3);
		planetNameGen.LoadDataFromFile("Names\\exoplanet_names.txt");
	}

	std::string planetName;
	while (planetNameGen.IsReady() && !planetNameGen.GenerateWord(6, 12, planetName));
	IsNewName() = planetNameGen.IsNewName(planetName);
	SetName(planetName);
	printf("%s %s\n", (IsNewName() ? "New:" : "Not:"), planetName.c_str());
}

void Planet::Draw(sf::RenderTarget* target)
{
	_orbit.Draw(target);
	AstroObject::Draw(target);
}
//End Planet Class

//Star Class
NameGenerator Star::starNameGen;

Star::Star()
{
	
}

Star::Star(sf::Vector2f position, float radius, sf::Color color)
	: AstroObject(position, radius, color)
{
	Init();
}

void Star::Init()
{
	GenerateName();
}

void Star::GenerateName()
{
	if (!starNameGen.IsReady())
	{
		starNameGen.Init(3);
		starNameGen.LoadDataFromFile("Names\\star_names.txt");
	}

	std::string starName;
	while (starNameGen.IsReady() && !starNameGen.GenerateWord(6, 12, starName));
	IsNewName() = starNameGen.IsNewName(starName);
	SetName(starName);
	printf("%s %s\n\n", IsNewName() ? "New:" : "Not:", starName.c_str());
}
//End Star Class