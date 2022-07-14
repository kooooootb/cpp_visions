#include <iostream>

#include "Game.h"
#include "Weapon.h"
#include "Projectile.h"
#include "Polygon.h"

Game::Game() :
        fpsCounter("", font, FONTSIZE) ,
        ammoCounter("", font, FONTSIZE) ,
        polygons(std::make_shared<Polygons>(loadLevelForTree(levelFname))) ,
        polygonTree(2, *polygons, {10 /* max leaf */}) ,
        player(std::make_shared<Player>(Player::loadEntity(playerFname))) ,
        event() ,
        weaponsDataSet(std::make_shared<Weapons>(weapons)) ,
        weaponsTree(2, *weaponsDataSet, {10 /* max leaf */})
{
    initWindow();

    loadFont();
    loadEnemies();
    loadWeapons();

    setEntities();
    initWeaponsTree();
    initAmmoCounter();

    createBackground();

    polygonsShapes = polygons->collectShapes();
    players.push_back(player);
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
    players.emplace_back(std::make_shared<Player>(Player::loadEntity(enemy1Fname)));
}

void Game::loadWeapons() {
    weapons.emplace_back(std::make_shared<Weapon>(Weapon::loadEntity(weapon1Fname)));
}

void Game::setEntities() {
    for(const auto &character : players){
        entities.push_back(character);
    }

    for(const auto &weapon : weapons){
        entities.push_back(weapon);
    }

    for(const auto &projectile : projectiles){
        entities.push_back(projectile);
    }
}

void Game::run() {
    while (window->isOpen()) {
        //event handler
        handleEvents();

        //move player
        acceleratePlayer();

        updateProjectiles();

        //update player
#ifndef T5_DEBUG
        player->update(*polygons, polygonTree, sf::Mouse::getPosition(*window), entities, viewShape);
        fpsCounter.setString(std::to_string(sf::seconds(1) / clock.getElapsedTime()));
        clock.restart();
#endif

        //refresh ammo amount
        updateAmmo();

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
                    changeWeapon();
                }else{
                    shootAtPoint();
                }
                break;
            }
            case sf::Event::MouseButtonReleased:
                if(event.mouseButton.button == sf::Mouse::Right){
                    //release right mb
                }else{
                    //release left mb
                }
                break;
            case sf::Event::MouseMoved:
                if(moveByMouse){
                    mouseControl();
                }
                break;
            case sf::Event::KeyPressed:
                switch(event.key.code){
                    case sf::Keyboard::E:
                        startMouseControl();
                        break;
                    case sf::Keyboard::Escape:
                        window->close();
                        break;
                    default:
                        //default
                        break;
                }
                break;
            case sf::Event::KeyReleased:
                switch(event.key.code){
                    case sf::Keyboard::E:
                        stopMouseControl();
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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player->accelerate(0, -1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player->accelerate(-1, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player->accelerate(0, 1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player->accelerate(1, 0);
}

void Game::refreshWindow() {
    window->clear();

    //draw section
    drawAll(defShapes);//draw default shapes

#ifdef T5_DEBUG
    player->update(polygons, playerTree, sf::Mouse::getPosition(window), enemies, enemiesShapes, viewShape, window);
#endif

    drawAll(polygonsShapes);//draw forms' shapes
    drawAll(viewShape);//draw view field shape

    drawAllEntities();//draw entities

    //show current fps
    window->draw(fpsCounter);
    window->draw(ammoCounter);

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

void Game::drawAll(const std::vector<std::shared_ptr<Entity>> &ents){
    for(const auto &it : ents){
        window->draw(*(it->getShape()));
    }
}

void Game::drawAllEntities(){
    for(const auto &it : players){
        window->draw(*(it->getShape()));
    }

    for(const auto &it : weapons){
        window->draw(*(it->getShape()));
    }

    for(const auto &it : projectiles){
        window->draw(*(it->getShape()));
    }
}

void Game::shootAtPoint() {
    std::shared_ptr<Projectile> projectile = nullptr;

    if(player->shoot(players, projectile, polygonTree, *polygons)) {
        projectiles.push_back(projectile);
    }

}

void Game::startMouseControl() {
    mousePos = sf::Mouse::getPosition(*window);
    moveByMouse = true;
}

void Game::stopMouseControl() {
    moveByMouse = false;
}

void Game::mouseControl() {
    sf::Vector2i newMousePos = sf::Mouse::getPosition(*window);
    player->move(newMousePos - mousePos);
    mousePos = newMousePos;
}

void Game::changeWeapon() {
    player->changeWeapon(weaponsTree, weapons);
}

void Game::initWeaponsTree() {
    weaponsTree.addPoints(0, weapons.size() - 1);
}

void Game::updateProjectiles() {
    std::vector<int> indexes;
    bool reset = false;
    int i = 0;

    for(auto &projectile : projectiles){
        if(projectile->update()){
            indexes.push_back(i);
            reset = true;
        }

        ++i;
    }

    for(auto it : indexes){
        projectiles[it] = projectiles.back();
        projectiles.pop_back();
    }

    setEntities();
}

void Game::updateAmmo() {
    if(player->isArmed()){
        ammoCounter.setString(std::to_string(player->getAmmo()));
    }else{
        ammoCounter.setString("");
    }
}

void Game::initAmmoCounter() {
    ammoCounter.setPosition(screen_width - FONTSIZE * 5, screen_height - FONTSIZE);
}
