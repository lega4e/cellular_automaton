#ifndef PLANT_HPP
#define PLANT_HPP

#include <utility>



class Plant
{
public:
	constexpr static float const DEFAULT_ENERGY = 100;

	Plant(float energy = DEFAULT_ENERGY);

	Plant &update();

	float getEnergy() const;
	Plant &setEnergy(float newenergy);

	int getAge() const;

	std::pair<int, int> const &getPosition() const;
	Plant &setPosition(std::pair<int, int> const &plant);

private:
	int _age = 0;
	float _energy = 0.0f;

	std::pair<int, int> _pos = {0, 0};

};

#endif // PLANT_HPP
