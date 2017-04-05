#include "PlanetaryBody.h"

PlanetaryBody::PlanetaryBody() //Standard constructor
{
	lines.setPrimitiveType(sf::LinesStrip);
}

PlanetaryBody::PlanetaryBody(bool _isStar, PlanetaryBody* _target, float _gravConstant) //Constructor with whether it's a star, the star to orbit and it's grav constant
{
	lines.setPrimitiveType(sf::LinesStrip);
	isStar = _isStar;
	m_planetaryBodyTarget = _target;
	m_gravConst = _gravConstant;
}

PlanetaryBody::~PlanetaryBody()
{

}

void PlanetaryBody::Update() //The main update loop, ovveriding the GameObject loop
{
	if (!isStar) //Only move if not a star
	{
		sf::Vector2f gravPoint = m_planetaryBodyTarget->GetPos();
		float otherGravConst = m_planetaryBodyTarget->GetGravConst();

		//A = -G*(M1 - M2) where A is acceleration, G is the gravitational constant, M1 is current position and M2 is other object position
		m_accX = -otherGravConst * (currentPos.x - gravPoint.x);
		m_accY = -otherGravConst * (currentPos.y - gravPoint.y);

		//Accumalting the speed
		m_speedX += m_accX;
		m_speedY += m_accY;
	}

	MoveSprite(sf::Vector2f(m_speedX * (1 / GetScale()), m_speedY * (1 / GetScale())));

	TrailRenderer();
}

void PlanetaryBody::SetSpeed(float _speedX, float _speedY) //Setting the object's speed
{
	m_speedX = _speedX;
	m_speedY = _speedY;
}

float PlanetaryBody::GetSpeedX() //Getting the object's speed in X
{
	return m_speedX;
}

float PlanetaryBody::GetSpeedY() //Getting the object's speed in Y
{
	return m_speedY;
}

float PlanetaryBody::GetGravConst() //Getting the gravitation constant for simulation
{
	return m_gravConst;
}

void PlanetaryBody::TrailRenderer() //Adding the next vertex to the vertex array
{
	mass = 5.973f * GetScale();
	if (!isStar) //Only add to vertex array if not a star
	{
		if (completeTrail) //The complete trail code
		{
			lines.append(currentPos);
			lines[lines.getVertexCount() - 1].color = trailColor;
			if (lines.getVertexCount() >= 800)
			{
				lines.resize(800); //Cut off any vertices above index 800
			}
		}
		else //The limited trail code
		{
			lines.append(currentPos);
			lines[lines.getVertexCount() - 1].color = trailColor;
			if (lines.getVertexCount() >= 200)
			{
				for (int i = 0; i < lines.getVertexCount() - 1; i++)
				{
					lines[i] = lines[i + 1]; //Move every vertex to the next point in the array
				}
				lines.resize(200);
			}
		}
	}	
}

void PlanetaryBody::SetTrailColor(sf::Color _color) //Setting the colour of the entire trail
{
	trailColor = _color;
	for (int i = 0; i < lines.getVertexCount(); i++)
	{
		lines[i].color = trailColor;
	}
}

sf::VertexArray PlanetaryBody::GetTrail() //Getting the trail for drawing
{
	return lines;
}

void PlanetaryBody::SetTrailComplete(bool _bool) //Setting whether the trail should extend to a compelete cycle (planets) or not (asteroids)
{
	completeTrail = _bool;
}

float PlanetaryBody::GetMass()
{
	return mass;
}
