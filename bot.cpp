#include <random>
#include <cmath>

#include "bot.hpp"



Chromosome &Chromosome::mutation()
{
	float *refs[] = {
		&startenergy, &saturation, &stepprice, &budprice
	};

	int tosub, toadd;
	do {
		tosub = rand()%GEN_COUNT;
		toadd = rand()%GEN_COUNT;
	} while(tosub == toadd && *refs[tosub]-MUTATION_POWER <= 0);

	*refs[tosub] -= MUTATION_POWER;
	*refs[toadd] += MUTATION_POWER;

	return *this;
};
		


Bot::Bot(Chromosome const &chroma)
{
	create(chroma);
}

Bot::~Bot() {}



void Bot::step()
{
	_energy -= _stepprice;
	_stepprice *= AGING_SPEED;
	return;
}

void Bot::eat()
{
	_energy += _saturation;
	return;
}

bool Bot::tryBud() const
{
	return BUD_PROBABILITY*((_energy-_budprice)/_budprice) > 
		float(rand())/RAND_MAX;
}

Bot *Bot::bud()
{
	_energy -= _budprice;
	return new Bot(
		Chromosome(_chroma).mutation()
	);
}



Chromosome const &Bot::getChromosome() const
{
	return _chroma;
}

Bot &Bot::create(Chromosome const &newchroma)
{
	_chroma = newchroma;

	_energy = DEFAULT_ENERGY +
		3*pow(DEFAULT_ENERGY*_chroma.startenergy, 0.9);
	_saturation = DEFAULT_SATURATION +
		3*pow(DEFAULT_SATURATION*_chroma.saturation, 0.9);
	_stepprice = DEFAULT_STEP_PRICE -
		2*pow(DEFAULT_STEP_PRICE*_chroma.stepprice, 0.9);
	_budprice = DEFAULT_BUD_PRICE -
		2*pow(DEFAULT_BUD_PRICE*_chroma.budprice, 0.9);

	return *this;
}



float Bot::getEnergy() const
{
	return _energy;
}

float Bot::getSaturation() const
{
	return _saturation;
}

float Bot::getStepPrice() const
{
	return _stepprice;
}

float Bot::getBudPrice() const
{
	return _budprice;
}



// end.
