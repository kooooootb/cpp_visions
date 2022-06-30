#include <vector>
#include <list>
#include <cmath>
#include <memory>
#include "SFML/Graphics.hpp"

struct Point;
class Polygon;
class Polygons;

struct Point{
	float x, y;
	
	Polygon &polygon;
	
	Point(Polygon &pol, float X, float Y) : polygon(pol) , x(X) , y(Y) {}
	
	float getDistance(const Point &point) const{
		return (float) sqrt(pow((x - point.x), 2) + pow((y - point.y), 2));
	}
};

class Polygon{
private:
	std::list<Point> boarderPoints, extraPoints;
	friend class Polygons;
	static constexpr float minDistance = 10;
	
	bool visible = false;
	
	std::shared_ptr<sf::ConvexShape> convex;
	
	void connectBPoints(const Point &bp1, const Point &bp2);
public:
	Polygon();
	void addBPoint(const Point &point);
	void endAdding();
	void setConvex();
	void makeInvisible();
	void makeVisible();
	void updateVisibility();
	const sf::ConvexShape &getConvex()const { return *convex; }
//	bool isVisible() const { return convex.getFillColor().a > 0; };
};

class Polygons{
private:
	std::list<std::shared_ptr<Polygon>> polygons;
	std::vector<Point> points;
public:
	Polygons() = default;
	
	void add(std::shared_ptr<Polygon> &polygon);
	
	bool empty() const { return polygons.empty(); }
	
	std::list<std::shared_ptr<Polygon>>::iterator begin() { return polygons.begin(); }
	std::list<std::shared_ptr<Polygon>>::iterator end() { return polygons.end(); }
	
	std::vector<std::shared_ptr<sf::Shape>> collectShapes() const;
	
	//------------------------------------------------------------------
	//Functions for nanoflann's kdtree:
	inline size_t kdtree_get_point_count() const { return points.size(); }
	
	inline float kdtree_get_pt(const size_t idx, const size_t dim) const
	{
		if (dim == 0)
			return points[idx].x;
		else
			return points[idx].y;
	}
	
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const
	{
		return false;
	}
	//------------------------------------------------------------------
	
	void hideAll();
	Point &operator[](unsigned int index);
	
	void updateVisibility() { for(auto &it : polygons) it->updateVisibility(); }
};
