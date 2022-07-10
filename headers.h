class Player;
class Point;
class PolygonPoint;
class Vector;
class Polygon;
class Polygons;

//#define T5_DEBUG
using Edge = std::pair<Point, Point>;
constexpr static float epsilon = 0.05;
constexpr static float VIEWDISTANCE = 100;
constexpr static float VIEWANGLEDEG = 40;
constexpr static float SHAPERADIUS = 6;
constexpr static float STEP = 1;
constexpr static float FRICTION = 0.5;