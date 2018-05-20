#include <iostream>

#include <clever/IostreamFunctions.hpp>
#include <clever/SFML/Widgets.hpp>

#include "CellPrinter.hpp"
#include "CellularAutomaton.hpp"

using namespace clever;
using namespace sf;
using namespace std;


typedef int8_t value_type;

RenderWindow window;
VideoMode dsmode = VideoMode::getDesktopMode();
string const TITLE = "Cellular Automaton";
unsigned int const FRAMERATE_LIMIT = 60;
Color const WINDOW_BACKGROUND_COLOR = Color(0xe6, 0x8e, 0x50);

Event event;

Field<value_type> field{0, 0, nullptr};
unsigned int const K_FIELD_SIZE = 30; 
float const K_PANEL_HEIGHT = 0.075f; 

FieldAdapter<value_type, CellPrinter<value_type>> fieldadapter;
CellularAutomaton automaton;
unsigned int age = 0; 

ChangingObject<string> lifestatus("Life: 0"), agestatus("Age:  0");

void init_window()
{
	window.create(dsmode, TITLE, Style::None);
	window.setPosition({0, 0});
	window.setFramerateLimit(FRAMERATE_LIMIT);
	return;
}


void init_field()
{
	if(field.d)
		delete field.d;
	field.w = dsmode.width/K_FIELD_SIZE;
	field.h = (dsmode.height*(1-K_PANEL_HEIGHT))/K_FIELD_SIZE;
	field.d = new value_type[field.w*field.h];

	field.clear();
	
	return;
}

void init_fieldadapter()
{
	fieldadapter.setField(&field);
	fieldadapter.setSize(
		{float(dsmode.width), float(dsmode.height)}
	);
	fieldadapter.setDrawBoundsEnable(false);
	fieldadapter.setGridColor(Color(0x47, 0x00, 0x27));

	return;
}

void init_automaton()
{
	automaton.field(&field);
	automaton.atType(CellularAutomaton::simpleat);
	return;
}



void update()
{
	automaton.update();
	++age;
	agestatus.change(string("Age: ") + to_string(age));
	lifestatus.change(
		string("Life: ")+
		to_string(automaton.alive())
	);
	return;
}

int main(int argc, char const *argv[])
{
	init_window();
	init_field();
	init_fieldadapter();
	init_automaton();


	WidgetFactory factory;

	auto lifebar = factory.createStatusBar(2, &lifestatus);
	lifebar->updateSpace(2.0f, 0.0f);
	auto agebar = factory.createStatusBar(2, &agestatus);
	lifebar->updateSpace(2.0f, 0.0f);

	auto statuspanel = factory.createLayout(
		0, Layout::horizontal, {lifebar, agebar}
	);
	auto mainpanel = factory.createLayout(
		0, Layout::vertical, {&fieldadapter, statuspanel}
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
					field.clear();
					age = 0;
					agestatus.change("Age: 0");
					lifestatus.change("Life: 0");
					break;
				case Keyboard::U:
					update();
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
				field.at(pos.first, pos.second) = 1;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Space))
			update();


		mainpanel->update(0.0f);
		window.clear(WINDOW_BACKGROUND_COLOR);
		window.draw(*mainpanel);
		window.display();
	}

	return 0;
}



// end.
