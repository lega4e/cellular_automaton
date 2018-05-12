#include <iostream>

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;


RenderWindow window(VideoMode::getDesktopMode(), "Cellular Automaton", Style::None);
Color backgroundcolor = Color::White;
Event event;

unsigned short int width = 30, height = 15;
uint8_t *field;

Color cellcolors[2] = {
	Color(0xff, 0x66, 0x66),
	Color(0xff, 0x0, 0x0)
};
uint8_t lastcolor = 0;

float sidelen, borderlen;
float kborderatsize = 0.05;
Vector2f printpos = { 0.0, 0.0 };
RectangleShape rectangle;

void print_field()
{
	for(unsigned short int y = 0; y < height; ++y) {
		for(unsigned short int x = 0; x < width; ++x) {
			cout << field[y*width+x] << ' ';
		}
		cout << '\n';
	}
	return;
}

void print_info()
{
	std::cout << "window.size = <" << window.getSize().x << ", " << window.getSize().y << ">" << std::endl;
	std::cout << "width = " << width << ", height = " << height << std::endl;
	print_field();
	std::cout << "lastcolor = " << lastcolor << std::endl;
	std::cout << "sidelen = " << sidelen << ", borderlen = " << borderlen << std::endl;
	std::cout << "printpos = <" << printpos.x << ", " << printpos.y << ">" << std::endl;
	return;
}
	

void init_field(uint8_t defvalue = 0)
{
	if(field)
		delete[] field;
	field = new uint8_t[width*height];
	for(unsigned short int i = 0; i < width*height; ++i) {
		field[i] = defvalue;
	}
	return;
}

void init_rectangle()
{
	float fulllen;
	auto winsize = window.getSize();
	fulllen = min(float(winsize.x)/width, float(winsize.y)/height);
	
	sidelen = fulllen/(1+2*kborderatsize);
	borderlen = (fulllen-sidelen)/2;

	printpos.x = (winsize.x-fulllen*width)/2;
	printpos.y = (winsize.y-fulllen*height)/2;

	rectangle.setSize({sidelen, sidelen});
	rectangle.setFillColor(cellcolors[lastcolor]);
	return;
}

void init_all()
{
	init_field();
	init_rectangle();
}

void draw_field()
{
	rectangle.setPosition(printpos.x+borderlen, printpos.y+borderlen);
	for(unsigned short int y = 0; y < height; ++y) {
		rectangle.setPosition(printpos.x+borderlen, printpos.y+borderlen+y*(sidelen+borderlen*2));
		for(unsigned short int x = 0; x < width; ++x) {
			if(field[y*width+x]) {
				if(lastcolor != field[y*width+x]-1) {
					lastcolor = field[y*width+x]-1;
					rectangle.setFillColor(cellcolors[lastcolor]);
				}
				window.draw(rectangle);
			}
			rectangle.move(sidelen+borderlen, 0.0);
		}
	}
}

void sfml_update_field()
{
	int toset = 2;
	if(Mouse::isButtonPressed(Mouse::Left) || (toset = 0) || Mouse::isButtonPressed(Mouse::Right)) {
		auto mousepos = Mouse::getPosition();
		int x = (mousepos.x-printpos.x)/(sidelen+borderlen);
		if(x < 0 || x >= width)
			return;
		int y = (mousepos.y-printpos.y)/(sidelen+borderlen);
		if(y < 0 || y >= height)
			return;
		field[y*width+x] = toset;
	}
	return;
}

uint8_t tape_at(unsigned short int x, unsigned short int y)
{
	if(x < 0) do
		x += width;
	while(x < 0);
	else while(x >= width)
		x -= width;

	if(y < 0) do
		y += height;
	while(y < 0);
	else while(y >= height)
		y -= height;

	return field[y*width+x];
}

uint8_t simple_at(unsigned short int x, unsigned short int y)
{
	if(x < 0 || x >= width || y < 0 || y >= height)
		return 0;
	else 
		return field[y*width+x];
}

uint8_t (*at)(unsigned short int, unsigned short int) = &tape_at;

uint8_t life_predicate(unsigned short int x, unsigned short int y)
{
	unsigned short int c = 0;
	if(at(x-1, y-1))
		++c;
	if(at(x, y-1))
		++c;
	if(at(x+1, y-1))
		++c;

	if(at(x-1, y))
		++c;
	if(at(x+1, y))
		++c;
	

	if(at(x-1, y+1))
		++c;
	if(at(x, y+1))
		++c;
	if(at(x+1, y+1))
		++c;

	if(at(x, y))
		return c == 2 || c == 3 ? 1 : 0;
	else
		return c == 3 ? 1 : 0;
}

uint8_t hard_life_predicate(unsigned short int x, unsigned short int y)
{
	unsigned short int c = 0;
	c += at(x-1, y-1);
	c += at(x, y-1);
	c += at(x+1, y-1);

	c += at(x-1, y);
	c += at(x+1, y);

	c += at(x-1, y+1);
	c += at(x, y+1);
	c += at(x+1, y+1);
	
	if(at(x, y) == 2)
		return c >= 4 && c <= 6 ? 2 : 1;
	else if(at(x, y) == 1)
		return c >= 5 && c <= 7 ? 2 : 0;
	else 
		return c == 6 || c == 7 ? 1 : 0;
}

uint8_t (*predicate)(unsigned short int, unsigned short int) = &hard_life_predicate;

void two_dimensional_update_field()
{
	static unsigned short int lastwidth = width, lastheight = height;
	static uint8_t *swapfield = new uint8_t[lastwidth*lastheight];
	if(lastwidth != width || lastheight != height) {
		delete[] swapfield;
		lastwidth = width;
		lastheight= height;
		swapfield = new uint8_t[lastwidth*lastheight];
	}
	for(unsigned short int y = 0; y < lastheight; ++y) {
		for(unsigned short int x = 0; x < lastwidth; ++x) {
			swapfield[y*lastwidth+x] = predicate(x, y);
		}
	}
	std::swap(field, swapfield);
	return;
}

void (*update_field)() = &two_dimensional_update_field;

int main(int argc, char const *argv[])
{
	init_all();
	window.setPosition({0, 0});
	window.setFramerateLimit(120);

	while(window.isOpen()) {
		if(window.pollEvent(event)) {
			switch(event.type) {
			case Event::KeyPressed:
				switch(event.key.code) {
				case Keyboard::C: case Keyboard::Escape:
					window.close();
					break;
				case Keyboard::R:
					init_field();
				default:
					update_field();
					break;
				}
			default:
				break;
			}
		}

		sfml_update_field();

		window.clear(backgroundcolor);
		draw_field();
		window.display();
	}
	return 0;
}
