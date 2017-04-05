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

sf::CircleShape GameObject::Draw() //Return the current circle for drawing
{
	return circle;
}

void GameObject::SetCircle(sf::Color _color, float _scale) //Setting the circle for the planet
{
	circle.setRadius(100);
	circle.setPosition(currentPos);
	circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
	m_hasSprite = true;
	circle.setScale(sf::Vector2f(_scale, _scale));
	circle.setFillColor(_color);
}

void GameObject::SetPos(sf::Vector2f tempPos) //Setting the position of the object
{
	currentPos = tempPos;
	circle.setPosition(tempPos);
	atmosphere.setPosition(tempPos);
}

sf::Vector2f GameObject::GetPos() const //Returning the position of the object in world coords
{
	return currentPos;
}

void GameObject::MoveSprite(sf::Vector2f movement) //Moving the sprite (as opposed to setting it's position)
{
	circle.move(movement);
	atmosphere.move(movement);
	currentPos += movement;
}

float GameObject::GetScale() //Getting the current scale of the object (as SFML will not adjust the radius according to the scale)
{
	return circle.getScale().x;
}

float GameObject::GetRadius() //Getting the current radius of the object (as SFML will not adjust the radius according to the scale)
{
	return circle.getRadius();
}

void GameObject::SetAtmosphere(sf::Color _color, float _thickness) //Setting the atmosphere of the object
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

sf::CircleShape GameObject::GetAtmosphere() //Getting the atmosphere for drawing
{
	return atmosphere;
}

int GameObject::GetThickness() //Getting the thickness of the atmosphere for displaying
{
	return atmosphereThickness;
}

void GameObject::SetAtmosphereName(std::string _string) //Setting the name of the element found in the atmosphere
{
	atmoshpereElement = _string;
}

std::string GameObject::GetAtmosphereName() //Getting the name of the atmosphere's elemnt for displaying
{
	return atmoshpereElement;
}

void GameObject::SetName(std::string _name) //Setting the name of the object
{
	name = _name;
}

std::string GameObject::GetName() //Getting the name of the object for displaying
{
	return name;
}