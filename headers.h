#include "nanoflann.hpp"
#include "SFML/Graphics.hpp"

class Player;
class Point;
class PolygonPoint;
class Vector;
class Polygon;
class Polygons;
class Weapon;
class Entity;

//type for kd tree
using my_kd_tree_t = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<float, Polygons>,Polygons, 2>;

constexpr static int screen_width = 800;
constexpr static int screen_height = 600;
constexpr static int point_radius = 2;

constexpr static unsigned int fps = 60;

constexpr static unsigned int FONTSIZE = 12;

//#define T5_DEBUG
using Edge = std::pair<Point, Point>;
constexpr static float epsilon = 0.05;
constexpr static float VIEWDISTANCE = 300;
constexpr static float VIEWANGLEDEG = 40;
constexpr static float SHAPERADIUS = 6;
constexpr static float STEP = 1;
constexpr static float FRICTION = 0.5;
constexpr static float SPEEDLIMIT = 3;
constexpr static float TRACERSHOWTIME = 0.05;
constexpr static float AMMO = 24;
constexpr static float DAMAGE = 100;
constexpr static float MAXHEALTH = 100;

//level save file
const static std::string levelFname = "level0.bin";
const static std::string playerFname = "player.bin";
const static std::string enemy1Fname = "enemy1.bin";
const static std::string weapon1Fname = "weapon1.bin";

const static char *argPlayerNames[] = {"friction" , "step" , "viewDistance" , "shapeRadius" , "viewAngle"};
const static int argPlayerLength = sizeof(argPlayerNames) / sizeof(argPlayerNames[0]);

const static char *argWeaponNames[] = {"ammo" , "damage"};
const static int argWeaponLength = sizeof(argWeaponNames) / sizeof(argWeaponNames[0]);

const static char *messages[] = {"1 - Rewrite File" , "2 - Edit element" , "3 - Print File" , "0 - Exit"};

const static sf::Color defPlayerColor(sf::Color(226, 143, 40));
const static sf::Color defWeaponColor(sf::Color(65, 179, 0));

const static char codePlayer = 'p';
const static char codeWeapon = 'w';
