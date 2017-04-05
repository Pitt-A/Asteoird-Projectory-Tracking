#include "Asteroid.h"

Asteroid::Asteroid(sf::RenderWindow* _window, std::vector<PlanetaryBody*> _list) //Pass the pointer to the window and the vector of pointers to the objects that will pull on thje asteroid
{
	SetTrailColor(sf::Color::Green);
	window = _window;
	aimLine.setPrimitiveType(sf::LinesStrip); //Cause the vertex array to draw lines
	for (int i = 0; i < 3; i++) //Add three points to the array (which is two lines)
	{
		aimLine.append(sf::Vector2f(i, 0));
	}
	m_planetaryBodyList = _list;
	
	sf::Vector2f tempPos = window->mapPixelToCoords(sf::Mouse::getPosition() - window->getPosition());
	tempPos.x -= 8 * zoomLevel;
	tempPos.y -= 31 * zoomLevel;
	SetPos(tempPos);
}

Asteroid::~Asteroid()
{
	lines.clear(); //Clear the vertex array out when destroying
}

void Asteroid::Update()
{
	switch (AState) //The state manager of the asteroid
	{
	case AsteroidState::AS_PLACE:
		PlaceUpdate();
		break;
	case AsteroidState::AS_AIM:
		AimUpdate();
		break;
	case AsteroidState::AS_SIM:
		SimUpdate();
		break;
	}
	//A small time delay on changing states to prevent the placing and firing of the asteroid occuring on the same frame
	if (AState == AsteroidState::AS_AIM && clock.getElapsedTime() > sf::Time(sf::seconds(0.1f)))
	{
		timer = true;
	}
}

void Asteroid::PlaceUpdate() //The update function for when the asteroid is going to be placed
{
	sf::Vector2f tempPos = window->mapPixelToCoords(sf::Mouse::getPosition() - window->getPosition());
	tempPos.x -= 8 * zoomLevel;
	tempPos.y -= 31 * zoomLevel;
	SetPos(tempPos);
	clock.restart();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		AState = AsteroidState::AS_AIM;
	}
}

void Asteroid::AimUpdate() //The update function for when the asteroid is going to be fired
{
	sf::Vector2f mouseLocation = window->mapPixelToCoords(sf::Mouse::getPosition() - window->getPosition()); //Getting the mouse position in world coords
	//Setting the three vertexs to their positions, 0 = mouse, 1 = asteroid, 2 = away from mouse
	aimLine[0].position = mouseLocation + windowDrawOffset * zoomLevel;
	aimLine[1].position = GetPos();
	aimLine[2].position = (GetPos() + (GetPos() - mouseLocation)) - windowDrawOffset * zoomLevel;

	//Ensure all vetices are coloured red
	for (int i = 0; i < aimLine.getVertexCount(); i++)
	{
		aimLine[i].color = sf::Color::Red;
	}

	//If the user presses left mouse button, move to next state
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && timer == true)
	{
		AState = AsteroidState::AS_SIM;
		m_speedX = (aimLine[2].position.x - mouseLocation.x) / 10;
		m_speedY = (aimLine[2].position.y - mouseLocation.y) / 10;
		aimLine.clear();
	}
}

void Asteroid::SimUpdate() //The update function for when the asteroid is moving through space
{
	for (int i = 0; i < m_planetaryBodyList.size(); i++) //Loop through all stars and planets and adjust velocity accordingly
	{
		sf::Vector2f gravPoint = m_planetaryBodyList.at(i)->GetPos(); //Get object position
		float otherGravConst = m_planetaryBodyList.at(i)->GetGravConst(); //Get object gravitationl constant

		//A = -G*(M1 - M2) where A is acceleration, G is the gravitational constant, M1 is current position and M2 is other object position
		m_accX = -otherGravConst * (currentPos.x - gravPoint.x);
		m_accY = -otherGravConst * (currentPos.y - gravPoint.y);

		m_speedX += m_accX;
		m_speedY += m_accY;
	}
	MoveSprite(sf::Vector2f(m_speedX, m_speedY));
	TrailRenderer(); //Render the trail
}

void Asteroid::UpdateZoom(float _zoomLevel) //Used to update the position in relation to the zoom level
{
	zoomLevel = _zoomLevel;
}

sf::VertexArray Asteroid::GetAimLine() //Return the vertex array for drawing
{
	return aimLine;
}

AsteroidState Asteroid::GetState() //Return the current state for checking
{
	return AState;
}
