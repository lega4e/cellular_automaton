#include <ctime>
#include <iostream>
#include <random>

#include <clever/IostreamFunctions.hpp>
#include <clever/SFML/Widgets.hpp>

#include "BotField.hpp"
#include "CellPrinter.hpp"

using namespace clever;
using namespace sf;
using namespace std;



BotField botfield;

RenderWindow window;
VideoMode dsmode = VideoMode::getDesktopMode();
string const TITLE = "Cellular Automaton";
unsigned int const FRAMERATE_LIMIT = 60;
Color const WINDOW_BACKGROUND_COLOR = Color(0xe6, 0x8e, 0x50);

Event event;

template<> 
void CellPrinter<Cell>::_value_dispatch()
{
	switch(_value.what) {
	case Cell::nothing:
		_rectangle.setFillColor(Color::Transparent);
		break;
	case Cell::plant:
		_rectangle.setFillColor(Color(0x13, 0x88, 0x08));
		break;
	case Cell::herbivorous:
		_rectangle.setFillColor(Color(0x8a, 0x45, 0x13));
		break;
	default:
		throw "invalid cell";
	}
	return;
}

FieldAdapter<Cell, CellPrinter<Cell>> fieldadapter;
int age = 0;

void init_window()
{
	window.create(dsmode, TITLE, Style::None);
	window.setPosition({0, 0});
	window.setFramerateLimit(FRAMERATE_LIMIT);
	return;
}

void init_botfield()
{
	botfield.setField({80, 40});
	return;
}

void init_fieldadapter()
{
	fieldadapter.setField(&botfield.getField());
	fieldadapter.setSize(
		{float(dsmode.width), float(dsmode.height-200)}
	);
	fieldadapter.setDrawBoundsEnable(false);
	fieldadapter.setGridColor(Color(0x47, 0x00, 0x27));

	return;
}




int main(int argc, char const *argv[])
{
	srand(time(0));
	init_window();
	init_botfield();
	init_fieldadapter();

	WidgetFactory factory;

	ChangingObject<string> agestatus("Age: 0");

	auto agestatusbar = factory.createStatusBar(1, &agestatus);
	agestatusbar->updateSpace(2.0f, 1.0f);
	auto mainpanel = factory.createLayout(
		0, Layout::vertical, {&fieldadapter, agestatusbar}
	);

	while(window.isOpen()) {
		if(window.pollEvent(event)) {
			switch(event.type) {
			case Event::KeyPressed:
				switch(event.key.code) {
				case Keyboard::C:
					window.close();
					break;
				case Keyboard::R:
					botfield.clear();
					age = 0;
					agestatus.change("Age: "+to_string(age));
					break;
				case Keyboard::U:
					botfield.update();
					++age;
					agestatus.change("Age: "+to_string(age));
					break;
				default:
					break;
				}
			default:
				break;
			}
		}
		

		if(Mouse::isButtonPressed(Mouse::Button::Left)) {
			auto pos = fieldadapter.cursorOn(
				conversion<float>(Mouse::getPosition())-
				fieldadapter.getPosition()
			);
			if(pos.first != -1) {
				if(botfield.getField().at(pos).what == Cell::herbivorous) {
					botfield.getField().at(pos).to<Herbivorous>()->
						print(cout) << endl;
				}
				else {
					botfield.addThing(
						pos.first, pos.second,
						{Cell::herbivorous, new Herbivorous}
					);
				}
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Space)) {
			botfield.update();
			++age;
			agestatus.change("Age: "+to_string(age));
		}

		mainpanel->update(0.0f);
		window.clear(WINDOW_BACKGROUND_COLOR);
		window.draw(*mainpanel);
		window.display();
	}

	return 0;
}



// end.
