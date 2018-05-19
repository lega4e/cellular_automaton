#ifndef INT_PRINTER_HPP
#define INT_PRINTER_HPP

#include <SFML/Graphics.hpp>

template<typename ValueType> class CellPrinter
{
public:
	typedef ValueType value_type;

	CellPrinter(
		value_type value = 0,
		sf::Vector2f const &size = {0.0f, 0.0f},
		sf::Vector2f const &position = {0.0f, 0.0f}
	): _value(value)
	{
		_value_dispatch();
		_rectangle.setSize(size);
		_rectangle.setPosition(position);
		return;
	}
	~CellPrinter() {}


	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states = sf::RenderStates::Default
	) const
	{
		target.draw(_rectangle, states);
		return;
	}

	value_type getValue() const
	{
		return _value;
	}
	void setValue(value_type value)
	{
		if(_value != value) {
			_value = value;
			_value_dispatch();
		}
		return;
	}

	sf::Vector2f getSize() const
	{
		return _rectangle.getSize();
	}
	void setSize(sf::Vector2f const &newsize)
	{
		_rectangle.setSize(newsize);
		return;
	}

	sf::Vector2f const &getPosition() const
	{
		return _rectangle.getPosition();
	}
	void setPosition(sf::Vector2f const &newposition)
	{
		_rectangle.setPosition(newposition);
		return;
	}
	
	void move(float offsetx, float offsety)
	{
		_rectangle.move(offsetx, offsety);
		return;
	}
	void move(sf::Vector2f const &offset)
	{
		_rectangle.move(offset.x, offset.y);
		return;
	}

private:
	void _value_dispatch()
	{
		if(_value)
			_rectangle.setFillColor(sf::Color(0x8a, 0x45, 0x13));
		else
			_rectangle.setFillColor(sf::Color::Transparent);
		return;
	}

	value_type _value;
	sf::RectangleShape _rectangle;

};


#endif // INT_PRINTER_HPP
