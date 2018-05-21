#include "BotField.hpp"
#include "Herbivorous.hpp"
#include "Predator.hpp"


Predator::Action Predator::whatDo(Enviroment const &env) const
{
	return Action();
}

float Predator::getEnergy() const
{
	return 0.0f;
}
Predator &Predator::setEnergy(float newenergy)
{
	return *this;
}

Predator &Predator::wait()
{
	return *this;
}

std::pair<int, int> Predator::getPosition() const
{
	return {0, 0};
}
Predator &Predator::setPosition(std::pair<int, int> const &newpos)
{
	return *this;
}
Predator &Predator::move(int newx, int newy)
{
	return *this;
}

int Predator::getDirect() const
{
	return 0;
}
Predator &Predator::setDirect(int newdirect)
{
	return *this;
}
Predator &Predator::turn(int offset)
{
	return *this;
}

Predator &Predator::eat(Herbivorous const &herbi)
{
	return *this;
}
Predator *Predator::reproduce()
{
	return nullptr;
}



// end
