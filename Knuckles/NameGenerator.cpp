#include "NameGenerator.h"

NameGenerator::NameGenerator()
{

}

NameGenerator::~NameGenerator()
{

}

std::string NameGenerator::CreatePlanetName(std::string _name, int _number)
{
	_name = _name + characters[_number];

	return _name;
}

std::string NameGenerator::CreateStarName()
{
	int randomInt = rand() % 298 + 1;
	int chosenName = rand() % planetNames.size();
	std::string generatedName = planetNames[chosenName] + "-";

	std::string number = std::to_string(randomInt);

	if (randomInt < 100)
	{
		number = std::to_string(zero) + number;
		if (randomInt < 10)
		{
			number = std::to_string(zero) + number;
		}
	}

	generatedName = generatedName + number;

	return generatedName;
}

std::string NameGenerator::CreateAsteroidName()
{
	int nameChosen = rand() % asteroidNames.size();
	int randomInt = rand() % 330000;
	std::string asteroidName = std::to_string(randomInt) + " " + asteroidNames[nameChosen];
	return asteroidName;
}
