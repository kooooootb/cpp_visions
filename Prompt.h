#ifndef T5_PROMPT_H
#define T5_PROMPT_H

template<class T>
class Prompt{
private:
    T &outer;
    using voidf = std::function<void(T &)>;
    using boolf = std::function<bool(const sf::String&)>; // checks if input sf string can be accepted
    using applyf = std::function<void(T &, sf::String)>; // use string after pressing ok button

    boolf checkInput;
    std::vector<std::shared_ptr<Button<T>>> buttons;

    applyf applyFunction;

    sf::VertexArray vertices, inputVertices, inputBgVertices;
    sf::Texture texture, inputTexture, inputBgTexture;

    // ratio to nearest object (input box to whole box, text to input box)
    constexpr static const float INPUTBOXHEIGHTRATIO = 0.2;
    constexpr static const float INPUTBOXWIDTHRATIO = 0.9;
    constexpr static const float INPUTBOXTHICKNESS = 0.0005;
    constexpr static const float BUTTONXOFFSETRATIO = 0.25;
    constexpr static const float BUTTONYOFFSETRATIO = 0.25;
    constexpr static const float BUTTONWIDTHRATIO = 0.2;
    constexpr static const float BUTTONHEIGHTRATIO = 0.2;
    constexpr static const float TEXTYOFFSETRATIO = 0.25;
    constexpr static const float TEXTHEIGHTRATIO = 0.8;
    constexpr static const float INPUTTEXTXOFFSETRATIO = 0.9;

    sf::Text labelText;
    sf::Font labelFont;
    static constexpr unsigned int labelFontSize = 16;

    sf::String currentInput;
    sf::Text inputText;
    sf::Font inputFont;

    void initShapes(const sf::Color &color, float x, float y, float width, float height);
    void setLabelText(std::string label, sf::Vector2f);
    void setInputText(float inputTextHeight, sf::Vector2f position);

public:
    Prompt(T &outer_, applyf applyFunction_, voidf ok, voidf cancel, boolf checkInput_, float x_, float y_, float width_, float height_,
           sf::Color color, std::string label, sf::String defaultInput = "");
    Prompt(T &outer_, applyf applyFunction_, voidf ok, boolf checkInput_, float x_, float y_, float width_, float height_,
           sf::Color color, std::string label, sf::String defaultInput = "");

    void draw(sf::RenderWindow &window) const;
    bool handleClick(const sf::Vector2f &clickPos) const;
    bool approve() const;
    void addInput(sf::Uint32 unicode);

    void setOkButtonColor(sf::Color color);
    void setCancelButtonColor(sf::Color color);
    void setInputColor(sf::Color color);
    void setInputBgColor(sf::Color color);
    void setColor(sf::Color color);
};

// with cancel button
template<class T>
Prompt<T>::Prompt(T &outer_, applyf applyFunction_, voidf ok, voidf cancel, boolf checkInput_, float x, float y,
                  float width, float height, sf::Color color, std::string label, sf::String defaultInput) :
Prompt(outer_, applyFunction_, ok, checkInput_, x, y, width, height, color, label, defaultInput)
{
    // cancel button
    sf::Vector2f topLeft = {x + width * (1 - 2 * BUTTONXOFFSETRATIO - BUTTONWIDTHRATIO) / 2, y + height * (1 + 2 * BUTTONYOFFSETRATIO - BUTTONHEIGHTRATIO) / 2};
    sf::Vector2f dimensions = {width * BUTTONWIDTHRATIO, height * BUTTONHEIGHTRATIO};
    buttons.push_back(std::make_shared<Button<T>>(outer, cancel, topLeft, dimensions, sf::Color::Red, "Cancel"));
}

// with no cancel
template<class T>
Prompt<T>::Prompt(T &outer_, applyf applyFunction_, voidf ok, boolf checkInput_, float x, float y,
                  float width, float height, sf::Color color, std::string label, sf::String defaultInput) :
    outer(outer_) , checkInput(checkInput_) , currentInput(defaultInput) , applyFunction(applyFunction_)
{
    initShapes(color, x, y, width, height);

    // ok button
    sf::Vector2f topLeft = {x + width * (1 + 2 * BUTTONXOFFSETRATIO - BUTTONWIDTHRATIO) / 2, y + height * (1 + 2 * BUTTONYOFFSETRATIO - BUTTONHEIGHTRATIO) / 2};
    sf::Vector2f dimensions = {width * BUTTONWIDTHRATIO, height * BUTTONHEIGHTRATIO};
    buttons.push_back(std::make_shared<Button<T>>(outer, ok, topLeft, dimensions, sf::Color::Green, "Ok"));

    setLabelText(label, {0, -height * TEXTYOFFSETRATIO});
    setInputText(height * INPUTBOXHEIGHTRATIO * TEXTHEIGHTRATIO, {x + width / 2 - INPUTTEXTXOFFSETRATIO * INPUTBOXWIDTHRATIO / 2 * width, y + height / 2 - INPUTBOXHEIGHTRATIO * height / 2});
}

template<class T>
void Prompt<T>::initShapes(const sf::Color &color, float x, float y, float width, float height) {
    sf::Vector2f center(x + width / 2, y + height / 2);
    sf::Vector2f help;

    // main box's shape
    texture = createPlainTexture(1, 1, color);
    help = {width / 2, height / 2};
    setVerticesAsBox(vertices, center, help);

    // input's shape
    inputTexture = createPlainTexture(1, 1, sf::Color::Black);
    help = {width * INPUTBOXWIDTHRATIO / 2, height * INPUTBOXHEIGHTRATIO / 2};
    setVerticesAsBox(inputVertices, center, help);

    // input bg's shape
    inputBgTexture = createPlainTexture(1, 1, sf::Color::White);
    help = {width * (INPUTBOXWIDTHRATIO / 2 + INPUTBOXTHICKNESS), height * (INPUTBOXHEIGHTRATIO / 2 + INPUTBOXTHICKNESS)};
    setVerticesAsBox(inputBgVertices, center, help);
}

template<class T>
void Prompt<T>::setLabelText(std::string label, sf::Vector2f position) {
    if(!labelFont.loadFromFile("arial.ttf")){
        throw std::runtime_error("Can't load fonts for prompt");
    }

    labelText = sf::Text(label, labelFont, labelFontSize);
    labelText.setOutlineThickness(1);
    labelText.setOutlineColor(sf::Color::Black);
    labelText.setOrigin(labelText.getLocalBounds().width / 2, labelText.getLocalBounds().height / 2);
    labelText.setPosition(position);
}

template<class T>
void Prompt<T>::setInputText(float inputTextHeight, sf::Vector2f position) {
    if(!inputFont.loadFromFile("arial.ttf")){
        throw std::runtime_error("Can't load fonts for prompt");
    }

    inputText = sf::Text("", inputFont, inputTextHeight);
    inputText.setString(currentInput);
    inputText.setOutlineThickness(0.2);
    inputText.setOutlineColor(sf::Color::Black);
    inputText.setOrigin(0, 0);
    inputText.setPosition(position);
}

template<class T>
void Prompt<T>::setOkButtonColor(sf::Color color) {
    // there's always at least one button
    buttons[0]->setColor(color);
}

template<class T>
void Prompt<T>::setCancelButtonColor(sf::Color color) {
    if(buttons.size() > 0) {
        buttons[1]->setColor(color);
    }
}

template<class T>
void Prompt<T>::setInputColor(sf::Color color) {
    inputTexture = createPlainTexture(1, 1, color);
}

template<class T>
void Prompt<T>::setInputBgColor(sf::Color color) {
    inputBgTexture = createPlainTexture(1, 1, color);
}

template<class T>
void Prompt<T>::setColor(sf::Color color) {
    texture = createPlainTexture(1, 1, color);
}

template<class T>
bool Prompt<T>::handleClick(const sf::Vector2f &clickPos) const {
    if(buttons[0]->check(clickPos)){
        return approve();
    } else if(buttons.size() > 1 && buttons[1]->check(clickPos)){
        buttons[1]->run();
        return true;
    }
    return false;
}

template<class T>
void Prompt<T>::addInput(sf::Uint32 unicode) {
    if(unicode == 8){
        if(currentInput.getSize() > 0) {
            currentInput.erase(currentInput.getSize() - 1);
        }
    } else{
        currentInput += unicode;
    }

    // reconstruct positions and others
    inputText.setString(currentInput);
}

template<class T>
void Prompt<T>::draw(sf::RenderWindow &window) const {
    // main box
    window.draw(vertices, &texture);

    window.draw(labelText); // draw label
    window.draw(inputBgVertices, &inputBgTexture); // draw input background
    window.draw(inputVertices, &inputTexture); // draw input box
    window.draw(inputText); // draw input text

    // draw buttons
    for(const auto &button : buttons){
        button->draw(window);
    }
}

template<class T>
bool Prompt<T>::approve() const {
    if(checkInput(currentInput)){
        buttons[0]->run();
        applyFunction(outer, currentInput);
        return true;
    }
    return false;
}

#endif //T5_PROMPT_H
