#ifndef BOT_HPP
#define BOT_HPP



struct Chromosome
{
	constexpr static float const MUTATION_POWER = 0.01f;
	constexpr static int const GEN_COUNT = 4;
	static_assert(GEN_COUNT > 1);


	float startenergy = 0.25f;
	float saturation = 0.25f;
	float stepprice = 0.25f;
	float budprice = 0.25f;

	Chromosome &mutation();
	
	template<class Ostream> Ostream &print(Ostream &os) const
	{
		os << "start energy:	" << startenergy << '\n';
		os << "saturation:	" << saturation << '\n';
		os << "step price:	" << stepprice << '\n';
		os << "bud price:	" << budprice << '\n';
		return os;
	}

};

template<class Ostream> Ostream &operator<<(
	Ostream &os, Chromosome const &chroma
)
{
	return chroma.print(os);
}



class Bot
{
public:
	constexpr static float const DEFAULT_ENERGY = 1000.0f;
	constexpr static float const DEFAULT_SATURATION = 100.0f;
	constexpr static float const DEFAULT_STEP_PRICE = 5.0f;
	constexpr static float const DEFAULT_BUD_PRICE = 2000.0f;
	constexpr static float const BUD_PROBABILITY = 1.5f;
	constexpr static float const AGING_SPEED = 1.05;


	Bot(Chromosome const &chroma = Chromosome());
	~Bot();

	void step();
	void eat();
	bool tryBud() const;
	Bot *bud();

	Chromosome const &getChromosome() const;
	// this function set chromosome and reset energy.
	Bot &create(Chromosome const &newchroma);

	float getEnergy() const;
	float getSaturation() const;
	float getStepPrice() const;
	float getBudPrice() const;

	template<class Ostream> Ostream &print(Ostream &os) const
	{
		os << "energy:		" << _energy << '\n';
		os << "saturation:	" << _saturation << '\n';
		os << "stepprice: 	" << _stepprice << '\n';
		os << "budprice:	" << _budprice << '\n';
		return os;
	}

	bool wasUpdate = false;

private:
	Chromosome _chroma;
	
	float _energy = 100.0f;

	float _saturation;
	float _stepprice;
	float _budprice;
};
	
template<class Ostream> Ostream &operator<<(
	Ostream &os, Bot const &bot
)
{
	return bot.print(os);
}



#endif // BOT_HPP
