#include <ctime>
#include <iostream>
#include <random>

#include "SFMLFieldAdapter.hpp"

using namespace sf;
using namespace std;

typedef unsigned int value_type;

VideoMode dstopmode = VideoMode::getDesktopMode();
RenderWindow window(dstopmode, "Cellular Automaton", Style::None);
int const FRAMERATE_LIMIT = 120;
Event event;

Field<value_type> field{nullptr, 0, 0};
SFMLFieldAdapter<value_type> fieldadapter;
auto colorfun = [](value_type value)->Color
{
	switch(value) {
	case 0:
		return Color::White;
	case 1:
		return Color::Red;
	default:
		return Color::Black;
	}
};

void init_field(value_type defvalue = 0)
{
	static unsigned short int DEFAULT_WIDTH = 40, DEFAULT_HEIGHT = 30;
	if(field.d)
		delete[] field.d;
	field.w = DEFAULT_WIDTH;
	field.h = DEFAULT_HEIGHT;
	field.d = new value_type[field.w*field.h];
	for(unsigned short int i = 0; i < field.w*field.h; ++i) {
		field.d[i] = defvalue;
	}
	return;
}
	

int main(int argc, char const *argv[])
{
	srand(time(0));
	window.setPosition({0, 0});
	window.setFramerateLimit(FRAMERATE_LIMIT);

	init_field();
	for(unsigned int i = 0; i < field.w*field.h; ++i) {
		field.d[i] = rand()%3;
	}

	fieldadapter.window(&window);
	fieldadapter.field(&field);
	fieldadapter.colorfun(colorfun);
	fieldadapter.bordertocell(0.1);
	fieldadapter.gridWidth(20);
	fieldadapter.gridColor(Color::Blue);
	fieldadapter.drawBoundsEnable(false);
	fieldadapter.updateSize();


	while(window.isOpen()) {
		if(window.pollEvent(event)) {
			if(event.type == Event::KeyPressed) {
				switch(event.key.code) {
				case Keyboard::C:
					window.close();
					break;
				default:
					break;
				}
			}
		}

		window.clear(Color::Yellow);
		fieldadapter.draw();
		window.display();
	}

	return 0;
}
