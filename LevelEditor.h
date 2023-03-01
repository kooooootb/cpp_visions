#ifndef T5_LEVELEDITOR_H
#define T5_LEVELEDITOR_H

#include "Common.h"
#include "Button.h"
#include "Prompt.h"

#include <queue>

#define LEVELEDITOR_DEBUGVIEW

class LevelEditor {
private:
    enum InsertionMode{
        POLYGON = 1,
        WEAPON,
        MODESLENGTH
    };

    using ButtonArray = std::vector<std::shared_ptr<Button<LevelEditor>>>;
    friend class Button<LevelEditor>;
    friend class Prompt<LevelEditor>;

    // window cfgs
    std::shared_ptr<sf::RenderWindow> pWindow;
    sf::Vector2f viewSpeed;
    sf::Vector2f viewAcceleration;
    static constexpr float viewStep = 2;
    static constexpr float viewFriction = 1;
    sf::View levelView, hudView;
    bool exiting = false;
    bool needSave = false;

    // editor variables
    bool isPrecise;
    sf::Vector2f aimPoint;
    sf::CircleShape aimPointShape;
    float precision;
    InsertionMode insertionMode;
    sf::Event event;

    // entities to edit
    sf::VertexArray currentLine;
    PolygonArray polygons;
    std::shared_ptr<Polygon> currentPolygon;
    std::vector<Point> weapons;
    BoarderType boarders;
    static constexpr BoarderType::first_type BOARDER_MIN = 100;
    static constexpr BoarderType::first_type BOARDER_MAX = 5000;

    // buttons
    ButtonArray buttons;

    // prompts
    std::queue<Prompt<LevelEditor>> promptQueue;

    // background
    sf::VertexArray bgVertices;
    sf::Texture bgTexture;

    // unprecise grid
    sf::VertexArray unpreciseGridVertices;
    sf::Texture unpreciseGridTexture;

    // file name for loading saving
    std::string saveFileName;

#ifdef LEVELEDITOR_DEBUGVIEW
    sf::CircleShape vertexPoint;
    sf::VertexArray triangle;
    sf::Texture plainRedTexture;
#endif

    // helper functions (fabric methods, )
    void insertToLevel();
    void showUnpreciseGrid();
    void hideUnpreciseGrid();
    void setAimPoint();
    void switchInsertionMode();
    void buildPolygon();
    void clearCurrentPolygon();
    void moveView(sf::Vector2f &);
    void setButtons();
    bool handleButtons();
    void exitEditor();
    void handleNormalEvent();
    void handlePromptEvent();
    void handlePromptClick();
    void promptBoarders();
    bool areBoardersSet() const;
    void setBoarders(BoarderType);

    // drawings
    void drawBackground();
    void drawUnpreciseGrid();
    void drawPolygons();
    void drawCurrentPolygon();
    void drawAimPoint();
    void drawButtons();
    void drawPrompt();

    // window functions (draw, update events)
    void handleEvents();
    void updateView();
    void updateWindow();
public:
    LevelEditor(std::shared_ptr<sf::RenderWindow> &pWindow, const std::string &levelFileName);

    void run();

};


#endif //T5_LEVELEDITOR_H
