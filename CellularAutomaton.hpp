#ifndef CELLULAR_AUTOMATON_HPP
#define CELLULAR_AUTOMATON_HPP

#include <clever/Field.hpp>

class CellularAutomaton
{
public:
	typedef int8_t value_type;

	enum UpdateType
	{
		lifeupdate, wolframupdate
	};
	enum AtType
	{
		tapeat, simpleat
	};

	CellularAutomaton(clever::Field<value_type> *field = nullptr);
	~CellularAutomaton();

	void adjust();
	void update();
	void clear();

	unsigned int alive() const; 
	
	UpdateType updateType() const;
	void updateType(UpdateType newupdatetype);

	AtType atType() const;
	void atType(AtType newattype);

	clever::Field<value_type> *field() const;
	void field(clever::Field<value_type> *newfield);

	unsigned int wolframCode() const;
	void wolframCode(unsigned int newwolframcode); 
private:
	clever::Field<value_type> *_field;

	void _oneDimensionalUpdate();
	unsigned int _linetoupdate = 1;
	void _twoDimensionalUpdate();
	UpdateType _updatetype;
	
	value_type _lifePredicate(unsigned int x, unsigned int y);
	value_type *_swapfield = nullptr;
	unsigned int _alive = 0; 
	value_type _wolframPredicate(unsigned int x, unsigned int y);
	unsigned int _wolframcode = 0;

	value_type &_simpleAt(int x, int y);
	value_type &_tapeAt(int x, int y);
	AtType _attype;

	void (CellularAutomaton::*_update)() = &CellularAutomaton::_twoDimensionalUpdate;
	value_type (CellularAutomaton::*_predicate)(unsigned int, unsigned int) = &CellularAutomaton::_lifePredicate;
	value_type &(CellularAutomaton::*_at)(int, int) = &CellularAutomaton::_tapeAt;

	
};


#endif // CELLULAR_AUTOMATON_HPP
