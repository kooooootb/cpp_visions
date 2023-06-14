#ifndef VISIONS_SFEVENTSERIALIZER_H
#define VISIONS_SFEVENTSERIALIZER_H

#include <SFML/Graphics.hpp>

/**
 * This class converts sf::Event to std::string only by type of event and some more included enums like keyboard button.
 * Used to store events (key bindings) in memory
 */
class sfEventSerializer {
public:
    sfEventSerializer() = delete;

    // there is a better approach
    static sf::Event::EventType codeToEventType(int code);
    static int eventTypeToCode(sf::Event::EventType eventType);

    static sf::Keyboard::Key codeToKeyboardKey(int code);
    static int keyboardKeyToCode(sf::Keyboard::Key key);
    static sf::Keyboard::Key stringToKeyboardKey(const std::string &stringCode);
    static std::string keyboardKeyToString(sf::Keyboard::Key key);

    static sf::Mouse::Button codeToMouseButton(int code);
    static int mouseButtonToCode(sf::Mouse::Button button);

    static constexpr char SEPARATOR = ' ';

    static std::string serialize(const sf::Event &event);
    static sf::Event deserialize(const std::string &query);
};


#endif //VISIONS_SFEVENTSERIALIZER_H
