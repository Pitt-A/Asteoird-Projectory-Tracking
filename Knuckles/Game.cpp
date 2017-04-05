#include "Game.h"

Game::Game()
{
	std::srand(std::time(NULL)); //Seed the random number generator

	font.loadFromFile("Assets/pixelFont.ttf"); //Load the font

	asteroidInfo.setFillColor(sf::Color::White); //Setup the asteroidInfo text
	asteroidInfo.setFont(font);
	asteroidInfo.setCharacterSize(16);
	asteroidInfo.setPosition(sf::Vector2f(windowWidth - 150, 10));

	planetInfo.setFillColor(sf::Color::White); //Setup the planetInfo text
	planetInfo.setFont(font);
	planetInfo.setCharacterSize(16);
	planetInfo.setPosition(sf::Vector2f(10, 10));

	collisionAlert.setFillColor(sf::Color::White); //Setup the collisionAlert text
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
	WindowSetup(false); //Create a new window
	view = m_window->getView();

	//Create the three lists to contain the different gameObjects
	std::vector<GameObject*> _gameObjectList;
	std::vector<PlanetaryBody*> _planetaryBodyList;
	std::vector<Asteroid*> _asteroidList;

	NameGenerator* nameGen = new NameGenerator; //Creating a new NameGenerator
	std::string starName;

	starName = nameGen->CreateStarName(); //Generate the star's name

	PlanetaryBody *starObject = new PlanetaryBody(true, NULL, 0.0003f); //Create the central star
	starObject->SetCircle(sf::Color(255, 127, 0, 255), 2.0f);
	starObject->SetPos(sf::Vector2f(windowWidth / 2.0f, windowHeight / 2.0f));
	starObject->SetName(starName);
	_gameObjectList.push_back(starObject);
	_planetaryBodyList.push_back(starObject);

	for (int i = 0; i < std::rand() % 5 + 4; i++) //Generate a random number of planets
	{
		GeneratePlanet(starObject, _gameObjectList, _planetaryBodyList, i, starName, nameGen);
	}

	GenerateAsteroid(_planetaryBodyList, _gameObjectList, _asteroidList, nameGen);

	while (m_window->isOpen()) //Only exit when closing window
	{
		CollisionDetection(_planetaryBodyList, _asteroidList, _gameObjectList);

		if (asteroidToDelete >= 0 || gameobjectToDelete >= 0) //Handling the deletion of the asteroid upon collision (roundabout way, hence why it's here instead of CollisionDetection())
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
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) //Only exit when closing window
			{
				m_window->close();
			}
			else if (event.type == sf::Event::MouseWheelMoved) //Handling the zooming in/out
			{
				if (event.mouseWheel.delta == 1)
				{
					view.zoom(0.5f);
					viewZoom *= 0.5f;
				}
				if (event.mouseWheel.delta == -1 && viewZoom < 32)
				{
					view.zoom(2.0f);
					viewZoom *= 2.0f;
				}

				for(int i = 0; i < _asteroidList.size(); i++)
				{
					_asteroidList.at(i)->UpdateZoom(viewZoom); //Updating the zoom level for asteroid placement
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) //Main mouse button check
			{
				if (event.mouseButton.button == sf::Mouse::Right) //RMB for panning
				{
					isPanning = true;
					panningStart = sf::Vector2f(sf::Mouse::getPosition());
				}
				else if (event.mouseButton.button == sf::Mouse::Left) //LMB for placing asteroid
				{
					if (_asteroidList.size() == 0 || _asteroidList.at(_asteroidList.size() - 1)->GetState() == AsteroidState::AS_SIM && _asteroidList.size() <= 12)
					{
						GenerateAsteroid(_planetaryBodyList, _gameObjectList, _asteroidList, nameGen);
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Right) //For turning the panning off when not right clicking
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
		m_window->clear(sf::Color(0, 0, 0, 0)); //Background colour (black)
		for (int i = 0; i < _gameObjectList.size(); i++) //Add all gameObjects to the frame buffer and update
		{
			_gameObjectList.at(i)->Update();
			m_window->draw(_gameObjectList.at(i)->Draw());
		}
		for (int i = 0; i < _asteroidList.size(); i++) //Add all trails and aimlines to the draw buffer
		{
			m_window->draw(_asteroidList.at(i)->GetTrail());
			m_window->draw(_asteroidList.at(i)->GetAimLine());
		}
		for (int i = 0; i < _planetaryBodyList.size(); i++) //Add all trails and atmospheres to the draw buffer
		{
			m_window->draw(_planetaryBodyList.at(i)->GetTrail());
			m_window->draw(_planetaryBodyList.at(i)->GetAtmosphere());
		}
		DisplayPlanetaryBodyNames(_planetaryBodyList, m_window, viewZoom, _asteroidList); //Update the information displayed on screen

		m_window->setView(originalView); //Swap the current view out with the original, to draw the info in the same spot each time without updating zoom and positions
		m_window->draw(asteroidInfo); //Add the info's to the draw buffer
		m_window->draw(planetInfo);
		m_window->draw(collisionAlert);
		DisplayAsteroidInfo(_asteroidList, m_window, _planetaryBodyList); //Update the information displayed on screen
		m_window->setView(view); //Set the view back to the proper view to be rendered
		
		m_window->display(); //Draw the frame buffer to screen
	}

	//No need to clear other vectors out as all pointers are added to _gameObjectList
	for (int i = 0; i < _gameObjectList.size(); i++)
	{
		if (_gameObjectList.at(i) != NULL)
		{
			delete _gameObjectList.at(i);
		}
	}
	delete nameGen;
	return;
}

void Game::WindowSetup(bool _fullscreen) //Setting up the window for drawing
{
	if (_fullscreen) //Fullscreen mode, only accessible through code
	{
		m_window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Asteroid Simulator", sf::Style::None);
		sf::Vector2u tempVec = m_window->getSize();
	}
	else //(1440 x 900) mode, only present mode in final build
	{
		m_window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Asteroid Simulator");
	}
	
	m_window->setFramerateLimit(60); //Framerate limit, included to prevent stuttering
	m_window->setVerticalSyncEnabled(true); //VSync, included to prevent tearing
	originalView = m_window->getView(); //Setting up the original view for UI rendering
}

void Game::GeneratePlanet(PlanetaryBody* _target, std::vector<GameObject*>& _gameObjectList, std::vector<PlanetaryBody*>& _planetaryBodyList,
	int& _planetNo, std::string _starName, NameGenerator* _nameGen) //Creating a new planet
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

void Game::GenerateAsteroid(std::vector<PlanetaryBody*> _planetList, std::vector<GameObject*>& _gameObjectList, std::vector<Asteroid*>& _asteroidList, NameGenerator* _nameGen) //Creating a new asteroid
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

void Game::DisplayAsteroidInfo(std::vector<Asteroid*> _asteroids, sf::RenderWindow* _window, std::vector<PlanetaryBody*> _planets) //Displaying the info of asteroids and star system (name outdated)S
{
	std::string asInfo = "ASTEROIDS VELOCITY\n\n";
	for (int i = 0; i < _asteroids.size(); i++) //For loop adding an entry for each asteroid on the right side of the screen
	{
		int tempX = _asteroids.at(i)->GetSpeedX();
		int tempY = _asteroids.at(i)->GetSpeedY();
		asInfo += _asteroids.at(i)->GetName() + "'s Speed\nX: " + std::to_string(tempX) + " km/s\nY: " + std::to_string(tempY) + " km/s\n\n";
	}
	asteroidInfo.setString(asInfo);

	std::string plInfo = "The [" + _planets.at(0)->GetName() + "] System\n\n";	
	for (int i = 1; i < _planets.size(); i++) //For loop adding an entry for each planet on the left side of the screen
	{
		plInfo += _planets.at(i)->GetName() + "\n" + "Atmosphere: " + _planets.at(i)->GetAtmosphereName() + "\nStrength: " + std::to_string(_planets.at(i)->GetThickness()) + "%\n\n";
	}
	planetInfo.setString(plInfo);

	collisionAlert.setString(lastCollision); //Updating the collision alert with information on the last collision (see CollisionDetection())
}

void Game::DisplayPlanetaryBodyNames(std::vector<PlanetaryBody*> _planetList, sf::RenderWindow* _window, float _zoomView, std::vector<Asteroid*> _asteroidList) //Displaying the names of each planet, star and asteroid
{
	sf::Text planetBodyText;
	planetBodyText.setFont(font);
	planetBodyText.setCharacterSize(16 * _zoomView); //Set character size relative to the view's zoom

	for (int i = 0; i < _planetList.size(); i++) //Planet list includes star, so no need to include star manually
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

void Game::CameraPan() //Moving the camera when holding right-click
{
	sf::Vector2f tempMousePos = sf::Vector2f(sf::Mouse::getPosition().x - panningStart.x, sf::Mouse::getPosition().y - panningStart.y);
	view.move(-1.0f * tempMousePos * viewZoom);
	panningStart = sf::Vector2f(sf::Mouse::getPosition());
}

float Game::RandomRGBValue() //Calculating a random value between 0 and 255, for use in a sf::color
{
	return rand() % 254 + 1;
}

void Game::CollisionDetection(std::vector<PlanetaryBody*> _planetList, std::vector<Asteroid*> _asteroidList, std::vector<GameObject*> _gameObjectList) //Detecting collisions for asteroids
{
	for (int i = 0; i < _asteroidList.size(); i++)
	{
		for (int j = 0; j < _planetList.size(); j++)
		{
			//Loop through asteroid and planet vectors and check their distances to see if it's less than their total combined radii
			//If true, delete the asteroid
			sf::Vector2f tempDist = _asteroidList.at(i)->GetPos() - _planetList.at(j)->GetPos();
			float minimumDistance = (_asteroidList.at(i)->GetRadius() * _asteroidList.at(i)->GetScale()) + (_planetList.at(j)->GetRadius() * _planetList.at(j)->GetScale());
			if ((tempDist.x * tempDist.x) + (tempDist.y * tempDist.y) < minimumDistance * minimumDistance && _asteroidList.at(i)->GetState() == AsteroidState::AS_SIM)
			{
				for (int k = 0; k < _gameObjectList.size(); k++)
				{
					if (_gameObjectList.at(k) == _asteroidList.at(i))
					{
						//Had to delete the pointer AND erase from the vectors, so searched for identical entries in each vector and marked them for removal
						lastCollision = "Asteroid [" + _asteroidList.at(i)->GetName() + "] has collided with [" + _planetList.at(j)->GetName() + "].";
						asteroidToDelete = i;
						gameobjectToDelete = k;
					}
				}
			}
		}
	}
}

void Game::SetComposition() //Function for populating the elements and color vectors
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