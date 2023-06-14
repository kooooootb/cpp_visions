#include "sfEventSerializer.h"

std::string sfEventSerializer::serialize(const sf::Event &event) {
    std::string result = std::to_string(eventTypeToCode(event.type));

    if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased){
        result += SEPARATOR;
        result += std::to_string(keyboardKeyToCode(event.key.code));
    } else if(event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased){
        result += SEPARATOR;
        result += std::to_string(mouseButtonToCode(event.mouseButton.button));
    }

    return result;
}

sf::Event sfEventSerializer::deserialize(const std::string &query) {
    sf::Event event{};
    auto space = query.find(SEPARATOR);

    if(space != std::string::npos){
        // get event type from substr to space, which isn't npos
        event.type = codeToEventType(std::stoi(query.substr(0, space)));

        // get first character of second word
        ++space;

        // get secondary type
        if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased){
            event.key.code = codeToKeyboardKey(std::stoi(query.substr(space, query.size())));
        } else if(event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased){
            event.mouseButton.button = codeToMouseButton(std::stoi(query.substr(space, query.size())));
        }
    } else{
        // get event type from whole string
        event.type = codeToEventType(std::stoi(query));
    }

    return event;
}

sf::Event::EventType sfEventSerializer::codeToEventType(int code) {
    switch(code){
        case 1: return sf::Event::Closed;
        case 2: return sf::Event::Resized;
        case 3: return sf::Event::LostFocus;
        case 4: return sf::Event::GainedFocus;
        case 5: return sf::Event::TextEntered;
        case 6: return sf::Event::KeyPressed;
        case 7: return sf::Event::KeyReleased;
        case 8: return sf::Event::MouseWheelMoved;
        case 9: return sf::Event::MouseWheelScrolled;
        case 10: return sf::Event::MouseButtonPressed;
        case 11: return sf::Event::MouseButtonReleased;
        case 12: return sf::Event::MouseMoved;
        case 13: return sf::Event::MouseEntered;
        case 14: return sf::Event::MouseLeft;
        case 15: return sf::Event::JoystickButtonPressed;
        case 16: return sf::Event::JoystickButtonReleased;
        case 17: return sf::Event::JoystickMoved;
        case 18: return sf::Event::JoystickConnected;
        case 19: return sf::Event::JoystickDisconnected;
        case 20: return sf::Event::TouchBegan;
        case 21: return sf::Event::TouchMoved;
        case 22: return sf::Event::TouchEnded;
        case 23: return sf::Event::SensorChanged;
        default: return sf::Event::Count;
    }
}

int sfEventSerializer::eventTypeToCode(sf::Event::EventType eventType) {
    switch(eventType){
        case sf::Event::Closed: return 1;
        case sf::Event::Resized: return 2;
        case sf::Event::LostFocus: return 3;
        case sf::Event::GainedFocus: return 4;
        case sf::Event::TextEntered: return 5;
        case sf::Event::KeyPressed: return 6;
        case sf::Event::KeyReleased: return 7;
        case sf::Event::MouseWheelMoved: return 8;
        case sf::Event::MouseWheelScrolled: return 9;
        case sf::Event::MouseButtonPressed: return 10;
        case sf::Event::MouseButtonReleased: return 11;
        case sf::Event::MouseMoved: return 12;
        case sf::Event::MouseEntered: return 13;
        case sf::Event::MouseLeft: return 14;
        case sf::Event::JoystickButtonPressed: return 15;
        case sf::Event::JoystickButtonReleased: return 16;
        case sf::Event::JoystickMoved: return 17;
        case sf::Event::JoystickConnected: return 18;
        case sf::Event::JoystickDisconnected: return 19;
        case sf::Event::TouchBegan: return 20;
        case sf::Event::TouchMoved: return 21;
        case sf::Event::TouchEnded: return 22;
        case sf::Event::SensorChanged: return 23;
        default: return 24;
    }
}

sf::Keyboard::Key sfEventSerializer::codeToKeyboardKey(int code) {
    switch(code){
        case 0: return sf::Keyboard::Key::A;
        case 1: return sf::Keyboard::Key::B;
        case 2: return sf::Keyboard::Key::C;
        case 3: return sf::Keyboard::Key::D;
        case 4: return sf::Keyboard::Key::E;
        case 5: return sf::Keyboard::Key::F;
        case 6: return sf::Keyboard::Key::G;
        case 7: return sf::Keyboard::Key::H;
        case 8: return sf::Keyboard::Key::I;
        case 9: return sf::Keyboard::Key::J;
        case 10: return sf::Keyboard::Key::K;
        case 11: return sf::Keyboard::Key::L;
        case 12: return sf::Keyboard::Key::M;
        case 13: return sf::Keyboard::Key::N;
        case 14: return sf::Keyboard::Key::O;
        case 15: return sf::Keyboard::Key::P;
        case 16: return sf::Keyboard::Key::Q;
        case 17: return sf::Keyboard::Key::R;
        case 18: return sf::Keyboard::Key::S;
        case 19: return sf::Keyboard::Key::T;
        case 20: return sf::Keyboard::Key::U;
        case 21: return sf::Keyboard::Key::V;
        case 22: return sf::Keyboard::Key::W;
        case 23: return sf::Keyboard::Key::X;
        case 24: return sf::Keyboard::Key::Y;
        case 25: return sf::Keyboard::Key::Z;
        case 26: return sf::Keyboard::Key::Num0;
        case 27: return sf::Keyboard::Key::Num1;
        case 28: return sf::Keyboard::Key::Num2;
        case 29: return sf::Keyboard::Key::Num3;
        case 30: return sf::Keyboard::Key::Num4;
        case 31: return sf::Keyboard::Key::Num5;
        case 32: return sf::Keyboard::Key::Num6;
        case 33: return sf::Keyboard::Key::Num7;
        case 34: return sf::Keyboard::Key::Num8;
        case 35: return sf::Keyboard::Key::Num9;
        case 36: return sf::Keyboard::Key::Escape;
        case 37: return sf::Keyboard::Key::LControl;
        case 38: return sf::Keyboard::Key::LShift;
        case 39: return sf::Keyboard::Key::LAlt;
        case 40: return sf::Keyboard::Key::LSystem;
        case 41: return sf::Keyboard::Key::RControl;
        case 42: return sf::Keyboard::Key::RShift;
        case 43: return sf::Keyboard::Key::RAlt;
        case 44: return sf::Keyboard::Key::RSystem;
        case 45: return sf::Keyboard::Key::Menu;
        case 46: return sf::Keyboard::Key::LBracket;
        case 47: return sf::Keyboard::Key::RBracket;
        case 48: return sf::Keyboard::Key::Semicolon;
        case 49: return sf::Keyboard::Key::Comma;
        case 50: return sf::Keyboard::Key::Period;
        case 51: return sf::Keyboard::Key::Apostrophe;
        case 52: return sf::Keyboard::Key::Slash;
        case 53: return sf::Keyboard::Key::Backslash;
        case 54: return sf::Keyboard::Key::Grave;
        case 55: return sf::Keyboard::Key::Equal;
        case 56: return sf::Keyboard::Key::Hyphen;
        case 57: return sf::Keyboard::Key::Space;
        case 58: return sf::Keyboard::Key::Enter;
        case 59: return sf::Keyboard::Key::Backspace;
        case 60: return sf::Keyboard::Key::Tab;
        case 61: return sf::Keyboard::Key::PageUp;
        case 62: return sf::Keyboard::Key::PageDown;
        case 63: return sf::Keyboard::Key::End;
        case 64: return sf::Keyboard::Key::Home;
        case 65: return sf::Keyboard::Key::Insert;
        case 66: return sf::Keyboard::Key::Delete;
        case 67: return sf::Keyboard::Key::Add;
        case 68: return sf::Keyboard::Key::Subtract;
        case 69: return sf::Keyboard::Key::Multiply;
        case 70: return sf::Keyboard::Key::Divide;
        case 71: return sf::Keyboard::Key::Left;
        case 72: return sf::Keyboard::Key::Right;
        case 73: return sf::Keyboard::Key::Up;
        case 74: return sf::Keyboard::Key::Down;
        case 75: return sf::Keyboard::Key::Numpad0;
        case 76: return sf::Keyboard::Key::Numpad1;
        case 77: return sf::Keyboard::Key::Numpad2;
        case 78: return sf::Keyboard::Key::Numpad3;
        case 79: return sf::Keyboard::Key::Numpad4;
        case 80: return sf::Keyboard::Key::Numpad5;
        case 81: return sf::Keyboard::Key::Numpad6;
        case 82: return sf::Keyboard::Key::Numpad7;
        case 83: return sf::Keyboard::Key::Numpad8;
        case 84: return sf::Keyboard::Key::Numpad9;
        case 85: return sf::Keyboard::Key::F1;
        case 86: return sf::Keyboard::Key::F2;
        case 87: return sf::Keyboard::Key::F3;
        case 88: return sf::Keyboard::Key::F4;
        case 89: return sf::Keyboard::Key::F5;
        case 90: return sf::Keyboard::Key::F6;
        case 91: return sf::Keyboard::Key::F7;
        case 92: return sf::Keyboard::Key::F8;
        case 93: return sf::Keyboard::Key::F9;
        case 94: return sf::Keyboard::Key::F10;
        case 95: return sf::Keyboard::Key::F11;
        case 96: return sf::Keyboard::Key::F12;
        case 97: return sf::Keyboard::Key::F13;
        case 98: return sf::Keyboard::Key::F14;
        case 99: return sf::Keyboard::Key::F15;
        case 100: return sf::Keyboard::Key::Pause;
        default: return sf::Keyboard::Key::Unknown;
    }
}

int sfEventSerializer::keyboardKeyToCode(sf::Keyboard::Key key) {
    switch(key){
        case sf::Keyboard::Key::A: return 0;
        case sf::Keyboard::Key::B: return 1;
        case sf::Keyboard::Key::C: return 2;
        case sf::Keyboard::Key::D: return 3;
        case sf::Keyboard::Key::E: return 4;
        case sf::Keyboard::Key::F: return 5;
        case sf::Keyboard::Key::G: return 6;
        case sf::Keyboard::Key::H: return 7;
        case sf::Keyboard::Key::I: return 8;
        case sf::Keyboard::Key::J: return 9;
        case sf::Keyboard::Key::K: return 10;
        case sf::Keyboard::Key::L: return 11;
        case sf::Keyboard::Key::M: return 12;
        case sf::Keyboard::Key::N: return 13;
        case sf::Keyboard::Key::O: return 14;
        case sf::Keyboard::Key::P: return 15;
        case sf::Keyboard::Key::Q: return 16;
        case sf::Keyboard::Key::R: return 17;
        case sf::Keyboard::Key::S: return 18;
        case sf::Keyboard::Key::T: return 19;
        case sf::Keyboard::Key::U: return 20;
        case sf::Keyboard::Key::V: return 21;
        case sf::Keyboard::Key::W: return 22;
        case sf::Keyboard::Key::X: return 23;
        case sf::Keyboard::Key::Y: return 24;
        case sf::Keyboard::Key::Z: return 25;
        case sf::Keyboard::Key::Num0: return 26;
        case sf::Keyboard::Key::Num1: return 27;
        case sf::Keyboard::Key::Num2: return 28;
        case sf::Keyboard::Key::Num3: return 29;
        case sf::Keyboard::Key::Num4: return 30;
        case sf::Keyboard::Key::Num5: return 31;
        case sf::Keyboard::Key::Num6: return 32;
        case sf::Keyboard::Key::Num7: return 33;
        case sf::Keyboard::Key::Num8: return 34;
        case sf::Keyboard::Key::Num9: return 35;
        case sf::Keyboard::Key::Escape: return 36;
        case sf::Keyboard::Key::LControl: return 37;
        case sf::Keyboard::Key::LShift: return 38;
        case sf::Keyboard::Key::LAlt: return 39;
        case sf::Keyboard::Key::LSystem: return 40;
        case sf::Keyboard::Key::RControl: return 41;
        case sf::Keyboard::Key::RShift: return 42;
        case sf::Keyboard::Key::RAlt: return 43;
        case sf::Keyboard::Key::RSystem: return 44;
        case sf::Keyboard::Key::Menu: return 45;
        case sf::Keyboard::Key::LBracket: return 46;
        case sf::Keyboard::Key::RBracket: return 47;
        case sf::Keyboard::Key::Semicolon: return 48;
        case sf::Keyboard::Key::Comma: return 49;
        case sf::Keyboard::Key::Period: return 50;
        case sf::Keyboard::Key::Apostrophe: return 51;
        case sf::Keyboard::Key::Slash: return 52;
        case sf::Keyboard::Key::Backslash: return 53;
        case sf::Keyboard::Key::Grave: return 54;
        case sf::Keyboard::Key::Equal: return 55;
        case sf::Keyboard::Key::Hyphen: return 56;
        case sf::Keyboard::Key::Space: return 57;
        case sf::Keyboard::Key::Enter: return 58;
        case sf::Keyboard::Key::Backspace: return 59;
        case sf::Keyboard::Key::Tab: return 60;
        case sf::Keyboard::Key::PageUp: return 61;
        case sf::Keyboard::Key::PageDown: return 62;
        case sf::Keyboard::Key::End: return 63;
        case sf::Keyboard::Key::Home: return 64;
        case sf::Keyboard::Key::Insert: return 65;
        case sf::Keyboard::Key::Delete: return 66;
        case sf::Keyboard::Key::Add: return 67;
        case sf::Keyboard::Key::Subtract: return 68;
        case sf::Keyboard::Key::Multiply: return 69;
        case sf::Keyboard::Key::Divide: return 70;
        case sf::Keyboard::Key::Left: return 71;
        case sf::Keyboard::Key::Right: return 72;
        case sf::Keyboard::Key::Up: return 73;
        case sf::Keyboard::Key::Down: return 74;
        case sf::Keyboard::Key::Numpad0: return 75;
        case sf::Keyboard::Key::Numpad1: return 76;
        case sf::Keyboard::Key::Numpad2: return 77;
        case sf::Keyboard::Key::Numpad3: return 78;
        case sf::Keyboard::Key::Numpad4: return 79;
        case sf::Keyboard::Key::Numpad5: return 80;
        case sf::Keyboard::Key::Numpad6: return 81;
        case sf::Keyboard::Key::Numpad7: return 82;
        case sf::Keyboard::Key::Numpad8: return 83;
        case sf::Keyboard::Key::Numpad9: return 84;
        case sf::Keyboard::Key::F1: return 85;
        case sf::Keyboard::Key::F2: return 86;
        case sf::Keyboard::Key::F3: return 87;
        case sf::Keyboard::Key::F4: return 88;
        case sf::Keyboard::Key::F5: return 89;
        case sf::Keyboard::Key::F6: return 90;
        case sf::Keyboard::Key::F7: return 91;
        case sf::Keyboard::Key::F8: return 92;
        case sf::Keyboard::Key::F9: return 93;
        case sf::Keyboard::Key::F10: return 94;
        case sf::Keyboard::Key::F11: return 95;
        case sf::Keyboard::Key::F12: return 96;
        case sf::Keyboard::Key::F13: return 97;
        case sf::Keyboard::Key::F14: return 98;
        case sf::Keyboard::Key::F15: return 99;
        case sf::Keyboard::Key::Pause: return 100;
        default: return -1;
    }
}

sf::Mouse::Button sfEventSerializer::codeToMouseButton(int code) {
    switch(code){
        case 0: return sf::Mouse::Button::Left;
        case 1: return sf::Mouse::Button::Right;
        case 2: return sf::Mouse::Button::Middle;
        case 3: return sf::Mouse::Button::XButton1;
        case 4: return sf::Mouse::Button::XButton2;
        default: return sf::Mouse::Button::ButtonCount;
    }
}

int sfEventSerializer::mouseButtonToCode(sf::Mouse::Button button) {
    switch(button){
        case sf::Mouse::Button::Left: return 0;
        case sf::Mouse::Button::Right: return 1;
        case sf::Mouse::Button::Middle: return 2;
        case sf::Mouse::Button::XButton1: return 3;
        case sf::Mouse::Button::XButton2: return 4;
        default: return -1;
    }
}

sf::Keyboard::Key sfEventSerializer::stringToKeyboardKey(const std::string &stringCode) {
    int code = std::stoi(stringCode);
    return codeToKeyboardKey(code);
}

std::string sfEventSerializer::keyboardKeyToString(sf::Keyboard::Key key) {
    return std::to_string(keyboardKeyToCode(key));
}
