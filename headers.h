#include "nanoflann.hpp"
#include "SFML/Graphics.hpp"

class Player;
class Point;
class PolygonPoint;
class Vector;
class Polygon;
class Polygons;
class Weapon;
class Weapons;
class Projectile;
class Entity;

//type for kd tree
using KDPolygonsTree = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<float, Polygons>,Polygons, 2>;
using KDWeaponsTree = nanoflann::KDTreeSingleIndexDynamicAdaptor<nanoflann::L2_Simple_Adaptor<float, Weapons>,Weapons, 2>;

constexpr static int screen_width = 800;
constexpr static int screen_height = 600;
constexpr static int point_radius = 2;

constexpr static unsigned int fps = 120;

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
constexpr static float AMMO = 24;
constexpr static float DAMAGE = 100;
constexpr static float MAXHEALTH = 100;
constexpr static float PICKUPRADIUSSQR = 10 * 10;
constexpr static float BULLETSPEED = 10;

//level save file
const static std::string levelFname = "level0.bin";
const static std::string playerFname = "player.bin";
const static std::string enemy1Fname = "enemy1.bin";
const static std::string weapon1Fname = "weapon1.bin";

const static std::vector<std::string> argPlayerNames = {"x coordinate", "y coordinate", "friction" , "step" , "view distance" , "shape radius" , "view angle", "max health", "max speed"};
const static unsigned long long argPlayerLength = argPlayerNames.size();

const static std::vector<std::string> argWeaponNames = {"x coordinate", "y coordinate", "ammo" , "damage", "speed"};
const static unsigned long long argWeaponLength = argWeaponNames.size();

const static char *messages[] = {"1 - Rewrite File" , "2 - Edit element" , "3 - Print File" , "0 - Exit"};

const static sf::Color defPlayerColor(sf::Color(226, 143, 40));
const static sf::Color defWeaponColor(sf::Color(65, 179, 0));

const static char codePlayer = 'p';
const static char codeWeapon = 'w';
