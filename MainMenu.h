#ifndef T5_MAINMENU_H
#define T5_MAINMENU_H

#include "Common.h"
#include "Button.h"
#include "Prompt.h"

#include <queue>

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
    friend class Prompt<MainMenu>;

    // we need to choose: play game or edit level
    ChooseStatus chooseStatus = IDLE;

    // we need prompt queue to be able to ask for file name
    std::queue<Prompt<MainMenu>> promptQueue;

    // event holder
    sf::Event event;

    // file name of level that will run after choosing
    std::string &chosenFileName;

    void setButtons();
    void handleButtons();
    void handlePromptClick();
    void drawButtons();
    void drawPrompt();
    void handleNormalEvent();
    void handlePromptEvent();

    void goToGame();
    void goToEditor();
    void goToExit();

    void handleEvents();
    void updateWindow();
public:
    MainMenu(std::shared_ptr<sf::RenderWindow> &pWindow_, std::string &chosenFileName_);

    ChooseStatus run();
};


#endif //T5_MAINMENU_H
