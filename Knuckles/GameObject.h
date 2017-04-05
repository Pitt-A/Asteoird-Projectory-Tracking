#pragma once

#include <SFML/Graphics.hpp>

class GameObject
{
public:
	GameObject();
	~GameObject();
	virtual void Update();
	sf::CircleShape Draw();
	void SetCircle(sf::Color, float);
	void SetPos(sf::Vector2f);
	sf::Vector2f GetPos() const;
	void MoveSprite(sf::Vector2f);
	float GetScale();
	float GetRadius();
	void SetAtmosphere(sf::Color, float);
	sf::CircleShape GetAtmosphere();
	int GetThickness();
	void SetAtmosphereName(std::string);
	std::string GetAtmosphereName();
	void SetName(std::string);
	std::string GetName();
protected:
	sf::Vector2f currentPos;
private:
	bool m_hasSprite = false;
	sf::CircleShape circle;
	sf::CircleShape atmosphere;
	float atmosphereThickness = 0.0f;
	std::string atmoshpereElement;
	std::string name;
};