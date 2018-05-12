#ifndef SFML_FIELD_ADAPTER_HPP
#define SFML_FIELD_ADAPTER_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <ostream>

template<typename ValueType, typename SizeType = unsigned short int> struct Field
{
	typedef ValueType value_type;
	typedef SizeType size_type;
	
	value_type *d;
	size_type w, h;
};	

template<typename ValueType> class SFMLFieldAdapter
{
public:
	typedef ValueType value_type;

	SFMLFieldAdapter(sf::RenderWindow *window = nullptr, Field<value_type> const *field = nullptr): _window(window), _field(field) {}

	void draw() const;

	void updateSize();
	void updateBackground();


	sf::RenderWindow *window() const;
	auto &window(sf::RenderWindow *newwindow);

	Field<value_type> const *field() const;
	auto &field(Field<value_type> const *newfield);

	auto &printfield() const;
	
	std::function<sf::Color(value_type const &)> const &colorfun() const;
	auto &colorfun(std::function<sf::Color(value_type const &)> const &newcolorfun);

	float bordertocell() const;
	auto &bordertocell(float newbordertocell);


	bool drawBoundsEnable() const;
	auto &drawBoundsEnable(bool enable);

	float boundsWidth() const;
	auto &boundsWidth(float newboundswidth);

	sf::Color const &boundsColor() const;
	auto &boundsColor(sf::Color const &newboundscolor);

	bool drawGridEnable() const;
	auto &drawGridEnable(bool enable);

	float gridWidth() const;
	auto &gridWidth(float newgridwidth);
	
	sf::Color const &gridColor() const;
	auto &gridColor(sf::Color const &newgridcolor);

private:
	sf::RenderWindow *_window;
	Field<value_type> const *_field;
	std::function<sf::Color(value_type const &)> _colorfun = [](value_type const &)->sf::Color { return sf::Color::White; };
	sf::Vector2f _fieldoffset;
	sf::Vector2f _boundsoffset;

	float _bordersize = 0, _cellsize = 0;
	float _bordertocell = 0.1;

	sf::RenderTexture _rtexture;
	sf::Sprite _background;

	bool _drawbounds = true;
	float _boundswidth = 5;
	sf::Color _boundscolor = sf::Color::Black;

	bool _drawgrid = true;
	float _gridwidth = 4;
	sf::Color _gridcolor = sf::Color::Black;
};




template<typename ValueType> void SFMLFieldAdapter<ValueType>::draw() const
{
	_window->draw(_background);

	sf::RectangleShape rect;
	rect.setSize({_cellsize, _cellsize});
	rect.setPosition({_fieldoffset.x+_boundsoffset.x+_bordersize, _fieldoffset.y+_boundsoffset.y+_bordersize});

	for(unsigned short int y = 0; y < _field->h; ++y) {
		for(unsigned short int x = 0; x < _field->w; ++x) {
			rect.setFillColor(_colorfun(_field->d[y*_field->w+x]));
			_window->draw(rect);
			rect.move(_cellsize+2*_bordersize, 0.0);
		}
		rect.move(-_field->w*(_cellsize+2*_bordersize), _cellsize+2*_bordersize);
	}

	return;
}


template<typename ValueType> void SFMLFieldAdapter<ValueType>::updateSize()
{
	auto winsize = _window->getSize();
	if(_drawbounds) {
		winsize.x -= 2*_boundswidth;
		winsize.y -= 2*_boundswidth;
	}
	float fullcellsize = std::min(float(winsize.x)/_field->w, float(winsize.y)/_field->h);

	_cellsize = fullcellsize/(2*_bordertocell+1);
	_bordersize = (fullcellsize-_cellsize)/2;

	_fieldoffset.x = (winsize.x-_field->w*(_cellsize+2*_bordersize))/2;
	_fieldoffset.y = (winsize.y-_field->h*(_cellsize+2*_bordersize))/2;

	updateBackground();

	return;
}
template<typename ValueType> void SFMLFieldAdapter<ValueType>::updateBackground()
{
	_rtexture.create(_window->getSize().x, _window->getSize().y);

	if(_drawbounds)
		_boundsoffset = {_boundswidth, _boundswidth};
	else
		_boundsoffset = {0, 0};

	if(_drawgrid) {
		sf::RectangleShape rect;
		rect.setFillColor(_gridcolor);

		// Horizontal
		rect.setSize({_field->w*(_cellsize+2*_bordersize), _gridwidth});
		rect.setPosition({_fieldoffset.x+_boundsoffset.x, _fieldoffset.y+_boundsoffset.y-_gridwidth/2});
		_rtexture.draw(rect);
		for(unsigned short int y = 0; y < _field->h; ++y) {
			rect.move(0.0, _cellsize+2*_bordersize);
			_rtexture.draw(rect);
		}

		// Vertical
		rect.setSize({_gridwidth, _field->h*(_cellsize+2*_bordersize)});
		rect.setPosition({_fieldoffset.x+_boundsoffset.x-_gridwidth/2, _fieldoffset.y+_boundsoffset.y});
		_rtexture.draw(rect);
		for(unsigned short int x = 0; x < _field->w; ++x) {
			rect.move(_cellsize+2*_bordersize, 0.0);
			_rtexture.draw(rect);
		}
	}

	if(_drawbounds) {
		sf::RectangleShape rect;
		rect.setFillColor(_boundscolor);
		
		// Horizontal
		rect.setSize({_field->w*(_cellsize+2*_bordersize)+2*_boundswidth, _boundswidth});
		rect.setPosition(_fieldoffset);
		_rtexture.draw(rect);
		rect.setPosition({_fieldoffset.x, _fieldoffset.y+_boundswidth+_field->h*(_cellsize+2*_bordersize)});
		_rtexture.draw(rect);

		// Verical
		rect.setSize({_boundswidth, _field->h*(_cellsize+2*_bordersize)+2*_boundswidth});
		rect.setPosition(_fieldoffset);
		_rtexture.draw(rect);
		rect.setPosition({_fieldoffset.x+_boundswidth+_field->w*(_cellsize+2*_bordersize), _fieldoffset.y});
		_rtexture.draw(rect);

	}

	_rtexture.display();
	_background.setTexture(_rtexture.getTexture(), true);

	return;
}



template<typename ValueType> sf::RenderWindow *SFMLFieldAdapter<ValueType>::window() const
{
	return _window;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::window(sf::RenderWindow *newwindow)
{
	_window = newwindow;
	return *this;
}

template<typename ValueType> Field<typename SFMLFieldAdapter<ValueType>::value_type> const *SFMLFieldAdapter<ValueType>::field() const
{
	return _field;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::field(Field<value_type> const *newfield)
{
	_field = newfield;
	return *this;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::printfield() const
{
	for(unsigned int y = 0; y < _field->h; ++y) {
		for(unsigned int x = 0; x < field.w; ++x) {
			std::cout << int(_field[y*_field->w+x]) << ' ';
		}
		std::cout << std::endl;
	}
}

template<typename ValueType> std::function<sf::Color(typename SFMLFieldAdapter<ValueType>::value_type const &)> const &SFMLFieldAdapter<ValueType>::colorfun() const
{
	return _colorfun;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::colorfun(std::function<sf::Color(value_type const &)> const &newcolorfun)
{
	_colorfun = newcolorfun;
	return *this;
}

template<typename ValueType> float SFMLFieldAdapter<ValueType>::bordertocell() const
{
	return _bordertocell;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::bordertocell(float newbordertocell)
{
	_bordertocell = newbordertocell;
	return *this;
}

template<typename ValueType> bool SFMLFieldAdapter<ValueType>::drawBoundsEnable() const
{
	return _drawbounds;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::drawBoundsEnable(bool enable)
{
	_drawbounds = enable;
	updateBackground();
	return *this;
}

template<typename ValueType> float SFMLFieldAdapter<ValueType>::boundsWidth() const
{
	return _boundswidth;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::boundsWidth(float newboundswidth)
{
	_boundswidth = newboundswidth;
	updateBackground();
	return *this;
}

template<typename ValueType> sf::Color const &SFMLFieldAdapter<ValueType>::boundsColor() const
{
	return _boundscolor;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::boundsColor(sf::Color const &newboundscolor)
{
	_boundscolor = newboundscolor;
	updateBackground();
	return *this;
}


template<typename ValueType> bool SFMLFieldAdapter<ValueType>::drawGridEnable() const
{
	return _drawgrid;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::drawGridEnable(bool enable)
{
	_drawgrid = enable;
	updateBackground();
	return *this;
}

template<typename ValueType> float SFMLFieldAdapter<ValueType>::gridWidth() const
{
	return _gridwidth;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::gridWidth(float newgridwidth)
{
	_gridwidth = newgridwidth;
	return *this;
}


template<typename ValueType> sf::Color const &SFMLFieldAdapter<ValueType>::gridColor() const
{
	return _gridcolor;
}
template<typename ValueType> auto &SFMLFieldAdapter<ValueType>::gridColor(sf::Color const &newgridcolor)
{
	_gridcolor = newgridcolor;
	updateBackground();
	return *this;
}
	

#endif // SFML_FIELD_ADAPTER_HPP
