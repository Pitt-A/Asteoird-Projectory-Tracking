#include "Game.h"

Game::Game()
{
	std::srand(std::time(NULL));

	font.loadFromFile("Assets/pixelFont.ttf");

	asteroidInfo.setFillColor(sf::Color::White);
	asteroidInfo.setFont(font);
	asteroidInfo.setCharacterSize(16);
	asteroidInfo.setPosition(sf::Vector2f(windowWidth - 150, 10));

	planetInfo.setFillColor(sf::Color::White);
	planetInfo.setFont(font);
	planetInfo.setCharacterSize(16);
	planetInfo.setPosition(sf::Vector2f(10, 10));

	collisionAlert.setFillColor(sf::Color::White);
	collisionAlert.setFont(font);
	collisionAlert.setCharacterSize(16);
	collisionAlert.setPosition(sf::Vector2f(10, windowHeight - 30));

	SetComposition();
}

Game::~Game()
{
	delete m_window;
}

void Game::Execute()
{
	WindowSetup(false);
	view = m_window->getView();

	std::vector<GameObject*> _gameObjectList;
	std::vector<PlanetaryBody*> _planetaryBodyList;
	std::vector<Asteroid*> _asteroidList;

	NameGenerator* nameGen = new NameGenerator;
	std::string starName;

	starName = nameGen->CreateStarName();

	PlanetaryBody *starObject = new PlanetaryBody(true, NULL, 0.0003f);
	starObject->SetCircle(sf::Color(255, 127, 0, 255), 2.0f);
	starObject->SetPos(sf::Vector2f(windowWidth / 2.0f, windowHeight / 2.0f));
	starObject->SetName(starName);
	_gameObjectList.push_back(starObject);
	_planetaryBodyList.push_back(starObject);

	for (int i = 0; i < std::rand() % 5 + 4; i++)
	{
		GeneratePlanet(starObject, _gameObjectList, _planetaryBodyList, i, starName, nameGen);
	}

	GenerateAsteroid(_planetaryBodyList, _gameObjectList, _asteroidList, nameGen);

	while (m_window->isOpen())
	{
		CollisionDetection(_planetaryBodyList, _asteroidList, _gameObjectList);

		if (asteroidToDelete >= 0 || gameobjectToDelete >= 0)
		{
			delete _asteroidList.at(asteroidToDelete);
			_asteroidList.erase(_asteroidList.begin() + asteroidToDelete);
			_gameObjectList.erase(_gameObjectList.begin() + gameobjectToDelete);
			asteroidToDelete = -1;
			gameobjectToDelete = -1;
		}

		view = m_window->getView();
		while (m_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				m_window->close();
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				if (event.mouseWheel.delta == 1)
				{
					view.zoom(0.5f);
					viewZoom *= 0.5f;
				}
				if (event.mouseWheel.delta == -1 && viewZoom < 16)
				{
					view.zoom(2.0f);
					viewZoom *= 2.0f;
				}

				for(int i = 0; i < _asteroidList.size(); i++)
				{
					_asteroidList.at(i)->UpdateZoom(viewZoom);
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					isPanning = true;
					panningStart = sf::Vector2f(sf::Mouse::getPosition());
				}
				else if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (_asteroidList.size() == 0 || _asteroidList.at(_asteroidList.size() - 1)->GetState() == AsteroidState::AS_SIM && _asteroidList.size() <= 12)
					{
						GenerateAsteroid(_planetaryBodyList, _gameObjectList, _asteroidList, nameGen);
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					isPanning = false;
				}
			}
		}

		if (isPanning)
		{
			CameraPan();
		}

		m_window->setView(view);
		m_window->clear(sf::Color(0, 0, 0, 0));
		for (int i = 0; i < _gameObjectList.size(); i++)
		{
			_gameObjectList.at(i)->Update();
			m_window->draw(_gameObjectList.at(i)->Draw());
		}
		for (int i = 0; i < _asteroidList.size(); i++)
		{
			m_window->draw(_asteroidList.at(i)->GetTrail());
			m_window->draw(_asteroidList.at(i)->GetAimLine());
		}
		for (int i = 0; i < _planetaryBodyList.size(); i++)
		{
			m_window->draw(_planetaryBodyList.at(i)->GetTrail());
			m_window->draw(_planetaryBodyList.at(i)->GetAtmosphere());
		}
		DisplayPlanetaryBodyNames(_planetaryBodyList, m_window, viewZoom, _asteroidList);

		m_window->setView(originalView);
		m_window->draw(asteroidInfo);
		m_window->draw(planetInfo);
		m_window->draw(collisionAlert);
		DisplayAsteroidInfo(_asteroidList, m_window, _planetaryBodyList);
		m_window->setView(view);
		
		m_window->display();
	}

	//No need to clear other vectors out as all pointers are added to _gameObjectList
	for (int i = 0; i < _gameObjectList.size(); i++)
	{
		if (_gameObjectList.at(i) != NULL)
		{
			delete _gameObjectList.at(i);
		}
	}
	return;
}

void Game::WindowSetup(bool _fullscreen)
{
	if (_fullscreen)
	{
		m_window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Asteroid Simulator", sf::Style::None);
		sf::Vector2u tempVec = m_window->getSize();
	}
	else
	{
		m_window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Asteroid Simulator");
	}
	
	m_window->setFramerateLimit(60);
	m_window->setVerticalSyncEnabled(true);
	originalView = m_window->getView();
}

void Game::GeneratePlanet(PlanetaryBody* _target, std::vector<GameObject*>& _gameObjectList, std::vector<PlanetaryBody*>& _planetaryBodyList,
	int& _planetNo, std::string _starName, NameGenerator* _nameGen)
{
	float planetScale = rand() % 300 + 70;
	planetScale = planetScale / 100;
	float planetXSpeed = rand() % 5;
	float planetYSpeed = -(rand() % 20 + 10) * ((_planetNo + 2) / 2);

	int index = rand() % atmosphereElementName.size();

	PlanetaryBody *planetObject = new PlanetaryBody(false, _target, 0.00001f);
	int tempColorValue = RandomRGBValue();
	planetObject->SetCircle(sf::Color(tempColorValue, tempColorValue / 2, 0, 255), planetScale);
	planetObject->SetPos(sf::Vector2f((windowWidth / 2.0f) - distance, (windowHeight / 2.0f) + std::rand() % 2000 + 50));
	distance += std::rand() % 500 + 2000;
	planetObject->SetSpeed(planetXSpeed, planetYSpeed);
	planetObject->SetTrailColor(sf::Color(100, 100, 100, 255));
	int randAtmos = rand() % 100 + 1;
	planetObject->SetAtmosphere(sf::Color(atmosphereColour.at(index).r, atmosphereColour.at(index).g, atmosphereColour.at(index).b, randAtmos), randAtmos);
	planetObject->SetAtmosphereName(atmosphereElementName.at(index));
	planetObject->SetTrailComplete(true);
	std::string tempPlanetName = _nameGen->CreatePlanetName(_starName, _planetNo);
	planetObject->SetName(tempPlanetName);
	_gameObjectList.push_back(planetObject);
	_planetaryBodyList.push_back(planetObject);
}

void Game::GenerateAsteroid(std::vector<PlanetaryBody*> _planetList, std::vector<GameObject*>& _gameObjectList, std::vector<Asteroid*>& _asteroidList, NameGenerator* _nameGen)
{
	Asteroid *asteroidObject = new Asteroid(m_window, _planetList);
	asteroidObject->SetCircle(sf::Color(120, 120, 120, 255), 0.5f);
	asteroidObject->UpdateZoom(viewZoom);
	asteroidObject->SetSpeed(3.0f, -10.0f);
	asteroidObject->SetTrailComplete(false);
	asteroidObject->SetTrailColor(sf::Color(0, 150, 0, 255));
	asteroidObject->SetName(_nameGen->CreateAsteroidName());
	_gameObjectList.push_back(asteroidObject);
	_asteroidList.push_back(asteroidObject);
}

void Game::DisplayAsteroidInfo(std::vector<Asteroid*> _asteroids, sf::RenderWindow* _window, std::vector<PlanetaryBody*> _planets)
{
	std::string asInfo = "ASTEROIDS VELOCITY\n\n";
	for (int i = 0; i < _asteroids.size(); i++)
	{
		int tempX = _asteroids.at(i)->GetSpeedX();
		int tempY = _asteroids.at(i)->GetSpeedY();
		asInfo += _asteroids.at(i)->GetName() + "'s Speed\nX: " + std::to_string(tempX) + " km/s\nY: " + std::to_string(tempY) + " km/s\n\n";
	}
	asteroidInfo.setString(asInfo);

	std::string plInfo = "The [" + _planets.at(0)->GetName() + "] System\n\n";	
	for (int i = 1; i < _planets.size(); i++)
	{
		plInfo += _planets.at(i)->GetName() + "\n" + "Atmosphere: " + _planets.at(i)->GetAtmosphereName() + "\nStrength: " + std::to_string(_planets.at(i)->GetThickness()) + "%\n\n";
	}
	planetInfo.setString(plInfo);

	collisionAlert.setString(lastCollision);
}

void Game::DisplayPlanetaryBodyNames(std::vector<PlanetaryBody*> _planetList, sf::RenderWindow* _window, float _zoomView, std::vector<Asteroid*> _asteroidList)
{
	sf::Text planetBodyText;
	planetBodyText.setFont(font);
	planetBodyText.setCharacterSize(16 * _zoomView);

	for (int i = 0; i < _planetList.size(); i++)
	{
		planetBodyText.setString(_planetList.at(i)->GetName());
		planetBodyText.setPosition(_planetList.at(i)->GetPos());
		_window->draw(planetBodyText);
	}
	for (int i = 0; i < _asteroidList.size(); i++)
	{
		planetBodyText.setString(_asteroidList.at(i)->GetName());
		planetBodyText.setPosition(_asteroidList.at(i)->GetPos());
		_window->draw(planetBodyText);
	}
}

void Game::CameraPan()
{
	sf::Vector2f tempMousePos = sf::Vector2f(sf::Mouse::getPosition().x - panningStart.x, sf::Mouse::getPosition().y - panningStart.y);
	view.move(-1.0f * tempMousePos * viewZoom);
	panningStart = sf::Vector2f(sf::Mouse::getPosition());
}

float Game::RandomRGBValue()
{
	return rand() % 254 + 1;
}

void Game::CollisionDetection(std::vector<PlanetaryBody*> _planetList, std::vector<Asteroid*> _asteroidList, std::vector<GameObject*> _gameObjectList)
{
	
	for (int i = 0; i < _asteroidList.size(); i++)
	{
		for (int j = 0; j < _planetList.size(); j++)
		{
			sf::Vector2f tempDist = _asteroidList.at(i)->GetPos() - _planetList.at(j)->GetPos();
			float minimumDistance = (_asteroidList.at(i)->GetRadius() * _asteroidList.at(i)->GetScale()) + (_planetList.at(j)->GetRadius() * _planetList.at(j)->GetScale());
			if ((tempDist.x * tempDist.x) + (tempDist.y * tempDist.y) < minimumDistance * minimumDistance && _asteroidList.at(i)->GetState() == AsteroidState::AS_SIM)
			{
				for (int k = 0; k < _gameObjectList.size(); k++)
				{
					if (_gameObjectList.at(k) == _asteroidList.at(i))
					{
						lastCollision = "Asteroid [" + _asteroidList.at(i)->GetName() + "] has collided with [" + _planetList.at(j)->GetName() + "].";
						asteroidToDelete = i;
						gameobjectToDelete = k;
					}
				}
			}
		}
	}
}

void Game::SetComposition()
{
	atmosphereElementName.push_back("Oxygen");
	atmosphereElementName.push_back("Argon");
	atmosphereElementName.push_back("Sulfuric Acid");
	atmosphereElementName.push_back("Ammonia");
	atmosphereElementName.push_back("Ammonia Hydrosulfide");
	atmosphereElementName.push_back("Methane");
	atmosphereElementName.push_back("Hydrogen");
	atmosphereElementName.push_back("Nitrogen");
	atmosphereElementName.push_back("Carbon Dioxide");
	atmosphereElementName.push_back("Chlorine");
	atmosphereElementName.push_back("Ozone");
	atmosphereElementName.push_back("Nitrogen Dioxide");
	atmosphereElementName.push_back("Flourine");
	atmosphereElementName.push_back("Iodine");

	atmosphereColour.push_back(sf::Color(170, 225, 240, 255));
	atmosphereColour.push_back(sf::Color(255, 255, 255, 255));
	atmosphereColour.push_back(sf::Color(255, 210, 35, 255));
	atmosphereColour.push_back(sf::Color(255, 255, 255, 255));
	atmosphereColour.push_back(sf::Color(255, 127, 39, 255));
	atmosphereColour.push_back(sf::Color(140, 210, 255, 255));
	atmosphereColour.push_back(sf::Color(255, 255, 255, 255));
	atmosphereColour.push_back(sf::Color(255, 255, 255, 255));
	atmosphereColour.push_back(sf::Color(255, 255, 255, 255));
	atmosphereColour.push_back(sf::Color(65, 255, 100, 255));
	atmosphereColour.push_back(sf::Color(65, 225, 255, 255));
	atmosphereColour.push_back(sf::Color(255, 45, 20, 255));
	atmosphereColour.push_back(sf::Color(200, 100, 0, 255));
	atmosphereColour.push_back(sf::Color(128, 0, 255, 255));
}