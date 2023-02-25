#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <cmath>
#include <iostream>
#include <fstream>

#include "Polygon.h"
#include "Common.h"


// extern variables:
const unsigned int screen_width = 800;
const unsigned int screen_height = 600;
const int point_radius = 2;
const unsigned int fps = 60;
const double epsilon = 0.19;//very small number

const unsigned int FONTSIZE = 12;
const double VIEWDISTANCE = 300;
const double VIEWANGLEDEG = 40;
const double SHAPERADIUS = 6;
const double STEP = 1;
const double FRICTION = 0.5;
const double SPEEDLIMIT = 3;
const double AMMO = 24;
const double DAMAGE = 100;
const double MAXHEALTH = 100;
const double PICKUPRADIUSSQR = 20 * 20;
const double BULLETSPEED = 10;

const std::string levelFname = "level2.bin";
const std::string playerFname = "player.bin";
const std::string enemy1Fname = "enemy1.bin";
const std::string weapon1Fname = "weapon1.bin";

const std::string texturesDirectory = "textures\\";
const std::vector<std::string> availableWeapons = { "ak47" , "m4" };
const std::string projectileSprite = "ak47.png";

const std::vector<std::string> argPlayerNames = {"x coordinate", "y coordinate", "friction" , "step" , "view distance" , "shape radius" , "view angle", "max health", "max speed"};
const unsigned long long argPlayerLength = argPlayerNames.size();

const std::vector<std::string> argWeaponNames = {"ammo" , "damage", "bullet speed"};
const unsigned long long argWeaponLength = argWeaponNames.size();

const char *messages[] = {"1 - Rewrite File" , "2 - Edit element" , "3 - Print File" , "0 - Exit"};

const sf::Color defPlayerColor(sf::Color(226, 143, 40));
const sf::Color defWeaponColor(sf::Color(65, 179, 0));

const char codePlayer = 'p';
const char codeWeapon = 'w';

int initViewSector(sf::VertexArray &viewShape, const Point &center, double viewDistance, double currentAngle, double viewAngle){
    if(viewAngle > currentAngle){
        viewAngle -= 2 * M_PI;
    }
    viewAngle = currentAngle - viewAngle;

	int dotsAmount = (int) (viewDistance * viewAngle);//dots in arc

    if(dotsAmount < 1){
        return 2;
    }

    size_t index = viewShape.getVertexCount();
    size_t topIndex = index + dotsAmount;
    viewShape.resize(topIndex);
	double dAngle = viewAngle / (double) (dotsAmount + 1);

	for(;index < topIndex;++index){
        viewShape[index].position = sf::Vector2f((float) (center.x + viewDistance * cos(currentAngle)),
                                                 (float) (center.y + viewDistance * sin(currentAngle)));
		
		currentAngle -= dAngle;
	}

    return dotsAmount;
}

Point Point::operator+(const Vector &vector) const {
    return { x + vector.x, y + vector.y };
}

Point Point::operator-(const Vector &vector) const {
    return { x - vector.x, y - vector.y };
}

Point &Point::operator+=(const Vector &vector) {
    x += vector.x;
    y += vector.y;

    return *this;
}

Point &Point::operator+=(const sf::Vector2f &vector) {
    x += (double) vector.x;
    y += (double) vector.y;

    return *this;
}

Vector operator-(const Point &point, const sf::Vector2f &vector) {
    return { point.x - vector.x, point.y - vector.y };
}

Vector operator-(const Point &point, const sf::Vector2i &vector) {
    return { point.x - vector.x, point.y - vector.y };
}

double Point::distSqr(const Point &point) const {
    return std::pow((x - point.x), 2) + std::pow((y - point.y), 2);
}

double Point::getDistance(const Point &point) const {
    return (double) std::sqrt(distSqr(point));
}

bool Point::operator!=(const Point &point) const {
    return x != point.x || y != point.y;
}

bool Point::operator==(const sf::Vector2i &vector) const {
    return x == vector.x && y == vector.y;
}

bool Point::operator==(const Point &point) const {
    return x == point.x && y == point.y;
}

double Point::sqr() const {
    return x * x + y * y;
}

sf::Vector2f Point::toSfVector() const {
    return {(float) x, (float) y};
}

Point calcClosestPoint(const Edge &edge, const Point &point){
    if (edge.first.x == edge.second.x){
        return { edge.first.x, point.y };
    }
    if (edge.first.y == edge.second.y){
        return { point.x, edge.first.x };
    }
    double m1 = (edge.second.y - edge.first.y) / (edge.second.x - edge.first.x);
    double m2 = -1 / m1;
    double x = (m1 * edge.first.x - m2 * point.x + point.y - edge.first.y) / (m1 - m2);
    return { x, m2 * (x - point.x) + point.y };
}

bool findClosestEdge(const std::vector<Edge> &edges, Edge &edge, const Point &point){
    if(edges.empty()){
        return false;
    }

    double minDistSqr = Vector(point, calcClosestPoint(edges.front(), point)).sqr();
    const Edge *edgePtr = &edges.front();

    for(auto it = edges.cbegin() + 1, endIt = edges.end();it < endIt;++it){
        double distSqr = Vector(point, calcClosestPoint(*it, point)).sqr();

        if(distSqr < minDistSqr){
            edgePtr = &(*it);
            minDistSqr = distSqr;
        }
    }

    edge = *edgePtr;
    return true;
}

Point findNearestPoint(const Point &center, const std::vector<Point> &points){
    double distSqr = std::numeric_limits<double>::max(), newDistSqr;
    Point res(0, 0);

    for(const auto &point : points){
        newDistSqr = Vector(center, point).sqr();
        if(newDistSqr < distSqr){
            res = point;
            distSqr = newDistSqr;
        }
    }

    return res;
}

std::vector<Point> arcSegmentVSLineIntersection(const Point &p1, const Point &p2, const Point &center, double radius, const std::array<Vector, 3> &views){
    //|X^2 - center^2| = radius^2
    //X(t) = p1 + td , (t from 0 to 1)
    //delta = p1 - center
    Vector d(p1, p2);
    Vector delta(center, p1);

    std::vector<Point> res;
    res.reserve(2);

    auto isInSight = [&](const Point &point) -> bool {
        Vector pV(center, point);
        if(pV * views[1] < 0){
            return false;
        }else if(views[0].cross(pV) * pV.cross(views[2]) > 0){
            return true;
        }else{
            return false;
        }
    };

    double discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

    if(discriminant < 0){
        return res;
    }else if(discriminant == 0){
        double t = (-1 * (d * delta)) / (d.sqr());
        if(t >= 0 && t <= 1){
            Point p(p1 + t * d);
            if(isInSight(p)){
                res.push_back(p);
            }
        }
        return res;
    }else{
        discriminant = std::sqrt(discriminant);

        double t;
        t = ((-1 * (d * delta)) + discriminant) / (d.sqr());
        if(t >= 0 && t <= 1){
            Point p(p1 + t * d);
            if(isInSight(p)){
                res.push_back(p);
            }
        }
        t = ((-1 * (d * delta)) - discriminant) / (d.sqr());
        if(t >= 0 && t <= 1){
            Point p(p1 + t * d);
            if(isInSight(p)){
                res.push_back(p);
            }
        }

        return res;
    }
}

std::vector<Point> circleVSLineIntersection(const Point &p1, const Point &p2, const Point &center, double radius){//vector is limited (p2)
    //|X^2 - center^2| = radius^2
    //X(t) = p1 + td , (t from 0 to 1)
    //delta = p1 - center
    Vector d(p1, p2);
    Vector delta(center, p1);

    std::vector<Point> res;

    double discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

    if(discriminant < 0){
        return res;
    }else if(discriminant == 0){
        double t = (-1 * (d * delta)) / (d.sqr());
        if(t >= 0){
            Point p(p1 + t * d);
            res.push_back(p);
        }
        return res;
    }else{
        discriminant = std::sqrt(discriminant);

        double t;
        t = ((-1 * (d * delta)) + discriminant) / (d.sqr());
        if(t >= 0 && t <= 1){
            Point p(p1 + t * d);
            res.push_back(p);
        }
        t = ((-1 * (d * delta)) - discriminant) / (d.sqr());
        if(t >= 0 && t <= 1){
            Point p(p1 + t * d);
            res.push_back(p);
        }

        return res;
    }
}

bool circleVSLineIntersectionCheck(const Point &p1, const Point &p2, const Point &center, double radius){//check if line segment intersects circle 2 times
    //if line segment can intersect circle only 1 time check if 1 of its points inside circle
    Vector d(p1, p2);
    Vector delta(center, p1);

    std::vector<Point> res;

    double discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

    if(discriminant < 0){
        return false;
    }else{
        double t = (-1 * (d * delta)) / (d.sqr());
        if(t >= 0 && t <= 1){
            return true;
        }
        else{
            return false;
        }
    }
}

std::vector<Point> twoLinesVSLineIntersection(const Point &p1, const Point &p2, const Point &center, const std::array<Vector, 3> &views){
    //1 line: p1 + s * d0
    const Vector d0(p1, p2);
    //2 line: center + t * views[0]
    //3 line: center + t * views[2]
    //p1 == P0 ; center == P1

    const Vector delta(p1, center);

    std::vector<Point> res;

    for(int i = 0;i < 3;i += 2){//for views[0] and views[2]
        const Vector &d1 = views[i];
        double denominator = d0 * (d1.perpendicular());

        if(denominator != 0){
            double t = (delta * (d0.perpendicular())) / denominator;
            double s = (delta * (d1.perpendicular())) / denominator;
            if(t >= 0 && t <= 1 && s >= 0 && s <= 1){
                Point cross = center + (t * d1);
                res.emplace_back(cross);
            }
        }
    }

    return res;
}

bool unlimitedLineVSLineIntersection(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1, Point &res){//d0 is unlimited
    const Vector delta(p1, p3);

    bool hit = false;

    double denominator = d0 * (d1.perpendicular());

    if(denominator != 0){
        double t = (delta * (d0.perpendicular())) / denominator;
        double s = (delta * (d1.perpendicular())) / denominator;
        if(t >= 0 && t <= 1 && s >= 0){
            res = p3 + (t * d1);
            hit = true;
        }
    }

    return hit;
}

bool lineVSLineIntersection(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1, Point &res){//d0 is unlimited
    const Vector delta(p1, p3);

    bool hit = false;

    double denominator = d0 * (d1.perpendicular());

    if(denominator != 0){
        double t = (delta * (d0.perpendicular())) / denominator;
        double s = (delta * (d1.perpendicular())) / denominator;
        if(t >= 0 && t <= 1 && s >= 0 && s <= 1){
            res = p3 + (t * d1);
            hit = true;
        }
    }

    return hit;
}

bool lineVSLineIntersectionCheck(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1){//only checks if its intersecting
    const Vector delta(p1, p3);

    bool hit = false;

    double denominator = d0 * (d1.perpendicular());

    if(denominator != 0){
        double t = (delta * (d0.perpendicular())) / denominator;
        double s = (delta * (d1.perpendicular())) / denominator;
        if(t >= 0 && t <= 1 && s >= 0 && s <= 1){
            hit = true;
        }
    }

    return hit;
}

bool vectorVSEdgesIntersectionCheck(const Point &center, const Vector &vector, const std::vector<Edge> &edges){
    bool hit = false;

    for (const auto &edge: edges) {
        if (lineVSLineIntersectionCheck(center, vector, edge.first, Vector(edge.first, edge.second))) {
            hit = true;
        }
    }

    return hit;
}

std::vector<Point> vectorVSEdgesIntersection(const Point &center, const Vector &vector, const std::vector<Edge> &edges){
    Point temp;
    std::vector<Point> res;

    for (const auto &edge: edges) {
        if (lineVSLineIntersection(center, vector, edge.first, Vector(edge.first, edge.second), temp)) {
            res.push_back(temp);
        }
    }

    return res;
}

void writeFname(std::string &fname, std::ofstream &fd){
    size_t size = fname.size();
    fd.write(reinterpret_cast<char *>(&size), sizeof(size));
    fd.write(reinterpret_cast<char *>(&fname[0]), sizeof(fname[0]) * size);
}

void writeFname(std::ofstream &fd){
    std::string fname;
    std::cout << "Input entity's sprite file name:";
    std::cin >> fname;

    writeFname(fname, fd);
}

std::string readFname(std::ifstream &fd){
    std::string res;

    size_t size;

    fd.read(reinterpret_cast<char *>(&size), sizeof(size));

    res.resize(size);

    fd.read(reinterpret_cast<char *>(&res[0]), sizeof(char) * size);

    return res;
}

sf::Vector2f getMousePosition(sf::RenderWindow &window, const sf::View &view){
    window.setView(view);
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

Point rayCheck(const Point &center, double radius, const Vector &pointVector, const std::vector<Edge> &blockingEdges){
    double maxR = radius * radius;
    bool hit = false;
    Point hitPoint(0, 0);
    for(const auto &edge : blockingEdges){
        Point intersection{};
        if(unlimitedLineVSLineIntersection(center, pointVector, edge.first, Vector(edge.first, edge.second),
                                           intersection)){
            Vector v(center, intersection);
            if(v.sqr() < maxR){
                hitPoint = intersection;
                hit = true;
                maxR = v.sqr();
            }
        }
    }

    if(hit){
        Vector res(center, hitPoint);
        if(res.sqr() > radius * radius) {
            double length = res.length();
            res.increase(radius / length);
            return center + res;
        }
    }else{
        Vector res(pointVector);
        double length = res.length();
        res.increase(radius / length);
        return center + res;
    }

    return hitPoint;
}

bool isOutOfBoarders(const Vector &pointVector, const std::array<Vector, 3> &views){
    return (views[0].cross(pointVector) * pointVector.cross(views[2]) < 0);
}

double Vector::operator*(const Point &point) const {
    return (x * point.x + y * point.y);
}

bool Vector::operator!=(const Vector &vector) const {
    return x != vector.x || y != vector.y;
}

double Vector::cross(const Point &point) const {
    return (x * point.y - y * point.x);
}

double Vector::cross(const Vector &vector) const {
    return (x * vector.y - y * vector.x);
}

Vector Vector::operator/(double k) const {
    return { x / k, y / k };
}

Vector operator*(const Vector &v, double k) {
    return { v.x * k, v.y * k };
}

Vector operator*(double k, const Vector &v) {
    return { v.x * k, v.y * k };
}

double getAngleToZero(const Vector &a) {
    if(a.x == 0 && a.y == 0){
        return 0.0;
    }else if(a.x > 0){
        return atan((double) a.y / a.x);
    }else{
        return atan((double) a.y / a.x) + M_PI;
    }
}

Vector Vector::perpendicular() const {
    return { y, -x };
}

double Vector::sqr() const { return (std::pow(x, 2) + std::pow(y, 2)); }

double Vector::length() const { return std::sqrt(sqr()); }

void Vector::increase(double k) { x *= k; y *= k; }

void Vector::move(double dx, double dy) { x += dx; y += dy; }

void Vector::rotate(double radians) {
    double cs = cos(radians);
    double sn = sin(radians);

    double newX = x * cs - y * sn;
    y = x * sn + y * cs;
    x = newX;
}

sf::Texture createPlainTexture(unsigned int width, unsigned int height, const sf::Color &color){
    sf::Texture texture;
    texture.create(width, height);

//    unsigned int pixelsSize = bgTextHeight * bgTextWidth;
    unsigned int pixelsSize = 1;
    auto *pixels = new sf::Uint8[pixelsSize * 4 /*for RGBA*/], *curPixel = pixels;
    for(unsigned int i = 0;i < pixelsSize;++i){
        curPixel[0] = color.r; // R
        curPixel[1] = color.g; // G
        curPixel[2] = color.b; // B
        curPixel[3] = color.a; // A

        curPixel += 4;
    }

    texture.update(pixels, 1, 1, 0, 0);

    delete [] pixels;

    return texture;
}

void createBackground(sf::VertexArray &bgVertices, sf::Texture &bgTexture, unsigned int width, unsigned int height){
    bgVertices[0].position = sf::Vector2f(-100, -100);
    bgVertices[1].position = sf::Vector2f((float) width + 100, -100);
    bgVertices[2].position = sf::Vector2f((float) width + 100, (float) height + 100);
    bgVertices[3].position = sf::Vector2f(-100, (float) height + 100);

    unsigned int bgTextWidth = width + 200, bgTextHeight = height + 200;
    bgTexture = createPlainTexture(bgTextWidth, bgTextHeight, sf::Color(228, 228, 228, 255));

    bgVertices[0].texCoords = sf::Vector2f(0, 0);
    bgVertices[1].texCoords = sf::Vector2f(0, 0);
    bgVertices[2].texCoords = sf::Vector2f(0, 0);
    bgVertices[3].texCoords = sf::Vector2f(0, 0);
}

bool almost_equal(double x, double y, int ulp) {
    return std::abs(x-y) <= std::numeric_limits<float>::epsilon() * std::abs(x+y) * static_cast<float>(ulp)
    || std::abs(x-y) < std::numeric_limits<float>::min();
}

bool almost_equal(const Point &p1, const Point &p2) {
    return almost_equal(p1.x, p2.x) && almost_equal(p1.y, p2.y);
}

bool almost_equal(const Edge &e1, const Edge &e2){
    return almost_equal(e1.first, e2.first) && almost_equal(e1.second, e2.second) ||
            almost_equal(e1.first, e2.second) && almost_equal(e1.second, e2.first);
}

std::string getStringFromWindow(sf::RenderWindow &window, const std::string &message){
    sf::String sfInput;
    sf::Event event{};
    while(window.isOpen()){
        while (window.pollEvent(event)){
            switch(event.type){
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::TextEntered: {
                    int keycode = event.key.code;
                    if (keycode >= '0' && keycode <= '9') sfInput += event.text.unicode;
                    break;
                }
                case sf::Event::KeyPressed: {
                    switch (event.key.code) {
                        case sf::Keyboard::Enter: {
                            return sfInput.toAnsiString();
                        }
                    }
                }
            }
        }
    }
    return "";
}

// add 4 rectangles to polygons
void addBoarders(PolygonArray &polygons, BoarderType &boarders){
    int d = 10;
    int width = (int) boarders.first;
    int height = (int) boarders.second;
    polygons.push_back(std::make_shared<Polygon>(std::vector<Point>({{-d, -d}, {0, -d}, {0, height + d}, {-d, height + d}})));
    polygons.push_back(std::make_shared<Polygon>(std::vector<Point>({{-d, 0}, {-d, -d}, {width, -d}, {width, 0}})));
    polygons.push_back(std::make_shared<Polygon>(std::vector<Point>({{width, -d}, {width + d, -d}, {width + d, height +d}, {width, height + d}})));
    polygons.push_back(std::make_shared<Polygon>(std::vector<Point>({{width + d, height}, {width + d, height + d}, {-d, height + d}, {-d, height}})));
}

std::vector<Point> vertexArrayToPoints(sf::VertexArray &v){
    std::vector<Point> points;
    points.reserve(v.getVertexCount());
    for(int i = 0;i < v.getVertexCount();++i){
        points.emplace_back(v[i].position);
    }
    return points;
}

sf::VertexArray createGridVertices(int width, int height, float precision){
    sf::VertexArray grid(sf::Lines);
    float x = 0, y = 0;
    // vertical
    while(x < width){
        grid.append(sf::Vector2f(x, 0));
        grid.append(sf::Vector2f(x, height));
        x += precision;
    }
    // horizontally
    while(y < height){
        grid.append(sf::Vector2f(0, y));
        grid.append(sf::Vector2f(width, y));
        y += precision;
    }

    return grid;
}

sf::VertexArray createGridVertices(BoarderType::first_type width, BoarderType::second_type height, float precision){
    sf::VertexArray grid(sf::Lines);
    float x = 0, y = 0;
    // vertical
    while(x < width){
        grid.append(sf::Vector2f(x, 0));
        grid.append(sf::Vector2f(x, height));
        x += precision;
    }
    // horizontally
    while(y < height){
        grid.append(sf::Vector2f(0, y));
        grid.append(sf::Vector2f(width, y));
        y += precision;
    }

    return grid;
}

sf::CircleShape setAimPointShape(float radius){
    sf::CircleShape cs(radius);

    cs.setFillColor(sf::Color(205, 110, 0, 255));
    cs.setOrigin(cs.getRadius(), cs.getRadius());
    return cs;
}

void applyFriction(sf::Vector2f &vector, float friction){
    bool movingForward;
    friction += (vector.x + vector.y) / 500;

    movingForward = vector.x > 0;
    vector.x += movingForward ? -friction : friction;
    if(vector.x > 0 != movingForward){
        vector.x = 0;
    }

    movingForward = vector.y > 0;
    vector.y += movingForward ? -friction : friction;
    if(vector.y > 0 != movingForward){
        vector.y = 0;
    }
}

bool checkKeyPressed(std::initializer_list<sf::Keyboard::Key> keys){
    return std::any_of(std::begin(keys), std::end(keys), [](sf::Keyboard::Key key) -> bool {
        return sf::Keyboard::isKeyPressed(key);
    });
}















