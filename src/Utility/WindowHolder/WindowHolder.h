#ifndef VISIONS_WINDOWHOLDER_H
#define VISIONS_WINDOWHOLDER_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <utility>

#include "Point.h"

class WindowHolder {
private:
    std::unique_ptr<sf::RenderWindow> window;
    sf::View view;

    static constexpr char FPSKEY[] = "fps";
    static constexpr char AALVLKEY[] = "aalvl";
    static constexpr char WINDOWWIDTHKEY[] = "width";
    static constexpr char WINDOWHEIGHTKEY[] = "height";
    static constexpr char WINDOWLABEL[] = "app window";

    void setWindow();
public:
    WindowHolder();

    // draw everything
    template<typename ...Args>
    void draw(Args... args);

    // simple operations
    [[nodiscard]] bool isOpen() const;
    void clear();
    void display();
    void close();
    sf::Vector2u getSize() const;
    template<typename View>
    void setView(View &&forwardedView);

    // set view by mouse position
    template<typename Metrics_t>
    void updateViewWithMouse(const Point &point, Metrics_t levelWidth, Metrics_t levelHeight);

    // work with attached events
    bool pollEvent(sf::Event &event);

    // get mouse position on level's map
    Point getMousePositionOnLevel() const;
};

template<typename... Args>
void WindowHolder::draw(Args... args) {
    window->draw(args...);
}

template<typename Metrics_t>
void WindowHolder::updateViewWithMouse(const Point &point, Metrics_t levelWidth, Metrics_t levelHeight) {
    auto viewSize = view.getSize();
    auto dp = Point(viewSize.x / 2., viewSize.y / 2.) - sf::Mouse::getPosition(*window);

    float sx = (levelWidth - (levelWidth - 2 * point.x) * (1 - (viewSize.x - 40) / levelWidth)) / 2 - dp.x;
    float sy = (levelHeight - (levelHeight - 2 * point.y) * (1 - (viewSize.y - 40) / levelHeight)) / 2 - dp.y;

    view.setCenter(sx, sy);
    window->setView(view);
}

template<typename View>
void WindowHolder::setView(View &&forwardedView) {
    window->setView(std::forward<View>(forwardedView));
}

#endif //VISIONS_WINDOWHOLDER_H
