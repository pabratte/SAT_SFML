#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>

/**
 * Test if two sprites overlap using Separating Axis Theorem (http://www.dyn4j.org/2010/01/sat/),
 * optionally can receive a pointer to a sf::Vector2f to output the MTV (Minimum Translation Vector)
 * to correct the first sprite position
 * \param sp1 First sprite
 * \param sp2 Second sprite
 * \param out_mtv Pointer to a sf::Vector2f in wich to receive the MTV
 * \return true if overlapping exist, false otherwise
 */
bool sat_test(const sf::Sprite &sp1, const sf::Sprite &sp2, sf::Vector2f *out_mtv = nullptr){
	const sf::FloatRect &rectSp1 = sp1.getGlobalBounds();
	const sf::FloatRect &rectSp2 = sp2.getGlobalBounds();
	float proj_x, proj_y, overlap_x, overlap_y;
	
	// test overlap in x axis
	proj_x = std::max(rectSp1.left+rectSp1.width, rectSp2.left+rectSp2.width)-std::min(rectSp1.left, rectSp2.left);
	if(proj_x < rectSp1.width+ rectSp2.width){
		if(out_mtv){
			// calculate mtv in x
			overlap_x = rectSp1.width+ rectSp2.width - proj_x;
		}
		// test overlap in y axis
		proj_y = std::max(rectSp1.top+rectSp1.height, rectSp2.top+rectSp2.height)-std::min(rectSp1.top, rectSp2.top);
		if(proj_y < rectSp1.height+ rectSp2.height){
			if(out_mtv){
				// calculate mtv in y
				overlap_y = rectSp1.height+ rectSp2.height - proj_y;
				out_mtv->x = out_mtv->y = 0;
				
				// choose minimun overlap
				if(overlap_x < overlap_y){
					out_mtv->x = overlap_x * (rectSp1.left < rectSp2.left?-1:1);
				}else{
					out_mtv->y = overlap_y * (rectSp1.top < rectSp2.top?-1:1);
				}
			}
			return true;
		}
	}
	return false;
}




int main(int argc, char *argv[]){
	const float spVel = 2;
	const unsigned windowWidth = 640;
	const unsigned windowHeight = 400;
	
	
	sf::RenderWindow w(sf::VideoMode(windowWidth, windowHeight), "Separating Axis Theorem Demo");
	
	sf::Texture tex;
	tex.loadFromFile("quad.png");
	
	sf::Sprite sp1;
	sp1.setTexture(tex);
	sp1.setColor(sf::Color::Red);
	
	srand(time(0));
	std::vector<sf::Sprite> walls;
	walls.resize(80);
	for(auto &wall: walls){
		wall.setTexture(tex);
		wall.setColor(sf::Color::Blue);
		wall.setPosition(32*(rand()%(windowWidth/32)), 32*(rand()%(windowHeight/32)));
	}
	
	w.setFramerateLimit(60);
	while(w.isOpen()) {
		sf::Event e;
		while(w.pollEvent(e)) {
			if(e.type == sf::Event::Closed)
				w.close();
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
			sp1.move(-spVel, 0);
		}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			sp1.move(spVel, 0);
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			sp1.move(0, -spVel);
		}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			sp1.move(0, spVel);
		}
		
		sp1.setColor(sf::Color::Green);
		sf::Vector2f mtv;
		for(auto &wall: walls){
			if(sat_test(sp1, wall, &mtv)){
				sp1.move(mtv);
				sp1.setColor(sf::Color::Red);
			}
		}
		
		
		w.clear(sf::Color::White);
		for(auto &wall: walls){
			w.draw(wall);
		}
		w.draw(sp1);
		w.display();
	}
	return 0;
}

