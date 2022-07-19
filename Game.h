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

    //shapes
    std::vector<std::shared_ptr<sf::Shape>> defShapes;//default shapes (background)
    std::list<std::shared_ptr<sf::Shape>> viewShape;//view field shape
    std::vector<std::shared_ptr<sf::Shape>> polygonsShapes;//polygons' shapes

    //level variables
    std::shared_ptr<Polygons> polygons;
    KDPolygonsTree polygonTree;

    //players
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Player>> players;

    //weapons
    std::vector<std::shared_ptr<Weapon>> weapons;
    std::shared_ptr<Weapons> weaponsDataSet;
    KDWeaponsTree weaponsTree;
    std::vector<std::shared_ptr<Projectile>> projectiles;

    //entities
    std::vector<std::shared_ptr<Entity>> entities;

    //screen settings
    int viewWidth = screen_width, viewHeight = screen_height;
    int levelWidth, levelHeight;
    sf::View view;

    //other variables
    bool moveByMouse = false;
    sf::Vector2f mousePos;
    sf::Clock clock, tracerClock;
    sf::Event event;

    void setEntities();
    void initWindow();
    void initWeaponsTree();
    void initAmmoCounter();
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
    Game();

    void run();
};
