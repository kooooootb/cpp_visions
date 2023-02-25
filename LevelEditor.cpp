#include "LevelEditor.h"
#include "Polygon.h"

LevelEditor::LevelEditor(std::shared_ptr<sf::RenderWindow> &pWindow_, const std::string &levelFileName) :
    pWindow(pWindow_),
    currentLine(sf::LineStrip),
    bgVertices(sf::Quads, 4),
    event(),
    viewSpeed(0, 0),
    viewAcceleration(0, 0),
    levelView(sf::View(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(pWindow->getSize()))),
    hudView(sf::View(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(pWindow->getSize())))
{
    // editor variables
    isPrecise = true;
    aimPointShape = setAimPointShape();
    precision = 20;
    insertionMode = POLYGON;

    // try to load level file, if can't -> create new, ask for dimensions
    try{
        polygons = ldPl(levelFname);
        boarders = ldBd(levelFname);
        weapons = ldWp(levelFname);
    } catch(std::runtime_error &e){
        inputDimension(boarders.first, "Input width");
        inputDimension(boarders.second, "Input height");
        addBoarders(polygons, boarders);
    }

    // set view
    pWindow->setView(levelView);

    // set background
    createBackground(bgVertices, bgTexture, boarders.first, boarders.second);

    // set unprecise grid
    unpreciseGridVertices = createGridVertices(boarders.first, boarders.second, precision);
    unpreciseGridTexture = createPlainTexture(boarders.first, boarders.second, sf::Color(25, 25, 25, 255));

    // set buttons
    setButtons();

#ifdef LEVELEDITOR_DEBUGVIEW
    vertexPoint = sf::CircleShape(3);
    vertexPoint.setFillColor(sf::Color::Red);
    vertexPoint.setOrigin(1.5, 1.5);
    triangle = sf::VertexArray(sf::Lines, 3);
    plainRedTexture = createPlainTexture(1, 1, sf::Color::Red);
#endif
}

void LevelEditor::setButtons(){
    float width = (float) pWindow->getSize().x / 2;
    float height = (float) pWindow->getSize().y / 2;

    // clear polygon button
    buttons.push_back(std::make_shared<Button<LevelEditor>>(*this, [](LevelEditor &outer) -> void {
        outer.clearCurrentPolygon();
    }, width - 100, height - 100, 100, 100, sf::Color(41, 25, 111, 160), "Clear current\npolygon"));

    // exit button
    buttons.push_back(std::make_shared<Button<LevelEditor>>(*this, [](LevelEditor &outer) -> void {
        outer.exitEditor();
    }, width - 100, -height, 100, 100, sf::Color::Red, "Close editor"));
}

bool LevelEditor::handleButtons() {
    sf::Vector2f mousePos = getMousePosition(*pWindow, hudView);
    return std::any_of(std::begin(buttons), std::end(buttons), [&mousePos](auto &button) -> bool {
        return button->checkAndRun(mousePos);
    });
}

void LevelEditor::inputDimension(BoarderType::first_type &dimension, std::string message) {
    do {
        try {
            if(!pWindow->isOpen()){
                throw std::runtime_error("User closed window");
            }
            dimension = std::stoi(getStringFromWindow(*pWindow, message));
        } catch (std::runtime_error &ex) {
            dimension = 0;
        }
        message = "Dimension should be in range [" + std::to_string(BOARDER_MIN) + ", " + std::to_string(BOARDER_MAX) + "]";
    }while(BOARDER_MAX < dimension || dimension < BOARDER_MIN);
}

void LevelEditor::insertToLevel(){
    switch(insertionMode) {
        case POLYGON:
            currentLine.append(aimPoint);
            if(currentLine.getVertexCount() >= 3){
                currentPolygon = std::make_shared<Polygon>(vertexArrayToPoints(currentLine));
            }
            break;
        case WEAPON:
            weapons.emplace_back(aimPoint);
            break;
        default:
            throw std::runtime_error("Reached default case in mouse button pressing event");
    }
}

void LevelEditor::setAimPoint() {
    aimPoint = getMousePosition(*pWindow, levelView);
    if(!isPrecise){
        aimPoint.x = (float)((int)((aimPoint.x + precision / 2) / precision)) * precision;
        aimPoint.y = (float)((int)((aimPoint.y + precision / 2) / precision)) * precision;
    }
    aimPointShape.setPosition(aimPoint);
}

void LevelEditor::switchInsertionMode() {
    insertionMode = static_cast<InsertionMode>((static_cast<int>(insertionMode) + 1) % static_cast<int>(MODESLENGTH));
}

void LevelEditor::buildPolygon() {
    if(currentLine.getVertexCount() >= 3){
        polygons.push_back(currentPolygon);
    }
}

void LevelEditor::clearCurrentPolygon() {
    if(currentLine.getVertexCount() >= 3){
        currentPolygon = nullptr;
        currentLine.clear();
    }
}

void LevelEditor::showUnpreciseGrid() {
    isPrecise = false;
}

void LevelEditor::hideUnpreciseGrid() {
    isPrecise = true;
}

void LevelEditor::exitEditor() {
    exiting = true;
}

void LevelEditor::handleEvents(){
    while (pWindow->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                pWindow->close();
                exitEditor();
                return;
            case sf::Event::MouseButtonPressed: { // add some point
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if(!handleButtons()){
                        insertToLevel();
                    }
                } else {
                    showUnpreciseGrid();
                }
                break;
            }
            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    hideUnpreciseGrid();
                }
                break;
            }
            case sf::Event::MouseMoved: {
//                setAimPoint();
                break;
            }
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::G: // switch modes
                        switchInsertionMode();
                        break;
                    case sf::Keyboard::E: // build current polygon and
                        buildPolygon();
                        // no break
                    case sf::Keyboard::O: // clear current polygon
                        clearCurrentPolygon();
                        break;
                    case sf::Keyboard::Escape:
                        exitEditor();
                        return;
                    default:
                        //default keyboard button
                        break;
                }
                break;
            default:
                //default event
                break;
        }
    }
}

void LevelEditor::moveView(sf::Vector2f &d) {
    levelView.move(d);

    float x = levelView.getCenter().x, y = levelView.getCenter().y;
    if(x < 0){
        viewSpeed.x = 0;
        x = 0;
    } else if(x > (float) boarders.first){
        viewSpeed.x = 0;
        x = (float) boarders.first;
    }

    if(y < 0){
        viewSpeed.y = 0;
        y = 0;
    } else if(y > (float) boarders.second){
        viewSpeed.y = 0;
        y = (float) boarders.second;
    }

    levelView.setCenter(x, y);
}

void LevelEditor::updateView() {
    viewAcceleration = {0, 0};
    if(checkKeyPressed({sf::Keyboard::W, sf::Keyboard::Up})) viewAcceleration.y -= viewStep;
    if(checkKeyPressed({sf::Keyboard::A, sf::Keyboard::Left})) viewAcceleration.x -= viewStep;
    if(checkKeyPressed({sf::Keyboard::S, sf::Keyboard::Down})) viewAcceleration.y += viewStep;
    if(checkKeyPressed({sf::Keyboard::D, sf::Keyboard::Right})) viewAcceleration.x += viewStep;
    viewSpeed += viewAcceleration;
    applyFriction(viewSpeed, viewFriction);

    moveView(viewSpeed);
}

void LevelEditor::drawBackground() {
    pWindow->draw(bgVertices, &bgTexture);
}

void LevelEditor::drawUnpreciseGrid() {
    if(!isPrecise){
        pWindow->draw(unpreciseGridVertices, &unpreciseGridTexture);
    }
}

void LevelEditor::drawPolygons() {
    for(const auto &polygon : polygons){
        polygon->draw(*pWindow);
    }
}

void LevelEditor::drawCurrentPolygon() {
    if(currentPolygon){
        currentPolygon->draw(*pWindow);
    }
}

void LevelEditor::drawAimPoint(){
    pWindow->draw(aimPointShape);
}

void LevelEditor::drawButtons(){
    for(auto &button : buttons){
        button->draw(*pWindow);
    }
}

void LevelEditor::updateWindow() {
    // clear previous frame
    pWindow->clear();

    // draw level section:
    pWindow->setView(levelView);
    drawBackground();
    drawUnpreciseGrid();
    drawPolygons();
    drawCurrentPolygon();
    drawAimPoint();

    // draw hud section:
    pWindow->setView(hudView);
    drawButtons();

    // reset view
    pWindow->setView(levelView);

#ifdef LEVELEDITOR_DEBUGVIEW
    if(currentPolygon){
        int vertexNumber = 0;
        sf::VertexArray &vertices = currentPolygon->getViewVertices();
        while(vertexNumber < vertices.getVertexCount()){
//                std::cout << vertices[vertexNumber].position.x << ' ' << vertices[vertexNumber].position.y << ' ' <<
//                vertices[vertexNumber + 1].position.x << ' ' << vertices[vertexNumber + 1].position.y << ' ' <<
//                vertices[vertexNumber + 2].position.x << ' ' << vertices[vertexNumber + 2].position.y << std::endl;
            vertexPoint.setPosition(vertices[vertexNumber].position); triangle[vertexNumber % 3].position = vertices[vertexNumber++].position; pWindow->draw(vertexPoint);
            vertexPoint.setPosition(vertices[vertexNumber].position); triangle[vertexNumber % 3].position = vertices[vertexNumber++].position; pWindow->draw(vertexPoint);
            vertexPoint.setPosition(vertices[vertexNumber].position); triangle[vertexNumber % 3].position = vertices[vertexNumber++].position; pWindow->draw(vertexPoint);
            pWindow->draw(triangle, &plainRedTexture);
        }
    }
#endif

    // show our work
    pWindow->display();
}

void LevelEditor::run() {
    while(pWindow->isOpen() && !exiting){
        handleEvents();
        setAimPoint();
        updateView();
        updateWindow();
    }

    svLv(levelFname, polygons, weapons, boarders);
}
