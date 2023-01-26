class Player;
class Point;
class Vector;
class Polygon;
class Weapon;
class Projectile;
class Entity;

#include <SFML/Graphics.hpp>

#include <vector>
#include <list>
#include <string>

using Edge = std::pair<Point, Point>;

const extern unsigned int screen_width;
const extern unsigned int screen_height;
const extern int point_radius;
const extern unsigned int fps;
const extern double epsilon;

const extern unsigned int FONTSIZE;
const extern double VIEWDISTANCE;
const extern double VIEWANGLEDEG;
const extern double SHAPERADIUS;
const extern double STEP;
const extern double FRICTION;
const extern double SPEEDLIMIT;
const extern double AMMO;
const extern double DAMAGE;
const extern double MAXHEALTH;
const extern double PICKUPRADIUSSQR;
const extern double BULLETSPEED;

//level save file
const extern std::string levelFname;
const extern std::string playerFname;
const extern std::string enemy1Fname;
const extern std::string weapon1Fname;

const extern std::string texturesDirectory;
const extern std::vector<std::string> availableWeapons;
const extern std::string projectileSprite;

const extern std::vector<std::string> argPlayerNames;
const extern unsigned long long argPlayerLength;

const extern std::vector<std::string> argWeaponNames;
const extern unsigned long long argWeaponLength;

const extern char *messages[];

const extern sf::Color defPlayerColor;
const extern sf::Color defWeaponColor;

const extern char codePlayer;
const extern char codeWeapon;
