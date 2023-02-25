#ifndef T5_MAINMENU_H
#define T5_MAINMENU_H

#include "Common.h"
#include "Button.h"

enum ChooseStatus{
    IDLE,
    GAME,
    EDITOR,
    EXIT
};

class MainMenu {
private:
    // in fact, window
    std::shared_ptr<sf::RenderWindow> pWindow;

    // and vector of buttons
    std::vector<std::shared_ptr<Button<MainMenu>>> buttons;
    friend class Button<MainMenu>;

    // we need to choose: play game or edit level
    ChooseStatus chooseStatus = IDLE;

    // event holder
    sf::Event event;

    void setButtons();
    void handleButtons();
    void drawButtons();

    void goToGame();
    void goToEditor();
    void goToExit();

    void handleEvents();
    void updateWindow();
public:
    MainMenu(std::shared_ptr<sf::RenderWindow> &pWindow_);

    ChooseStatus run();
};


#endif //T5_MAINMENU_H
