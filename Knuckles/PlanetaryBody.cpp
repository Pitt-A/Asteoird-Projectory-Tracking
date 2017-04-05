#include "PlanetaryBody.h"

PlanetaryBody::PlanetaryBody()
{
	lines.setPrimitiveType(sf::LinesStrip);
}

PlanetaryBody::PlanetaryBody(bool _isStar, PlanetaryBody* _target, float _gravConstant)
{
	lines.setPrimitiveType(sf::LinesStrip);
	isStar = _isStar;
	m_planetaryBodyTarget = _target;
	m_gravConst = _gravConstant;
}

PlanetaryBody::~PlanetaryBody()
{

}

void PlanetaryBody::Update()
{
	if (!isStar)
	{
		sf::Vector2f gravPoint = m_planetaryBodyTarget->GetPos();
		float otherGravConst = m_planetaryBodyTarget->GetGravConst();

		m_accX = -otherGravConst * (currentPos.x - gravPoint.x);
		m_accY = -otherGravConst * (currentPos.y - gravPoint.y);

		m_speedX += m_accX;
		m_speedY += m_accY;
	}

	MoveSprite(sf::Vector2f(m_speedX * (1 / GetScale()), m_speedY * (1 / GetScale())));

	TrailRenderer();
}

void PlanetaryBody::SetSpeed(float _speedX, float _speedY)
{
	m_speedX = _speedX;
	m_speedY = _speedY;
}

float PlanetaryBody::GetSpeedX()
{
	return m_speedX;
}

float PlanetaryBody::GetSpeedY()
{
	return m_speedY;
}

float PlanetaryBody::GetGravConst()
{
	return m_gravConst;
}

void PlanetaryBody::TrailRenderer()
{
	if (!isStar)
	{
		if (completeTrail)
		{
			lines.append(currentPos);
			lines[lines.getVertexCount() - 1].color = trailColor;
			if (lines.getVertexCount() >= 800)
			{
				lines.resize(800);
			}
		}
		else
		{
			lines.append(currentPos);
			lines[lines.getVertexCount() - 1].color = trailColor;
			if (lines.getVertexCount() >= 200)
			{
				for (int i = 0; i < lines.getVertexCount() - 1; i++)
				{
					lines[i] = lines[i + 1];
				}
				lines.resize(200);
			}
		}
	}	
}

void PlanetaryBody::SetTrailColor(sf::Color _color)
{
	trailColor = _color;
	for (int i = 0; i < lines.getVertexCount(); i++)
	{
		lines[i].color = trailColor;
	}
}

sf::VertexArray PlanetaryBody::GetTrail()
{
	return lines;
}

void PlanetaryBody::SetTrailComplete(bool _bool)
{
	completeTrail = _bool;
}