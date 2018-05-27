#ifndef PREDATOR_HPP
#define PREDATOR_HPP

struct Cell;
struct Enviroment;
class Herbivorous;

class Predator
{
public:
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

	Action whatDo(Enviroment const &env) const;

	float getEnergy() const;
	Predator &setEnergy(float newenergy);

	Predator &wait();
	
	std::pair<int, int> getPosition() const;
	Predator &setPosition(std::pair<int, int> const &newpos);
	Predator &move(int newx, int newy);

	int getDirect() const;
	Predator &setDirect(int newdirect);
	Predator &turn(int offset);

	Predator &eat(Herbivorous const &herbi);
	Predator *reproduce();

private:
	float _energy;
	std::pair<int, int> _pos;
	int _direct;

};

#endif // PREDATOR_HPP
