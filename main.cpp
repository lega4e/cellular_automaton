#include <iomanip>
#include <iostream>

#include <clever/IostreamFunctions.hpp>
#include <clever/SFML/Widgets.hpp>

#include "bot.hpp"
#include "botfield.hpp"
#include "CellPrinter.hpp"

using namespace clever;
using namespace sf;
using namespace std;



RenderWindow window;
VideoMode dsmode = VideoMode::getDesktopMode();
string const TITLE = "Cellular Automaton";
unsigned int const FRAMERATE_LIMIT = 60;
Color const WINDOW_BACKGROUND_COLOR = Color(0xde, 0xaa, 0x88);

Event event;

unsigned int const K_FIELD_SIZE = 120; 
float const K_PANEL_HEIGHT = 0.075f; 

FieldAdapter<Cell, CellPrinter<Cell>> fieldadapter;

BotField field;
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
	Field<Cell> f;
	f.w = 60;
	f.h = 30;
	f.d = new Cell[f.w*f.h];
	field.setField(move(f));
	field.getField().clear({false, nullptr});
	return;
}

void init_fieldadapter()
{
	fieldadapter.setField(&field.getField());
	fieldadapter.setSize(
		{float(dsmode.width), float(dsmode.height)}
	);
	fieldadapter.setDrawBoundsEnable(false);
	fieldadapter.setGridColor(Color(0x47, 0x00, 0x27));

	return;
}



void print_field(Field<Cell> const &f)
{
	for(unsigned int y = 0; y < f.h; ++y) {
		for(unsigned int x = 0; x < f.w; ++x) {
			if(f.at(x, y).bot) {
				cout << 'b';
			}
			else if(f.at(x, y).haveFood) {
				cout << 'e';
			}
			else
				cout << '0';
			cout << ' ';
		}
		cout << '\n';
	}
	return;
}

void update()
{
	field.update();
	++age;
	agestatus.change(string("Age: ") + to_string(age));
	return;
}

void print_bot(Bot const &bot)
{
	cout << setprecision(2);
	cout.setf(ostream::ios_base::fixed);
	cout << "Chromosome: \n" << bot.getChromosome() << endl;
	cout << "Bot: \n" << bot << endl;
	return;
}

void print_info(float x, float y) 
{
	auto p = fieldadapter.cursorOn(sf::Vector2f({x, y})-fieldadapter.getPosition());
	if(p.first == -1 || !field.getField().at(p.first, p.second).bot)
		return;
	print_bot(
		*field.getField().at(p.first, p.second).bot
	);
}



int main(int argc, char const *argv[])
{
	init_window();
	init_field();
	init_fieldadapter();


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
	bool right;
	
	while(window.isOpen()) {
		if(window.pollEvent(event)) {
			switch(event.type) {
			case Event::KeyPressed:
				switch(event.key.code) {
				case Keyboard::C:
					window.close();
					break;
				case Keyboard::R:
					field.getField().
						clear({false, nullptr});
					age = 0;
					agestatus.change("Age: 0");
					lifestatus.change("Life: 0");
					break;
				case Keyboard::U:
					update();
					break;
				case Keyboard::I:
					print_info(
						Mouse::getPosition().x,
						Mouse::getPosition().y
					);
					break;
				default:
					break;
				}
			default:
				break;
			}
		}
		

		right = false;
		if(Mouse::isButtonPressed(Mouse::Button::Left) ||
			!(right = true) || Mouse::isButtonPressed(Mouse::Button::Right)) {
			auto pos = fieldadapter.cursorOn(
				conversion<float>(Mouse::getPosition())-
				fieldadapter.getPosition()
			);
			if(pos.first != -1) {
				if(!right)
					field.getField().
						at(pos.first, pos.second).
							bot = new Bot;
				else
					field.getField().at(pos.first, pos.second).
						haveFood = true;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Space)) {
			update();
		}


		mainpanel->update(0.0f);
		window.clear(WINDOW_BACKGROUND_COLOR);
		window.draw(*mainpanel);
		window.display();
	}

	return 0;
}



// end.
