#include "Game.h"
#include "MainMenu.h"
#include "LevelEditor.h"

std::shared_ptr<sf::RenderWindow> initWindow(unsigned int viewWidth, unsigned int viewHeight) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    std::shared_ptr<sf::RenderWindow> windowPtr = std::make_shared<sf::RenderWindow>(sf::VideoMode(viewWidth, viewHeight), std::to_string(fps), sf::Style::Default, settings);
    windowPtr->setFramerateLimit(fps);

    return windowPtr;
}

int main(){
    srand(time(nullptr));

    std::shared_ptr<sf::RenderWindow> pWindow = initWindow(screen_width, screen_height);
    std::string chosenFileName = levelFname;

    MainMenu mm(pWindow, chosenFileName);

    while(pWindow->isOpen()){
        ChooseStatus decision = mm.run();

        switch(decision){
            case GAME:{
                //	Game game(pWindow);
                //    game.run();

                break;
            }
            case EDITOR:{
                LevelEditor levelEditor(pWindow, chosenFileName);
                levelEditor.run();

                break;
            }
            case EXIT:{
                if(pWindow->isOpen()){
                    pWindow->close();
                }

                return 0;
            }
            default:
                throw std::runtime_error("met default case in menu switcher");
        }
    }
}