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
    static constexpr unsigned int labelFontSize = 12;

    void setShape(const sf::Color &color);
    void setLabel(std::string label);
public:
    Button(T &outer_, ftype function_, const sf::Vector2f &position_, const sf::Vector2f &dimensions_, sf::Color color, std::string label);
    Button(T &outer_, ftype function_, float x, float y, float width, float height, sf::Color color, std::string label);

    void draw(sf::RenderWindow &window);
    void run();
    bool checkAndRun(const sf::Vector2f &clickPos);
    bool check(const sf::Vector2f &clickPos);
};

template<class T>
Button<T>::Button(T &outer_, ftype function_, const sf::Vector2f &position_,
                  const sf::Vector2f &dimensions_, sf::Color color, std::string label_) :
    outer(outer_) , function(function_) , x(position_.x) , y(position_.y) , width(dimensions_.x) ,
    height(dimensions_.y)
{
    setShape(color);
    setLabel(label_);
}

template<class T>
Button<T>::Button(T &outer_, ftype function_, float x_, float y_, float width_, float height_, sf::Color color, std::string label_) :
    outer(outer_) , function(function_) , x(x_) , y(y_) , width(width_) , height(height_)
{
    setShape(color);
    setLabel(label_);
}

template<class T>
void Button<T>::setShape(const sf::Color &color) {
    texture = createPlainTexture(1, 1, color);

    vertices = sf::VertexArray(sf::TriangleFan, 4);
    vertices[0].position = {x, y};
    vertices[1].position = {x + width, y};
    vertices[2].position = {x + width, y + height};
    vertices[3].position = {x, y + height};
}

template<class T>
void Button<T>::draw(sf::RenderWindow &window) {
    window.draw(vertices, &texture);
    window.draw(labelText);
}

template<class T>
void Button<T>::run() {
    function(outer);
}

template<class T>
bool Button<T>::checkAndRun(const sf::Vector2f &pos) {
    bool clicked = check(pos);
    if(clicked){
        run();
    }
    return clicked;
}

template<class T>
bool Button<T>::check(const sf::Vector2f &pos) {
    return pos.x > x && pos.y > y && pos.x < x + width && pos.y < y + height;
}

template<class T>
void Button<T>::setLabel(std::string label) {
    if(!labelFont.loadFromFile("arial.ttf")){
        throw std::runtime_error("Can't load font for buttons");
    }

    labelText = sf::Text(label, labelFont, 16);
    labelText.setOutlineThickness(1);
    labelText.setOutlineColor(sf::Color::Black);
    labelText.setOrigin(labelText.getLocalBounds().width / 2, labelText.getLocalBounds().height / 2);
    labelText.setPosition(x + width / 2, y + height / 2);
}

#endif //T5_BUTTON_H
