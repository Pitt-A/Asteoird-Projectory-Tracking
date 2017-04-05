#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

#include "GameObject.h"
#include "PlanetaryBody.h"
#include "Asteroid.h"
#include "NameGenerator.h"

class Game
{
public:
	Game();
	~Game();
	void Execute();
	void GeneratePlanet(PlanetaryBody*, std::vector<GameObject*>&, std::vector<PlanetaryBody*>&, int&, std::string, NameGenerator*);
	void GenerateAsteroid(std::vector<PlanetaryBody*>, std::vector<GameObject*>&, std::vector<Asteroid*>&, NameGenerator*);
	void DisplayAsteroidInfo(std::vector<Asteroid*>, sf::RenderWindow*, std::vector<PlanetaryBody*>);
	void DisplayPlanetaryBodyNames(std::vector<PlanetaryBody*>, sf::RenderWindow*, float, std::vector<Asteroid*>);
	void CameraPan();
	float RandomRGBValue();
	void CollisionDetection(std::vector<PlanetaryBody*>, std::vector<Asteroid*>, std::vector<GameObject*>);
	void SetComposition();
private:
	sf::Vector2f panningStart;
	sf::View view;
	sf::View originalView;
	sf::Event event;
	void WindowSetup(bool);
	sf::RenderWindow *m_window;
	const int windowWidth = 1440, windowHeight = 900;
	int distance = 2000;
	bool isPanning = false;
	float viewZoom = 1.0f;
	sf::Text asteroidInfo;
	sf::Text planetInfo;
	sf::Text collisionAlert;
	sf::Font font;
	int asteroidToDelete = -1;
	int gameobjectToDelete = -1;
	std::vector<std::string> atmosphereElementName;
	std::vector<sf::Color> atmosphereColour;
	std::string lastCollision;
};