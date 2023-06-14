#include "LevelEditor.h"
#include "Polygon.h"

LevelEditor::LevelEditor(std::shared_ptr<sf::RenderWindow> &pWindow_, std::string levelFileName) :
    pWindow(pWindow_),
    currentLine(sf::LineStrip),
    bgVertices(sf::Quads, 4),
    event(),
    viewSpeed(0, 0),
    viewAcceleration(0, 0),
    levelView(sf::View(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(pWindow->getSize()))),
    hudView(sf::View(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(pWindow->getSize()))),
    boarders(-1, -1),
    saveFileName(std::move(levelFileName)),
    insertionMode(static_cast<InsertionMode>(0)),
    modeNames{"Polygon", "Weapon", "Player", "Enemies"}
{
    // editor variables
    isPrecise = true;
    aimPointShape = setAimPointShape();
    precision = 20;
    insertionMode = POLYGON;

    // try to load level file, if can't -> prompt for dimensions
    try{
        polygons = ldPo(saveFileName);
        setBoarders(ldBd(saveFileName));
        weapons = ldWp(saveFileName);
    } catch(std::runtime_error &e){
        promptBoarders();
    }

    // set view
    pWindow->setView(levelView);

    // set buttons
    setButtons();

    // set modes shapes
    setModes();

    #ifdef LEVELEDITOR_DEBUGVIEW
    vertexPoint = sf::CircleShape(3);
    vertexPoint.setFillColor(sf::Color::Red);
    vertexPoint.setOrigin(1.5, 1.5);
    triangle = sf::VertexArray(sf::Lines, 3);
    plainRedTexture = createPlainTexture(1, 1, sf::Color::Red);
    #endif
}

void LevelEditor::setBoarders(BoarderType newBoarders) {
    boarders = newBoarders;

    // set background
    createBackground(bgVertices, bgTexture, boarders.first, boarders.second);

    // set unprecise grid
    unpreciseGridVertices = createGridVertices(boarders.first, boarders.second, precision);
    unpreciseGridTexture = createPlainTexture(boarders.first, boarders.second, sf::Color(25, 25, 25, 255));

    // save level at the end
    needSave = true;
}

void LevelEditor::setModes() {
    sf::Vector2f windowSize2 = {(float) pWindow->getSize().x / 2, (float) pWindow->getSize().y / 2};
    float width = windowSize2.x * 2 * MODEPANELWIDTH;
    float height = windowSize2.y * 2 * MODEPANELHEIGHT;

    // set panel
    modeBgVertices = sf::VertexArray(sf::TriangleFan, 4);
    modeBgVertices[0].position = sf::Vector2f(0, 0) - windowSize2;
    modeBgVertices[1].position = sf::Vector2f(width, 0) - windowSize2;
    modeBgVertices[2].position = sf::Vector2f(width, height) - windowSize2;
    modeBgVertices[3].position = sf::Vector2f(0, height) - windowSize2;
    modeBgTexture = createPlainTexture(1, 1, sf::Color(149, 28, 0, 150));

    // set texts
    if(!modeFont.loadFromFile("arial.ttf")){
        throw std::runtime_error("Can't load font for buttons");
    }
    for(int i = 0;i < static_cast<unsigned>(MODESLENGTH);++i){
        modeTexts[i] = sf::Text(modeNames[i], modeFont, (unsigned) (height * 0.5f));
        modeTexts[i].setOutlineThickness(1);
        modeTexts[i].setOutlineColor(sf::Color::Black);
        modeTexts[i].setOrigin(modeTexts[i].getLocalBounds().width / 2, modeTexts[i].getLocalBounds().height / 2);
        modeTexts[i].setPosition(-windowSize2 + sf::Vector2f(width / 2, height * (0.5f + (float) i)));
    }
}

void LevelEditor::promptBoarders(){
    // add prompts
    float width3 = (float) pWindow->getSize().x / 3;
    float height3 = (float) pWindow->getSize().y / 3;
    float x = -width3;
    float y = -height3;
    float width = 2 * width3;
    float height = 2 * height3;
    sf::Color color = sf::Color(33, 85, 0, 160);

    auto checkFunction = [](const sf::String &sfString) -> bool {
        std::string numstr = sfString.toAnsiString();
        try{
            std::stoi(numstr);
        } catch(std::invalid_argument &e){
            return false;
        } catch(std::out_of_range &e){
            return false;
        }
        return true;
    };

    // one for width
    promptQueue.emplace(*this, [](LevelEditor &outer, const sf::String &sfString) -> void {
        outer.boarders.first = std::stoi(sfString.toAnsiString());
    }, [](LevelEditor &outer) -> void {
        // nothing to perform on ok
    }, checkFunction, x, y, width, height, color, "Input level's width", "1000");

    // and one for height (with addBoarders performing)
    promptQueue.emplace(*this, [](LevelEditor &outer, const sf::String &sfString) -> void {
        BoarderType::first_type boarder = std::stoi(sfString.toAnsiString());
        outer.setBoarders({outer.boarders.first, boarder});

        addBoarders(outer.polygons, outer.boarders);
    }, [](LevelEditor &outer) -> void {
        // nothing to perform on ok
    }, checkFunction, x, y, width, height, color, "Input level's height", "1000");
}

bool LevelEditor::areBoardersSet() const {
    return boarders.first > 0 && boarders.second  > 0;
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

    // set mode panel
    sf::Vector2f windowSize2 = {(float) pWindow->getSize().x / 2, (float) pWindow->getSize().y / 2};
    float width = windowSize2.x * 2 * MODEPANELWIDTH;
    float height = windowSize2.y * 2 * MODEPANELHEIGHT;
    sf::Vector2f offset = {0, height * (float) insertionMode};
    modeBgVertices[0].position = sf::Vector2f(0, 0) - windowSize2 + offset;
    modeBgVertices[1].position = sf::Vector2f(width, 0) - windowSize2 + offset;
    modeBgVertices[2].position = sf::Vector2f(width, height) - windowSize2 + offset;
    modeBgVertices[3].position = sf::Vector2f(0, height) - windowSize2 + offset;
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

void LevelEditor::handlePromptClick() {
    Prompt<LevelEditor> &prompt = promptQueue.front();
    sf::Vector2f mousePos = getMousePosition(*pWindow, pWindow->getView());
    if(prompt.handleClick(mousePos)){
        promptQueue.pop();
    }
}

void LevelEditor::handleNormalEvent(){
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

void LevelEditor::handlePromptEvent(){
    // work with enter or escape presses
    if(event.type == sf::Event::KeyPressed){
        switch(event.key.code) {
            case sf::Keyboard::Enter:
                if(promptQueue.front().approve()){
                    promptQueue.pop();
                }
                return;
            case sf::Keyboard::Escape:
                exitEditor();
                return;
            default:
                break;
        }
    }

    // other events
    switch(event.type){
        case sf::Event::Closed:
            pWindow->close();
            exitEditor();
            break;
        case sf::Event::TextEntered:
            promptQueue.front().addInput(event.text.unicode);
            break;
        case sf::Event::MouseButtonPressed:
            handlePromptClick();
            break;
        default:
            //default event
            break;
    }
}

void LevelEditor::handleEvents(){
    while (pWindow->pollEvent(event)) {
        if(promptQueue.empty()){
            handleNormalEvent();
        } else{
            handlePromptEvent();
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
    if(!areBoardersSet()){
        return;
    }

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

void LevelEditor::drawPrompt() {
    if(promptQueue.empty()){
        return;
    }

    const Prompt<LevelEditor> &prompt = promptQueue.front();
    prompt.draw(*pWindow);
}

void LevelEditor::drawModes(){
    // highlight current mode
    pWindow->draw(modeBgVertices, &modeBgTexture);

    // draw all mode texts
    for(const auto &text : modeTexts) {
        pWindow->draw(text);
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
    drawPrompt();
    drawModes();

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

    if(needSave){
        svLv(saveFileName, polygons, weapons, boarders);
    }
}
