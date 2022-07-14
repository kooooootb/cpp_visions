#include <vector>
#include <list>
#include <cmath>
#include <memory>

#include "Common.h"

class PolygonPoint : public Point{
public:
    Polygon &polygon;
    PointType type = outside;

    PolygonPoint(Polygon &pol, float X, float Y) : polygon(pol) , Point(X, Y) {}
};

class Polygon{
private:
	std::list<PolygonPoint> boarderPoints, extraPoints;
	friend class Polygons;
	static constexpr float minDistance = 10;
	
	bool visible = false;
	
	std::shared_ptr<sf::ConvexShape> convex;
	
	void connectBPoints(const PolygonPoint &bp1, const PolygonPoint &bp2);
public:
	Polygon();
	void addBPoint(const PolygonPoint &point);
	void endAdding();
	void setConvex();
	void makeInvisible();
	void makeVisible();

    void setPointsType(const Player &player, const std::array<Vector, 3> &vectors);

	const sf::ConvexShape &getConvex()const { return *convex; }
//	bool isVisible() const { return convex.getFillColor().a > 0; };
};

class Polygons{
private:
	std::list<std::shared_ptr<Polygon>> polygons;
	std::vector<PolygonPoint> points;
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
    std::vector<Edge> collectEdges() const;
	PolygonPoint &operator[](unsigned int index);
	
	void updateVisibility(Player &player, const std::vector<std::shared_ptr<Entity>> &entities,
                          std::list<std::shared_ptr<sf::Shape>> &viewShape
#ifdef T5_DEBUG
                          , sf::RenderWindow &window
#endif
                          );
};
