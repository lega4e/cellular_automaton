#ifndef BOT_FIELD_HPP
#define BOT_FIELD_HPP

#include <clever/Field.hpp>

#include "bot.hpp"

struct Cell
{
	bool haveFood = false;
	Bot *bot = nullptr;
};


class BotField
{
public: 
	constexpr static float const GENERATE_FOOD_PROBABILITY = 0.05f;

	BotField();
	BotField(clever::Field<Cell> &&);
	BotField(clever::Field<Cell> const &field);

	BotField &update();

	clever::Field<Cell> const &getField() const;
	clever::Field<Cell> &getField();
	BotField &setField(clever::Field<Cell> &&newfield);
	BotField &setField(clever::Field<Cell> const &newfield);

private:
	clever::Field<Cell> _field;
	void _update_individ(unsigned int x, unsigned int y); 

};



#endif // BOT_FIELD_HPP
