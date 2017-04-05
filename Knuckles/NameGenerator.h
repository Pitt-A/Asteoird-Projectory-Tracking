#include <string>
#include <vector>

class NameGenerator
{
public:
	NameGenerator();
	~NameGenerator();
	std::string CreatePlanetName(std::string, int);
	std::string CreateStarName();
	std::string CreateAsteroidName();
private:
	std::vector<std::string> planetNames = { "Kepler", "Gliese", "Wolf", "Proxima Centauri", "Kapteyn",
		"Tau Ceti", "Crux", "Cancer", "Taurus", "Lyra", "Virgo", "Cygnus",
		"Ursa Major", "Leo", "Grus", "Draco", "Gemini", "Orion", "Pisces",
		"Phoenix", "Hydrus", "Hercules", "Canis Major", "Canis Minor",
		"Corona Borealis", "Serpens", "Columba", "Ursa Minor" };
	std::vector<char> characters = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
		'y', 'z' };
	std::vector<std::string> asteroidNames = { "Sprite", "Maiden", "Zeus", "Medusa", "Hades", "Yrel",
		"Olympus", "Mithril", "Rune", "Prophet", "Hercules", "Poseidon",
		"Perseus", "Ares", "Nimbus", "Tempus", "Patria", "Gaia", "Chaos",
		"Honour", "Tenebris", "Despectum", "Spiritus", "Etiam",
		"Hominum", "Spiralis", "Alacritas", "Ignis", "Mortem" };
	const int zero = 0;

};