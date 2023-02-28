#include "MainMenu.h"
#include <fstream>

MainMenu::MainMenu(std::shared_ptr<sf::RenderWindow> &pWindow_, std::string &chosenFileName_) :
    pWindow(pWindow_) , event() , chosenFileName(chosenFileName_)
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
    // ask for file's name
    float width3 = (float) pWindow->getSize().x / 3;
    float height3 = (float) pWindow->getSize().y / 3;
    float x = -width3;
    float y = -height3;
    float width = 2 * width3;
    float height = 2 * height3;
    sf::Color color = sf::Color(33, 85, 0, 160);

    promptQueue.emplace(*this, [](MainMenu &outer, const sf::String &sfString) -> void {
            outer.chosenFileName = sfString.toAnsiString();
            outer.chooseStatus = EDITOR;
        }, [](MainMenu &outer) -> void{

        }, [](MainMenu &outer) -> void {
        // nothing, just close prompt;
    }, [](const sf::String &sfString) -> bool {
//        std::fstream fstream(sfString.toAnsiString(), std::ios::app);
//        bool result = fstream.is_open();
//        fstream.close(); // just to be sure
//        return result;
        return true;
    }, x, y, width, height, color, "Input level's file name", chosenFileName);
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

void MainMenu::handlePromptClick() {
    Prompt<MainMenu> &prompt = promptQueue.front();
    sf::Vector2f mousePos = getMousePosition(*pWindow, pWindow->getView());
    if(prompt.handleClick(mousePos)){
        promptQueue.pop();
    }
}

void MainMenu::drawButtons(){
    for(const auto &button : buttons){
        button->draw(*pWindow);
    }
}

void MainMenu::drawPrompt() {
    if(promptQueue.empty()){
        return;
    }

    const Prompt<MainMenu> &prompt = promptQueue.front();
    prompt.draw(*pWindow);
}

void MainMenu::handleNormalEvent() {
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

void MainMenu::handlePromptEvent() {
    // work with enter or escape presses
    if(event.type == sf::Event::KeyPressed){
        switch(event.key.code) {
            case sf::Keyboard::Enter:
                promptQueue.front().approve();
                return;
            case sf::Keyboard::Escape:
                promptQueue.pop();
                return;
        }
    }

    // other events
    switch(event.type){
        case sf::Event::Closed:
            pWindow->close();
            break;
        case sf::Event::TextEntered:
            promptQueue.front().addInput(event.text.unicode);
            break;
        case sf::Event::MouseButtonPressed:
            handlePromptClick();
            break;
        default:
            //default event
            break;
    }
}

void MainMenu::handleEvents(){
    while (pWindow->pollEvent(event)) {
        if(promptQueue.empty()){
            // work normally
            handleNormalEvent();
        } else{
            // handle prompt only
            handlePromptEvent();
        }
    }
}

void MainMenu::updateWindow() {
    // clear previous frame
    pWindow->clear();

    // draw section:
    drawButtons();
    drawPrompt();

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
