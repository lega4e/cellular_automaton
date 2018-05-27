#ifndef BOT_FIELD_HPP
#define BOT_FIELD_HPP

#include <list>

#include <clever/Field.hpp>

#include "Herbivorous.hpp"
#include "Predator.hpp"



class Plant;



struct Cell
{
	enum KeepThing
	{
		nothing, plant, herbivorous, predator
	};

	KeepThing what = nothing;
	void *thing = nullptr;

	template<typename T> inline T *to()
	{
		return static_cast<T*>(thing);
	}
	template<typename T> inline T const *to() const
	{
		return static_cast<T const *>(thing);
	}

};



struct Enviroment
{
	Cell *d[5] = {
		nullptr, nullptr, nullptr, nullptr, nullptr
	};
};



class BotField
{
public:
	BotField(clever::Field<Cell> &&field);
	BotField(clever::Field<Cell> const &field = {0, 0, nullptr});
	~BotField();

	BotField &update();

	clever::Field<Cell> const &getField() const;
	BotField &setField(clever::Field<Cell> &&newfield);
	BotField &setField(clever::Field<Cell> const &newfield);


	BotField &clear();
	BotField &addThing(int x, int y, Cell thing);
	BotField &removeThing(int x, int y);
	BotField &removeThing(void *thing);
	

private:
	clever::Field<Cell> _f;

	std::list<Plant*> _plants;
	std::list<Herbivorous*> _herbs;
	std::list<Predator*> _predators;

	Enviroment _learnEnviroment(int x, int y, int direct);

	void _actionDispatch(
		Herbivorous *herbi, Herbivorous::Action const &action
	);
	void _actionDispatch(
		Predator *predator, Predator::Action const &action
	);

	void _sawField();
};

#endif // BOT_FIELD_HPP
