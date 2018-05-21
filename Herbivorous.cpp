#include <iostream>
#include <random>

#include "BotField.hpp"
#include "Herbivorous.hpp"




Herbivorous::Herbivorous()
{

}

Herbivorous::Action Herbivorous::whatDo(Enviroment const &env) const
{
	if(_energy < 0) {
		return {Action::die, nullptr, 0};
	}
start_label:
	switch(random()%4) {
	case 0:
		return {Action::move, env.d[rand()%5], 0};
	case 1:
		return {Action::turn, nullptr, rand()%3-1};
	case 2:
		return {Action::eat, env.d[rand()%5], 0};
	case 3:
		if(_energy <= REPRODUCE_PRICE)
			goto start_label;
		return {Action::reproduce, env.d[rand()%5], 0};
	default:
		throw "it's inmpossible!";
	}
}

float Herbivorous::getEnergy() const
{
	return _energy;
}
Herbivorous &Herbivorous::setEnergy(float newenergy)
{
	_energy = newenergy;
	return *this;
}

Herbivorous &Herbivorous::wait()
{
	_energy -= WAIT_PRICE+_age*AGE_PRICE;
	++_age;
	return *this;
}

std::pair<int, int> const &Herbivorous::getPosition() const
{
	return _pos;
}

Herbivorous &Herbivorous::setPosition(std::pair<int, int> const &newpos)
{
	_pos = newpos;
	return *this;
}
Herbivorous &Herbivorous::move(int newx, int newy)
{
	_pos.first = newx;
	_pos.second = newy;
	_energy -= STEP_PRICE+_age*AGE_PRICE;
	++_age;
	return *this;
}

int Herbivorous::getDirect() const
{
	return _direct;
}
Herbivorous &Herbivorous::setDirect(int newdirect)
{
	_direct = newdirect;
	return *this;
}
Herbivorous &Herbivorous::turn(int offset)
{
	_direct += offset;

	if(_direct > 3) do {
		_direct -= 4;
	} while(_direct > 3);
	else if(_direct < 0) do {
		_direct += 4;
	} while(_direct < 0);

	return *this;
}

Herbivorous &Herbivorous::eat(Plant const &plant)
{
	_energy += SATURATION;
	++_age;
	return *this;
}
Herbivorous *Herbivorous::reproduce()
{
	++_age;
	_energy -= REPRODUCE_PRICE;
	return new Herbivorous;
}



// end
