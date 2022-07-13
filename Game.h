#include <memory>
#include <vector>
#include <list>

#include "Polygon.h"
#include "Player.h"

class Game {
private:
    //window settings
    std::shared_ptr<sf::RenderWindow> window;
    sf::Font font;
    sf::Text fpsCounter;

    //shapes
    std::vector<std::shared_ptr<sf::Shape>> defShapes;//default shapes (background)
    std::list<std::shared_ptr<sf::Shape>> playersShapes;//players' shapes
    std::list<std::shared_ptr<sf::Shape>> viewShape;//view field shape
    std::vector<std::shared_ptr<sf::Shape>> polygonsShapes;//polygons' shapes

    //level variables
    Polygons polygons;
    my_kd_tree_t polygonTree;

    //players
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Player>> enemies;

    //weapons
    std::vector<std::shared_ptr<Weapon>> weapons;

    //entities
    std::vector<std::shared_ptr<Entity>> entities;

    //tracer
    sf::VertexArray tracer;
    const sf::Time tracerTime = sf::seconds(TRACERSHOWTIME);

    //other variables
    bool moveByMouse = false;
    sf::Vector2i mousePos;
    sf::Clock clock, tracerClock;
    sf::Event event;

    void setEntities();
    void initWindow();
    void initTracer();
    void loadFont();
    void loadEnemies();
    void loadWeapons();

    void handleEvents();
    void acceleratePlayer();
    void refreshWindow();
    void showTracer();
    void createBackground();
    void setEntitiesShapes();

    void drawAll(const std::vector<std::shared_ptr<sf::Shape>> &shapes);
    void drawAll(const std::list<std::shared_ptr<sf::Shape>> &shapes);
public:
    Game();

    void run();
};
