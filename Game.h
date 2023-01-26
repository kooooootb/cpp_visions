#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <list>

#include "Player.h"

class Game {
private:
    //window settings
    std::shared_ptr<sf::RenderWindow> window;
    sf::Font font;
    sf::Text fpsCounter, ammoCounter;

    //shapes textures vertices
    sf::VertexArray backGroundVertices;
    sf::Texture backGroundTexture;
//    std::vector<std::shared_ptr<sf::Shape>> defShapes;//default shapes (background)
    sf::VertexArray viewShape;//view field shape
//    std::vector<std::shared_ptr<sf::Shape>> polygonsShapes;//polygons' shapes

    //level variables
    std::vector<std::shared_ptr<Polygon>> polygons;

    //players
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<BasePlayer>> players;

    //weapons
    std::vector<std::shared_ptr<Weapon>> weapons;
    std::vector<std::shared_ptr<Projectile>> projectiles;

    //entities
    std::vector<std::shared_ptr<Entity>> entities;

    //screen settings
    unsigned int viewWidth, viewHeight;
    unsigned int levelWidth, levelHeight;
    sf::View view;

    //other variables
    bool moveByMouse = false;
    sf::Vector2f mousePos;
    sf::Clock clock, tracerClock;
    sf::Event event;

    void setEntities();
    void loadFont();
    void setBoarders();
    void loadEnemies();
    void loadWeapons();
    void updateProjectiles();
    void updateAmmo();
    void updateViewPosition();

    void handleEvents();
    void acceleratePlayer();
    void refreshWindow();
    void createBackground();
    void shootAtPoint();
    void startMouseControl();
    void stopMouseControl();
    void mouseControl();
    void changeWeapon();

    void drawAll(const std::vector<std::shared_ptr<sf::Shape>> &shapes);
    void drawAll(const std::list<std::shared_ptr<sf::Shape>> &shapes);
    void drawAll(const std::vector<std::shared_ptr<Entity>> &entities);
    void drawAllEntities();
public:
    Game(std::shared_ptr<sf::RenderWindow> &windowPtr_);

    ~Game();

    void run();
};
