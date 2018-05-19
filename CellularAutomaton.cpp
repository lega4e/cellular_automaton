#include <iostream>
#include <utility>
#include "CellularAutomaton.hpp"

using namespace clever;



CellularAutomaton::CellularAutomaton(
	Field<CellularAutomaton::value_type> *field
): _field(field) {}

CellularAutomaton::~CellularAutomaton()
{
	if(_swapfield)
		delete[] _swapfield;
	return;
}



void CellularAutomaton::adjust()
{
	if(_swapfield) {
		delete[] _swapfield;
		_swapfield = nullptr;
	}
	return;
}



void CellularAutomaton::update()
{
	(this->*_update)();
	return;
}

void CellularAutomaton::clear()
{
	for(unsigned int i = 0; i < _field->w*_field->h; ++i) {
		_field->d[i] = 0;
	}
	return;
}



unsigned int CellularAutomaton::alive() const
{
	return _alive;
}



CellularAutomaton::UpdateType CellularAutomaton::updateType() const
{
	return _updatetype;
}

void CellularAutomaton::updateType(
	CellularAutomaton::UpdateType newupdatetype
)
{
	_updatetype = newupdatetype;
	switch(_updatetype) {
	case UpdateType::lifeupdate:
		_update = &CellularAutomaton::_twoDimensionalUpdate;
		_predicate = &CellularAutomaton::_lifePredicate;
		break;
	case UpdateType::wolframupdate:
		_update = &CellularAutomaton::_oneDimensionalUpdate;
		_predicate = &CellularAutomaton::_wolframPredicate;
		break;
	}
	return;
}



CellularAutomaton::AtType CellularAutomaton::atType() const
{
	return _attype;
}

void CellularAutomaton::atType(AtType newattype)
{
	_attype = newattype;
	switch(_attype) {
	case AtType::simpleat:
		_at = &CellularAutomaton::_simpleAt;
		break;
	case AtType::tapeat:
		_at = &CellularAutomaton::_tapeAt;
		break;
	}
	return;
}



Field<CellularAutomaton::value_type> *CellularAutomaton::field() const
{
	return _field;
}

void CellularAutomaton::field(Field<value_type> *newfield)
{
	if(_swapfield && (_field->w*_field->h != newfield->w*newfield->h)) {
		delete[] _swapfield;
		_swapfield = nullptr;
	}

	_field = newfield;
	return;
}



unsigned int CellularAutomaton::wolframCode() const
{
	return _wolframcode;
}

void CellularAutomaton::wolframCode(unsigned int newwolframcode) 
{
	_wolframcode = newwolframcode;
	return;
}



void CellularAutomaton::_oneDimensionalUpdate()
{	
	unsigned int lastline =
		_linetoupdate == 0 ? _field->h-1 : _linetoupdate-1; 

	for(unsigned int x = 0; x < _field->w; ++x) {
		_field->d[_linetoupdate*_field->w+x] =
			(this->*_predicate)(x, lastline);
	}
	if(_linetoupdate+1 == _field->h)
		_linetoupdate = 0;
	else
		++_linetoupdate;
	return;
}

void CellularAutomaton::_twoDimensionalUpdate()
{
	if(!_swapfield) {
		_swapfield = new value_type[_field->w*_field->h];
	}
	_alive = 0;
	for(unsigned int y = 0; y < _field->h; ++y) {
		for(unsigned int x = 0; x < _field->w; ++x) {
			if(
				(_swapfield[y*_field->w+x] =
					(this->*_predicate)(x, y)) == 1
			)
				++_alive;
		}
	}
	std::swap(_field->d, _swapfield);
	return;
}



CellularAutomaton::value_type CellularAutomaton::_lifePredicate(unsigned int x, unsigned int y)
{
	unsigned short int c = 0;
	if((this->*_at)(x-1, y-1))
		++c;
	if((this->*_at)(x, y-1))
		++c;
	if((this->*_at)(x+1, y-1))
		++c;

	if((this->*_at)(x-1, y))
		++c;
	if((this->*_at)(x+1, y))
		++c;
	

	if((this->*_at)(x-1, y+1))
		++c;
	if((this->*_at)(x, y+1))
		++c;
	if((this->*_at)(x+1, y+1))
		++c;

	if((this->*_at)(x, y))
		return c == 2 || c == 3 ? 1 : 0;
	else
		return c == 3 ? 1 : 0;
}

CellularAutomaton::value_type CellularAutomaton::_wolframPredicate(unsigned int x, unsigned int y)
{
	unsigned int c = 0;
	if((this->*_at)(x+1, y))
		c += 1 << 0;
	if((this->*_at)(x, y))
		c += 1 << 1;
	if((this->*_at)(x-1, y))
		c += 1 << 2;

	return (1 << c) & _wolframcode ? 1 : 0;
}
	


CellularAutomaton::value_type &CellularAutomaton::_simpleAt(int x, int y)
{
	static value_type dummy = 0;
	if(x < 0 || (unsigned int)(x) >= _field->w || y < 0 || (unsigned int)(y) >= _field->h) {
		dummy = 0;
		return dummy;
	}
	else
		return _field->d[y*_field->w+x];
}

CellularAutomaton::value_type &CellularAutomaton::_tapeAt(int x, int y)
{
	if(x < 0) do
		x += _field->w;
	while(x < 0);
	else while((unsigned int)(x) >= _field->w)
		x -= _field->w;

	if(y < 0) do
		y += _field->h;
	while(y < 0);
	else while((unsigned int)(y) >= _field->h)
		y -= _field->h;

	return _field->d[y*_field->w+x];
}

