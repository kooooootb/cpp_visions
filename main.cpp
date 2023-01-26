#include "Game.h"

std::shared_ptr<sf::RenderWindow> initWindow(unsigned int viewWidth, unsigned int viewHeight) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    std::shared_ptr<sf::RenderWindow> windowPtr = std::make_shared<sf::RenderWindow>(sf::VideoMode(viewWidth, viewHeight), std::to_string(fps), sf::Style::Default, settings);
    windowPtr->setFramerateLimit(fps);

    return windowPtr;
}

int main(){
    srand(time(nullptr));

    std::shared_ptr<sf::RenderWindow> window = initWindow(screen_width, screen_height);

	Game game(window);
    game.run();
	
	return 0;
}