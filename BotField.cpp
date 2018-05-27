#include <iostream>

#include <clever/IostreamFunctions.hpp>

#include "BotField.hpp"
#include "Herbivorous.hpp"
#include "Plant.hpp"
#include "Predator.hpp"

using namespace clever;
using namespace std;



BotField::BotField(clever::Field<Cell> &&field): _f(std::move(field)) {}
BotField::BotField(clever::Field<Cell> const &field): _f(field) {}
BotField::~BotField()
{
	for(auto i : _plants)
		delete i;
	for(auto i : _herbs)
		delete i;
	for(auto i : _predators)
		delete i;
	return;
}



BotField &BotField::update()
{
	for(auto i : _plants)
		i->update();

	Herbivorous::Action herbact;
	if(auto b = _herbs.begin(), e = _herbs.end(); b != e) do {
herbivorous_what_do_label:
		herbact = (*b)->whatDo(
			_learnEnviroment(
				(*b)->getPosition().first,
				(*b)->getPosition().second,
				(*b)->getDirect()
			)
		);

		if(herbact.type == Herbivorous::Action::turn) {
			(*b)->turn(herbact.turnpower);
			goto herbivorous_what_do_label;
		}
		if(herbact.type == Herbivorous::Action::die) {
			removeThing(*(b++));
			continue;
		}
			
		_actionDispatch((*b), herbact);
		++b;
	} while(b != e);

	Predator::Action pract;
	for(auto i : _predators) {
predator_what_do_label:
		pract = i->whatDo(
			_learnEnviroment(
				i->getPosition().first,
				i->getPosition().second,
				i->getDirect()
			)
		);

		if(pract.type == Predator::Action::turn) {
			i->turn(pract.turnpower);
			goto predator_what_do_label;
		}

		_actionDispatch(i, pract);
	}
	
	_sawField();

	return *this;
}



clever::Field<Cell> const &BotField::getField() const
{
	return _f;
}

BotField &BotField::setField(clever::Field<Cell> &&newfield)
{
	clear();
	_f = (newfield);

	return *this;
}

BotField &BotField::setField(clever::Field<Cell> const &newfield)
{
	clear();
	_f = newfield;
	return *this;
}



BotField &BotField::clear()
{
	for(auto i : _plants)
		delete i;
	for(auto i : _herbs)
		delete i;
	for(auto i : _predators)
		delete i;
	_plants.clear();
	_herbs.clear();
	_predators.clear();
	
	_f.clear(Cell{Cell::nothing, nullptr});

	return *this;
}

BotField &BotField::addThing(int x, int y, Cell thing)
{
	switch(thing.what) {
	case Cell::plant:
		_plants.push_back(thing.to<Plant>());
		_plants.back()->setPosition({x, y});
		break;
	case Cell::herbivorous:
		_herbs.push_back(thing.to<Herbivorous>());
		_herbs.back()->setPosition({x, y});
		break;
	case Cell::predator:
		_predators.push_back(thing.to<Predator>());
		_predators.back()->setPosition({x, y});
		break;
	default:
		throw "invalid thing to add";
	}

	removeThing(x, y);
	_f.at(x, y) = thing;

	return *this;
}

BotField &BotField::removeThing(int x, int y)
{
	removeThing(_f.at(x, y).thing);
	return *this;
}

BotField &BotField::removeThing(void *thing)
{
	if(!thing)
		return *this;

	std::pair<int, int> pos;
	for(auto b = _plants.begin(), e = _plants.end(); b != e; ++b) {
		if(*b == thing) {
			pos = (*b)->getPosition();
			delete *b;
			_plants.erase(b);
			goto delete_from_cell_label;
		}
	}

	for(auto b = _herbs.begin(), e = _herbs.end(); b != e; ++b) {
		if(*b == thing) {
			pos = (*b)->getPosition();
			delete *b;
			_herbs.erase(b);
			goto delete_from_cell_label;
		}
	}

	for(auto b = _predators.begin(), e = _predators.end(); b != e; ++b) {
		if(*b == thing) {
			pos = (*b)->getPosition();
			delete *b;
			_predators.erase(b);
			goto delete_from_cell_label;
		}
	}

	return *this;
delete_from_cell_label:
	_f.at(pos).what = Cell::nothing;
	_f.at(pos).thing = nullptr;
	return *this;
}



Enviroment BotField::_learnEnviroment(int x, int y, int direct)
{
	// first(that 4) - direct, second(that 5) - cell
	// 	2			* * *
	//    1	  3			* B *
	//      0
	constexpr static int const offsetx[4][5] = {
		{ -1, -1, 0, 1, 1 },
		{ 0, -1, -1, -1, 0 },
		{ 1, 1, 0, -1, -1 },
		{ 0, 1, 1, 1, 0 }
	};
	constexpr static int const offsety[4][5] = {
		{ 0, 1, 1, 1, 0 },
		{ -1, -1, 0, 1, 1 },
		{ 0, -1, -1, -1, 0 },
		{ 1, 1, 0, -1, -1 }
	};

	return { {
			&_f.tapeAt(x+offsetx[direct][0], y+offsety[direct][0]),
			&_f.tapeAt(x+offsetx[direct][1], y+offsety[direct][1]),
			&_f.tapeAt(x+offsetx[direct][2], y+offsety[direct][2]),
			&_f.tapeAt(x+offsetx[direct][3], y+offsety[direct][3]),
			&_f.tapeAt(x+offsetx[direct][4], y+offsety[direct][4])
	} };
}

void BotField::_actionDispatch(
	Herbivorous *herbi, Herbivorous::Action const &action
)
{
	switch(action.type) {
	case Herbivorous::Action::move:
		// Если перемещение невозможно
		if(action.cell->what != Cell::nothing) {
			herbi->wait();
			return;
		}



		// Травоядный отрывает свою задницу
		_f.at(herbi->getPosition()).thing = nullptr;
		_f.at(herbi->getPosition()).what = Cell::nothing;

		// Травоядный двигается
		herbi->move(
			(action.cell-_f.d)%_f.w,
			(action.cell-_f.d)/_f.w
		);
		
		// Травоядный опускает свою задницу
		action.cell->thing = herbi;
		action.cell->what = Cell::herbivorous;

		return;
	case Herbivorous::Action::eat:
		// Если съесть это невозможно
		if(action.cell->what != Cell::plant) {
			herbi->wait();
			return;
		}

		// Травоядный ест
		herbi->eat(*action.cell->to<Plant>());

		// Растение умирает. Растения больше нет. Грустно...
		removeThing(action.cell->thing);
		return;
	case Herbivorous::Action::reproduce:
		// Если невозможно родить
		if(action.cell->what != Cell::nothing) {
			herbi->wait();
			return;
		}

		// Маленький травоядный рождается на свет
		action.cell->thing = herbi->reproduce();
		action.cell->what = Cell::herbivorous;

		// Позиционируем
		action.cell->to<Herbivorous>()->setPosition(
			{(action.cell-_f.d)%_f.w,
			(action.cell-_f.d)/_f.w}
		);

		// Ставим на учет
		_herbs.push_back(action.cell->to<Herbivorous>());
		return;
	default:
		throw "invliad action";
	}
}
void BotField::_actionDispatch(
	Predator *predator, Predator::Action const &action
)
{
	return;
}

void BotField::_sawField()
{
	Plant *plant = nullptr;
	for(Cell *b = _f.d, *e = _f.d+_f.w*_f.h; b != e; ++b) {
		if(b->what == Cell::nothing && float(rand())/RAND_MAX > 0.999) {
			plant = new Plant;
			b->thing = plant;
			b->what = Cell::plant;

			plant->setPosition({
				(b-_f.d)%_f.w,
				(b-_f.d)/_f.w
			});
			_plants.push_back(plant);
		}
	}
	return;
}




// endu
