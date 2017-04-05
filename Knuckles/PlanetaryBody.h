#pragma once
#include "GameObject.h"

class PlanetaryBody : public GameObject
{
public:
	PlanetaryBody();
	PlanetaryBody(bool, PlanetaryBody*, float);
	~PlanetaryBody();
	void SetSpeed(float, float);
	float GetSpeedX();
	float GetSpeedY();
	void Update() override;
	float GetGravConst();
	void TrailRenderer();
	void SetTrailColor(sf::Color);
	sf::VertexArray GetTrail();
	void SetTrailComplete(bool);
	float GetMass();
protected:
	float m_speedX = 0, m_speedY = 0;
	float m_accX = 0, m_accY = 0;
	sf::VertexArray lines;
private:
	sf::Color trailColor = sf::Color::White;
	bool isStar;
	PlanetaryBody* m_planetaryBodyTarget;
	float m_gravConst = 0;
	bool completeTrail = false;
	float mass;
};