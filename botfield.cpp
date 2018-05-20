#include <iostream>
#include <random>
#include <utility>
#include "botfield.hpp"

using namespace clever;



BotField::BotField() {}

BotField::BotField(Field<Cell> const &field):
	_field(field) {}

BotField::BotField(Field<Cell> &&field):
	_field(std::move(field)) {}



BotField &BotField::update()
{
	for(unsigned int y = 0; y < _field.h; ++y) {
		for(unsigned int x = 0; x < _field.w; ++x) {
			if(
				_field.at(x, y).bot &&
				!_field.at(x, y).bot->wasUpdate
			)
				_update_individ(x, y);
		}
	}
	
	for(unsigned int y = 0; y < _field.h; ++y) {
		for(unsigned int x = 0; x < _field.w; ++x) {
			if(_field.at(x, y).bot)
				_field.at(x, y).bot->wasUpdate = false;
			else if(float(rand())/RAND_MAX > 0.996)
				_field.at(x, y).haveFood = true;
		}
	}

	// create food
	// ...

	return *this;
}

void BotField::_update_individ(unsigned int x, unsigned int y)
{
#define isValid(x, y) ((x) >= 0 && (x) < _field.w && (y) >= 0 && (y) < _field.h)
	int neighborc = 0;
	Cell *neighborv[8];

	constexpr static int const offsetx[8] =
		{ -1, 0, 1, -1, 1, -1, 0, 1 };
	constexpr static int const offsety[8] = 
		{ -1, -1, -1, 0, 0, 1, 1, 1 };

	for(unsigned int i = 0; i < 8; ++i) {
		if(
			isValid(x+offsetx[i], y+offsety[i]) && 
			_field.at(x+offsetx[i], y+offsety[i]).
				bot == nullptr
		) {
			neighborv[neighborc] =
				_field.d + (y+offsety[i])*_field.w
					+ x+offsetx[i];
			++neighborc;
		}
	}
	if(neighborc == 0) {
		_field.at(x, y).bot->step();
		if(_field.at(x, y).bot->getEnergy() < 0) {
			delete _field.at(x, y).bot;
			_field.at(x, y).bot = nullptr;
		}
		return;
	}

	Cell *acell = neighborv[rand()%neighborc];
	acell->bot = _field.at(x, y).bot;
	_field.at(x, y).bot = nullptr;
	acell->bot->step();

	if(acell->haveFood) {
		acell->bot->eat();
		acell->haveFood = false;
	}

	if(acell->bot->getEnergy() < 0) {
		delete acell->bot;
		acell->bot = nullptr;
		return;
	}

	if(acell->bot->tryBud()) {
		_field.at(x, y).bot = acell->bot->bud();
	}
	acell->bot->wasUpdate = true;

	return;
}



Field<Cell> const &BotField::getField() const
{
	return _field;
}
Field<Cell> &BotField::getField()
{
	return _field;
}
BotField &BotField::setField(clever::Field<Cell> &&newfield)
{
	_field = std::move(newfield);
	return *this;
}
BotField &BotField::setField(clever::Field<Cell> const &newfield)
{
	_field = newfield;
	return *this;
}



// end
