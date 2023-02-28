#ifndef T5_BUTTON_H
#define T5_BUTTON_H

template<class T>
class Button{
private:
    using ftype = std::function<void(T &outer)>;

    T &outer;
    ftype function;
    float x, y; // of top-left
    float width, height;

    sf::VertexArray vertices;
    sf::Texture texture;

    sf::Text labelText;
    sf::Font labelFont;
    static constexpr unsigned int labelFontSize = 16;

    void setShape(const sf::Color &color);
    void setLabel(std::string label);
public:
    Button(T &outer_, ftype function_, const sf::Vector2f &position_, const sf::Vector2f &dimensions_, sf::Color color, std::string label);
    Button(T &outer_, ftype function_, float x, float y, float width, float height, sf::Color color, std::string label);

    void draw(sf::RenderWindow &window) const;
    void run() const;
    bool checkAndRun(const sf::Vector2f &clickPos) const;
    bool check(const sf::Vector2f &clickPos) const;

    void setColor(sf::Color color);
};

template<class T>
Button<T>::Button(T &outer_, ftype function_, const sf::Vector2f &position_,
                  const sf::Vector2f &dimensions_, sf::Color color, std::string label) :
Button(outer_, function_, position_.x, position_.y, dimensions_.x, dimensions_.y, color, label) {}

template<class T>
Button<T>::Button(T &outer_, ftype function_, float x_, float y_, float width_, float height_, sf::Color color, std::string label) :
    outer(outer_) , function(function_) , x(x_) , y(y_) , width(width_) , height(height_)
{
    setShape(color);
    setLabel(label);
}

template<class T>
void Button<T>::setShape(const sf::Color &color) {
    texture = createPlainTexture(1, 1, color);
    sf::Vector2f center(x + width / 2, y + height / 2);
    sf::Vector2f help = {width / 2, height / 2};
    setVerticesAsBox(vertices, center, help);
}

template<class T>
void Button<T>::draw(sf::RenderWindow &window) const {
    window.draw(vertices, &texture);
    window.draw(labelText);
}

template<class T>
void Button<T>::run() const {
    function(outer);
}

template<class T>
bool Button<T>::checkAndRun(const sf::Vector2f &pos) const {
    bool clicked = check(pos);
    if(clicked){
        run();
    }
    return clicked;
}

template<class T>
bool Button<T>::check(const sf::Vector2f &pos) const {
    return pos.x > x && pos.y > y && pos.x < x + width && pos.y < y + height;
}

template<class T>
void Button<T>::setLabel(std::string label) {
    if(!labelFont.loadFromFile("arial.ttf")){
        throw std::runtime_error("Can't load font for buttons");
    }

    labelText = sf::Text(label, labelFont, labelFontSize);
    labelText.setOutlineThickness(1);
    labelText.setOutlineColor(sf::Color::Black);
    labelText.setOrigin(labelText.getLocalBounds().width / 2, labelText.getLocalBounds().height / 2);
    labelText.setPosition(x + width / 2, y + height / 2);
}

template<class T>
void Button<T>::setColor(sf::Color color){
    texture = createPlainTexture(1, 1, color);
}

#endif //T5_BUTTON_H
