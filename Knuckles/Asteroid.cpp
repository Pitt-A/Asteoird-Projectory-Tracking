#include "Asteroid.h"

Asteroid::Asteroid(sf::RenderWindow* _window, std::vector<PlanetaryBody*> _list)
{
	SetTrailColor(sf::Color::Green);
	window = _window;
	aimLine.setPrimitiveType(sf::LinesStrip);
	for (int i = 0; i < 3; i++)
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
	lines.clear();
}

void Asteroid::Update()
{
	switch (AState)
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
	case AsteroidState::AS_DESTROY:
		DestroyUpdate();
		break;
	}

	if (AState == AsteroidState::AS_AIM && clock.getElapsedTime() > sf::Time(sf::seconds(0.1f)))
	{
		timer = true;
	}
}

void Asteroid::PlaceUpdate()
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

void Asteroid::AimUpdate()
{
	sf::Vector2f mouseLocation = window->mapPixelToCoords(sf::Mouse::getPosition() - window->getPosition());

	aimLine[0].position = mouseLocation + windowDrawOffset * zoomLevel;
	aimLine[1].position = GetPos();
	aimLine[2].position = (GetPos() + (GetPos() - mouseLocation)) - windowDrawOffset * zoomLevel;

	for (int i = 0; i < aimLine.getVertexCount(); i++)
	{
		aimLine[i].color = sf::Color::Red;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && timer == true)
	{
		AState = AsteroidState::AS_SIM;
		m_speedX = (aimLine[2].position.x - mouseLocation.x) / 10;
		m_speedY = (aimLine[2].position.y - mouseLocation.y) / 10;
		aimLine.clear();
	}
}

void Asteroid::SimUpdate()
{
	for (int i = 0; i < m_planetaryBodyList.size(); i++)
	{
		sf::Vector2f gravPoint = m_planetaryBodyList.at(i)->GetPos();
		float otherGravConst = m_planetaryBodyList.at(i)->GetGravConst();

		m_accX = -otherGravConst * (currentPos.x - gravPoint.x);
		m_accY = -otherGravConst * (currentPos.y - gravPoint.y);

		m_speedX += m_accX;
		m_speedY += m_accY;
	}
	MoveSprite(sf::Vector2f(m_speedX, m_speedY));
	TrailRenderer();
}

void Asteroid::DestroyUpdate()
{

}

void Asteroid::UpdateZoom(float _zoomLevel)
{
	zoomLevel = _zoomLevel;
}

sf::VertexArray Asteroid::GetAimLine()
{
	return aimLine;
}

AsteroidState Asteroid::GetState()
{
	return AState;
}
