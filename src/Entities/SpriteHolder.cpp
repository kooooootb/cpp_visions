#include <fstream>
#include <sstream>
#include <cassert>
#include <filesystem>

#include "SpriteHolder.h"
#include "Utility.h"

SpriteHolder::SpriteHolder(const DataNode &dataNode) {
    initializeGraphics(dataNode.contains(TEXTUREKEY)
            ? dataNode.getFolder() / dataNode.getString(TEXTUREKEY)
            : dataNode.getFolder() / TEXTUREFILE);
    fillData(dataNode);
    resetSprite();
}

SpriteHolder::SpriteHolder(const SpriteHolder &sh) : texture(sh.texture) {
    sprite = std::make_unique<sf::Sprite>();
    sprite->setTexture(*texture);
    fillData(sh);
    resetSprite();
}

void SpriteHolder::initializeGraphics(const std::filesystem::path &file) {
    texture = std::make_shared<sf::Texture>();
    bool loaded = texture->loadFromFile(file.string());
    assert(loaded && "Can't load TEXTUREFILE in SpriteHolder");

    sprite = std::make_unique<sf::Sprite>();
    sprite->setTexture(*texture);
}

void SpriteHolder::fillData(const DataNode &dataNode) {
    defaultX = (int) dataNode.getNumber(XKEY);
    defaultY = (int) dataNode.getNumber(YKEY);
    width = (int) dataNode.getNumber(WIDTHKEY);
    height = (int) dataNode.getNumber(HEIGHTKEY);
}

void SpriteHolder::fillData(const SpriteHolder &sh) {
    defaultX = sh.defaultX;
    defaultY = sh.defaultY;
    width = sh.width;
    height = sh.height;
}

void SpriteHolder::repositionTexture(txtm_t x, txtm_t y) {
    sprite->setTextureRect({x, y, width, height});
}

void SpriteHolder::repositionTexture(const std::pair<txtm_t, txtm_t> &pair) {
    repositionTexture(pair.first, pair.second);
}

void SpriteHolder::resetSprite(){
    repositionTexture(defaultX, defaultY);
    sprite->setOrigin(static_cast<float>(width) / 2, static_cast<float>(height) / 2);
}

void SpriteHolder::setVisibility(bool isVisible_){
    isVisible = isVisible_;
}

void SpriteHolder::draw(WindowHolder &window) const {
    if(isVisible){
        window.draw(*sprite);
    }
}

void SpriteHolder::setSpritePosition(const Point &point){
    sprite->setPosition(point.toSfVector());
}

void SpriteHolder::setSpriteAngle(double angle){
    sprite->setRotation((float) Util::radToDeg(angle));
}

std::string SpriteHolder::serialize() const {
    using Formatter = JSONDataNode;
    return Formatter::formatNumber(XKEY, defaultX) + "," +
           Formatter::formatNumber(YKEY, defaultY) + "," +
           Formatter::formatNumber(WIDTHKEY, width) + "," +
           Formatter::formatNumber(HEIGHTKEY, height);
}
