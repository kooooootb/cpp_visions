#include "MainMenu.h"

MainMenu::MainMenu(std::shared_ptr<sf::RenderWindow> &pWindow_) :
    pWindow(pWindow_) , event()
{
    setButtons();
}

void MainMenu::setButtons() {
    float width2 = (float) pWindow->getSize().x / 2;
    float height2 = (float) pWindow->getSize().y / 2;
    float width4 = width2 / 2;

    float buttonHeight = height2 / 2;
    float spaceHeight = height2 / 8;
    sf::Color buttonColor = sf::Color(17, 0, 91, 127);

    buttons.push_back(std::make_shared<Button<MainMenu>>(*this, [](MainMenu &outer) -> void {
        outer.goToGame();
    }, -width4, -height2 + spaceHeight, width2, buttonHeight, buttonColor, "Play game"));

    buttons.push_back(std::make_shared<Button<MainMenu>>(*this, [](MainMenu &outer) -> void {
        outer.goToEditor();
    }, -width4, -height2 + spaceHeight * 2 + buttonHeight, width2, buttonHeight, buttonColor, "Go to editor"));

    buttons.push_back(std::make_shared<Button<MainMenu>>(*this, [](MainMenu &outer) -> void {
        outer.goToExit();
    }, -width4, -height2 + spaceHeight * 3 + buttonHeight * 2, width2, buttonHeight, buttonColor, "Exit"));
}

void MainMenu::goToGame() {
    chooseStatus = GAME;
}

void MainMenu::goToEditor() {
    chooseStatus = EDITOR;
}

void MainMenu::goToExit() {
    chooseStatus = EXIT;
}

void MainMenu::handleButtons(){
    sf::Vector2f mousePos = getMousePosition(*pWindow, pWindow->getView());

    // check until any button will react
    while(!std::any_of(std::begin(buttons), std::end(buttons), [&mousePos](auto &button) -> bool {
        return button->checkAndRun(mousePos);
    }));
}

void MainMenu::drawButtons(){
    for(auto &button : buttons){
        button->draw(*pWindow);
    }
}

void MainMenu::handleEvents(){
    while (pWindow->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                pWindow->close();
                break;
            case sf::Event::MouseButtonPressed: { // add some point
                if (event.mouseButton.button == sf::Mouse::Left) {
                    handleButtons();
                } else {
                    // nothing on rmb
                }
                break;
            }
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        pWindow->close();
                        break;
                    default:
                        //default keyboard button
                        break;
                }
                break;
            default:
                //default event
                break;
        }
    }
}

void MainMenu::updateWindow() {
    // clear previous frame
    pWindow->clear();

    // draw section:
    drawButtons();

    // show our work
    pWindow->display();
}

ChooseStatus MainMenu::run() {
    // reset variables
    chooseStatus = IDLE;
    sf::View view(sf::View(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(pWindow->getSize())));
    pWindow->setView(view);

    while(pWindow->isOpen()) {
        handleEvents();
        updateWindow();

        if (chooseStatus != IDLE) {
            return chooseStatus;
        }
    }
    return EXIT;
}
