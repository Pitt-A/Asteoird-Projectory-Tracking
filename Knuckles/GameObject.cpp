#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::Update()
{

}

sf::CircleShape GameObject::Draw()
{
	return circle;
}

void GameObject::SetCircle(sf::Color _color, float _scale)
{
	circle.setRadius(100);
	circle.setPosition(currentPos);
	circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
	m_hasSprite = true;
	circle.setScale(sf::Vector2f(_scale, _scale));
	circle.setFillColor(_color);
}

void GameObject::SetPos(sf::Vector2f tempPos)
{
	currentPos = tempPos;
	circle.setPosition(tempPos);
	atmosphere.setPosition(tempPos);
}

sf::Vector2f GameObject::GetPos() const
{
	return currentPos;
}

void GameObject::MoveSprite(sf::Vector2f movement)
{
	circle.move(movement);
	atmosphere.move(movement);
	currentPos += movement;
}

float GameObject::GetScale()
{
	return circle.getScale().x;
}

float GameObject::GetRadius()
{
	return circle.getRadius();
}

void GameObject::SetAtmosphere(sf::Color _color, float _thickness)
{
	atmosphere.setRadius(circle.getRadius() * 1.2f);
	atmosphere.setScale(circle.getScale());
	atmosphere.setPosition(currentPos);
	atmosphere.setOrigin(sf::Vector2f(atmosphere.getRadius(), atmosphere.getRadius()));
	sf::Color tempColor = _color;
	tempColor = sf::Color(tempColor.r, tempColor.g, tempColor.b, _thickness);
	atmosphere.setFillColor(tempColor);

	atmosphereThickness = _thickness;
}

sf::CircleShape GameObject::GetAtmosphere()
{
	return atmosphere;
}

int GameObject::GetThickness()
{
	return atmosphereThickness;
}

void GameObject::SetAtmosphereName(std::string _string)
{
	atmoshpereElement = _string;
}

std::string GameObject::GetAtmosphereName()
{
	return atmoshpereElement;
}

void GameObject::SetName(std::string _name)
{
	name = _name;
}

std::string GameObject::GetName()
{
	return name;
}