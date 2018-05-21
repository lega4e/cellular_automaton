#include "Plant.hpp"



Plant::Plant(float energy): _energy(energy) {}



Plant &Plant::update()
{
	++_age;
	return *this;
}



float Plant::getEnergy() const
{
	return _energy;
}

Plant &Plant::setEnergy(float newenergy)
{
	_energy = newenergy;
	return *this;
}



int Plant::getAge() const
{
	return _age;
}



std::pair<int, int> const &Plant::getPosition() const
{
	return _pos;
}

Plant &Plant::setPosition(std::pair<int, int> const &newpos)
{
	_pos = newpos;
	return *this;
}


// end
