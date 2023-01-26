#include <iostream>

#include "Game.h"
#include "Weapon.h"
#include "Projectile.h"
#include "Polygon.h"

Game::Game(std::shared_ptr<sf::RenderWindow> &window_) :
        window(window_) ,
        viewWidth(window_->getSize().x) ,
        viewHeight(window_->getSize().y) ,
        fpsCounter("", font, FONTSIZE) ,
        ammoCounter("", font, FONTSIZE) ,
        polygons(loadLevel(levelFname)) ,
        player(std::make_shared<Player>(Player::loadPlayer(playerFname))) ,
        event() ,
        view(sf::FloatRect(0, 0, (float) viewWidth, (float) viewHeight)) ,
        viewShape(sf::TriangleFan) ,
        backGroundVertices(sf::Quads, 4)
{
    loadFont();
    setBoarders();

    loadEnemies();
    loadWeapons();

    setEntities();

    createBackground();

    players.push_back(player);
    window->setView(view);
}

void Game::run() {
    while (window->isOpen()) {
        //event handler
        handleEvents();

        //move player
        acceleratePlayer();

        //move projectiles
        updateProjectiles();

        //update player
        player->update(polygons, getMousePosition(*window), entities);
/*
        fpsCounter.setString(std::to_string(sf::seconds(1) / clock.getElapsedTime()));
        clock.restart();
*/

        //refresh view position
        updateViewPosition();

        //refresh ammo amount
        updateAmmo();

        //draw shapes
        refreshWindow();
    }
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
    backGroundVertices[0].position = sf::Vector2f(-100, -100);
    backGroundVertices[1].position = sf::Vector2f((float) levelWidth + 100, -100);
    backGroundVertices[2].position = sf::Vector2f((float) levelWidth + 100, (float) levelHeight + 100);
    backGroundVertices[3].position = sf::Vector2f(-100, (float) levelHeight + 100);

    unsigned int bgTextWidth = levelWidth + 200, bgTextHeight = levelHeight + 200;

    backGroundTexture.create(bgTextWidth, bgTextHeight);

//    unsigned int pixelsSize = bgTextHeight * bgTextWidth;
    unsigned int pixelsSize = 1;
    auto *pixels = new sf::Uint8[pixelsSize * 4 /*for RGBA*/], *curPixel = pixels;
    for(unsigned int i = 0;i < pixelsSize;++i){
        curPixel[0] = 228; // R
        curPixel[1] = 228; // G
        curPixel[2] = 228; // B
        curPixel[3] = 255; // A

        curPixel += 4;
    }

//    backGroundTexture.update(pixels);
    backGroundTexture.update(pixels, 1, 1, 0, 0);

    delete [] pixels;

    backGroundVertices[0].texCoords = sf::Vector2f(0, 0);
    backGroundVertices[1].texCoords = sf::Vector2f(0, 0);
    backGroundVertices[2].texCoords = sf::Vector2f(0, 0);
    backGroundVertices[3].texCoords = sf::Vector2f(0, 0);
}

void Game::loadEnemies() {
    players.emplace_back(std::make_shared<Enemy>(Enemy::loadEnemy(enemy1Fname)));
}

void Game::loadWeapons() {
    std::vector<std::pair<std::string, std::vector<float>>> argsArray;

    for(int i = 1;true;++i){//load all weapon files
        std::pair<std::string, std::vector<float>> args = Weapon::loadEntity("weapon" + std::to_string(i) + ".bin");
        if(args.second.empty()){
            break;
        }else{
            argsArray.push_back(args);
        }
    }

    std::vector<Point> weaponSps = loadWeaponSpawnpoints(levelFname);
    for(const auto &point : weaponSps){
        int choice = rand() % argsArray.size();

        weapons.emplace_back(std::make_shared<Weapon>(argsArray[choice], point));
    }
}

void Game::setEntities() {
    entities.clear();

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
            case sf::Event::Resized:
                view.setSize(event.size.width, event.size.height);
                viewWidth = event.size.width;
                viewHeight = event.size.height;
                window->setView(view);
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
    window->draw(backGroundVertices, &backGroundTexture); // draw background

    //draw polygons
    for(const auto &polygon : polygons){
        polygon->drawShape(*window);
    }

    //draw player's area of view
    window->draw(player->getViewShape());

    drawAllEntities();//draw entities

    //show current fps
    window->draw(fpsCounter);

    //draw current ammo
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
        it->draw(*window);
    }

    for(const auto &it : weapons){
        it->draw(*window);
    }

    for(const auto &it : projectiles){
        it->draw(*window);
    }
}

void Game::shootAtPoint() {
    std::shared_ptr<Projectile> projectile = nullptr;

    if(player->shoot(players, projectile, polygons)) {
        projectiles.push_back(projectile);
    }
}

void Game::startMouseControl() {
    mousePos = getMousePosition(*window);
    moveByMouse = true;
}

void Game::stopMouseControl() {
    moveByMouse = false;
}

void Game::mouseControl() {
    sf::Vector2f newMousePos = getMousePosition(*window);
    player->move(newMousePos - mousePos);
    mousePos = newMousePos;
}

void Game::changeWeapon() {
    player->changeWeapon(weapons);
}

void Game::updateProjectiles() {
    std::vector<int> indexes;
    int i = 0;

    for(auto &projectile : projectiles){
        if(projectile->update()){
            indexes.push_back(i);
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
        sf::Vector2f screenVector((float) viewWidth / 2 - 20, (float) viewHeight / 2 - 20);
        ammoCounter.setPosition(view.getCenter() + screenVector);

        ammoCounter.setString(std::to_string(player->getAmmo()));
    }else{
        ammoCounter.setString("");
    }
}

void Game::setBoarders() {
    std::pair<int, int> boards = loadBoarders(levelFname);

    levelWidth = boards.first;
    levelHeight = boards.second;
}

void Game::updateViewPosition() {
    const Point &pp = player->getPosition();

//    (LS - P.X)(LS - SS)/LS = LS - S.X
    double lw = levelWidth;
    double lh = levelHeight;

    Vector dp = Point(viewWidth / 2., viewHeight / 2.) - sf::Mouse::getPosition(*window);

    double sy = (lh - (lh - 2*pp.y)*(1 - (viewHeight - 40)/lh))/2 - dp.y;
    double sx = (lw - (lw - 2*pp.x)*(1 - (viewWidth - 40)/lw))/2 - dp.x;

    view.setCenter((float) sx, (float) sy);
    window->setView(view);
}

Game::~Game(){

}
