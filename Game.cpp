#include <iostream>

#include "Game.h"

Game::Game() :
        fpsCounter("", font, FONTSIZE) ,
        polygons(loadLevelForTree(levelFname)) ,
        tree(2, polygons, {10 /* max leaf */}) ,
        tracerTime(sf::seconds(TRACERSHOWTIME)) ,
        player(Player::loadCharacter(playerFname)) ,
        tracer(sf::LineStrip, 2) ,
        event()
{
    initWindow();
    initTracer();

    loadFont();
    loadEnemies();

    createBackground();

    setEntitiesShapes();

    polygonsShapes = polygons.collectShapes();
}

void Game::initWindow() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(screen_width, screen_height), std::to_string(fps), sf::Style::Default, settings);
    window->setFramerateLimit(fps);
}

void Game::loadFont() {
    if(!font.loadFromFile("arial.ttf")){
        std::string fname;
        while(!font.loadFromFile(fname)) {
            std::cout << "Can't open font file. Input another font.ttf file name:";
            std::getline(std::cin, fname);
        }
    }
}

void Game::createBackground(){
    std::shared_ptr<sf::RectangleShape> backgroundShape = std::make_shared<sf::RectangleShape>(sf::Vector2f(screen_width, screen_height));

    backgroundShape->setPosition(0, 0);
//	backgroundShape->setFillColor(sf::Color(228, 228, 228));
    backgroundShape->setFillColor(sf::Color(10, 10, 10));

    defShapes.push_back(backgroundShape);
}

void Game::loadEnemies() {
    enemies.emplace_back(std::make_shared<Player>(Player::loadCharacter(enemy1Fname)));
}

void Game::initTracer() {
    tracer[0].color = sf::Color(30, 30, 30);
    tracer[1].color = sf::Color(30, 30, 30);
}

void Game::run() {
    while (window->isOpen()) {
        //event handler
        handleEvents();

        //move player
        acceleratePlayer();

        //update player
#ifndef T5_DEBUG
        clock.restart();
        player.update(polygons, tree, sf::Mouse::getPosition(*window), enemies, viewShape);
        fpsCounter.setString(std::to_string(sf::seconds(1) / clock.getElapsedTime()));
#endif

        //draw shapes
        refreshWindow();
    }
}

void Game::handleEvents() {
    while (window->pollEvent(event)) {
        switch (event.type){
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::MouseButtonPressed: {
                if(event.mouseButton.button == sf::Mouse::Right){
                    mousePos = sf::Mouse::getPosition(*window);
                    moveByMouse = true;
                }else{
                    std::shared_ptr<Player> temp = nullptr;
                    Point shootAt = sf::Mouse::getPosition(*window);

                    if(player.shoot(enemies, shootAt, temp)){
                        if(temp->takeDamage(player.getDamage())){
                            playersShapes.erase(std::find(playersShapes.begin(), playersShapes.end(), temp->getShape()));
                            enemies.erase(std::find(enemies.begin(), enemies.end(), temp));
                        }
                    }

                    tracer[0].position = { player.getPosition().x, player.getPosition().y };
                    tracer[1].position = { shootAt.x, shootAt.y };
                    tracerClock.restart();
                }
                break;
            }
            case sf::Event::MouseButtonReleased:
                moveByMouse = false;
                break;
            case sf::Event::MouseMoved:
                if(moveByMouse){
                    sf::Vector2i newMousePos = sf::Mouse::getPosition(*window);
                    player.move(newMousePos - mousePos);
                    mousePos = newMousePos;
                }
                break;
            case sf::Event::KeyPressed:
                switch(event.key.code){
                    case sf::Keyboard::Escape:
                        window->close();
                        break;
                    default:
                        //default
                        break;
                }
                break;
            default:
                //default
                break;
        }
    }
}

void Game::acceleratePlayer() {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player.accelerate(0, -1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.accelerate(-1, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player.accelerate(0, 1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.accelerate(1, 0);
}

void Game::refreshWindow() {
    window->clear();

    //draw section
    drawAll(defShapes);//draw default shapes

    //show current fps
    window->draw(fpsCounter);

#ifdef T5_DEBUG
    player.update(polygons, tree, sf::Mouse::getPosition(window), enemies, enemiesShapes, viewShape, window);
#endif

    drawAll(polygonsShapes);//draw forms' shapes
    drawAll(viewShape);//draw view field shape
    showTracer();
    drawAll(playersShapes);//draw enemies' shapes

    window->display();
}


void Game::drawAll(const std::vector<std::shared_ptr<sf::Shape>> &shapes){
    for(const auto &it : shapes){
        window->draw(*it);
    }
}
void Game::drawAll(const std::list<std::shared_ptr<sf::Shape>> &shapes){
    for(const auto &it : shapes){
        window->draw(*it);
    }
}

void Game::showTracer() {
    if(tracerClock.getElapsedTime() < tracerTime){
        window->draw(tracer);
    }
}

void Game::setEntitiesShapes() {
    playersShapes.push_back(player.getShape());

    for(const auto &enemy : enemies){
        playersShapes.push_back(enemy->getShape());
    }
}
