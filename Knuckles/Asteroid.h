#include <vector>
#include "PlanetaryBody.h"
#include "AsteroidState.h"

class Asteroid : public PlanetaryBody
{
public:
	Asteroid(sf::RenderWindow*, std::vector<PlanetaryBody*>);
	Asteroid();
	~Asteroid();
	//void SetList(std::vector<PlanetaryBody*>);
	void Update();
	void UpdateZoom(float);
	sf::VertexArray GetAimLine();
	AsteroidState GetState();
private:
	std::vector<PlanetaryBody*> m_planetaryBodyList;
	int noOfObjects;
	AsteroidState AState = AsteroidState::AS_PLACE;
	void PlaceUpdate();
	void AimUpdate();
	void SimUpdate();
	void DestroyUpdate();
	float zoomLevel;
	sf::RenderWindow *window;
	sf::VertexArray aimLine;
	bool timer = false;
	float time = 0.0f;
	sf::Clock clock;
	sf::Vector2f windowDrawOffset = sf::Vector2f(-8.0f, -31.0f);
};