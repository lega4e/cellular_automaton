#ifndef HERBIVOROUS_HPP
#define HERBIVOROUS_HPP

#include <iostream>
#include <random>
#include <utility>



struct Cell;
struct Enviroment;
class Plant;

class Herbivorous
{
public:
	constexpr static float const START_ENERGY = 300.0f;
	constexpr static float const STEP_PRICE = 0.7f;
	constexpr static float const WAIT_PRICE = 0.5f;
	constexpr static float const SATURATION = 50.0f;
	constexpr static float const REPRODUCE_PRICE = 1000.0f;
	constexpr static float const AGE_PRICE = 0.0001;


	struct Action
	{
		enum Type
		{
			move, turn, eat, reproduce, die
		};
		Type type;
		Cell *cell;
		int turnpower;
	};

	Herbivorous();

	Action whatDo(Enviroment const &) const;

	float getEnergy() const;
	Herbivorous &setEnergy(float newenergy);

	Herbivorous &wait();
	
	std::pair<int, int> const &getPosition() const;
	Herbivorous &setPosition(std::pair<int, int> const &newpos);
	Herbivorous &move(int newx, int newy);

	int getDirect() const;
	Herbivorous &setDirect(int newdirect);
	Herbivorous &turn(int offset);

	Herbivorous &eat(Plant const &plant);
	Herbivorous *reproduce();

	template<class Ostream> Ostream &print(Ostream &os) const
	{
		os << "energy: " << _energy << '\n';
		os << "direct: " << _direct << '\n';
		os << "age: " << _age << '\n';
		return os;
	}

private:
	float _energy = START_ENERGY;
	std::pair<int, int> _pos = {0, 0};
	int _direct = 0;
	int _age = 0;

};

template<class Ostream> Ostream &operator<<(Ostream &os, Herbivorous const &herbi)
{
	return herbi.print(os);
}	


#endif // HERBIVOROUS_HPP

