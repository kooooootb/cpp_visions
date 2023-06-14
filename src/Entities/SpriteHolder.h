#ifndef VISIONS_SPRITEHOLDER_H
#define VISIONS_SPRITEHOLDER_H

#include <memory>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "Drawable.h"
#include "Types.h"
#include "MainDataNode.h"
#include "Point.h"
#include "WindowHolder.h"

/**
 * Держит спрайт объекта, продвигает первую реализацию его отображения
 */
class SpriteHolder : public Drawable {
private:
    // main graphics
    std::shared_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> sprite;

    // i think this is right place for visible variable, closer to sprite
    bool isVisible = true;

    // keeping default texture position
    int defaultX, defaultY, width, height;

    static constexpr char TEXTUREFILE[] = "texture.png";
    static constexpr char TEXTUREKEY[] = "texture";
    static constexpr char XKEY[] = "text_x";
    static constexpr char YKEY[] = "text_y";
    static constexpr char WIDTHKEY[] = "text_width";
    static constexpr char HEIGHTKEY[] = "text_height";

    void initializeGraphics(const std::filesystem::path &file);
    void fillData(const DataNode &dataNode);
    void fillData(const SpriteHolder &sh);
protected:
    void repositionTexture(txtm_t x, txtm_t y);
    void repositionTexture(const std::pair<txtm_t, txtm_t> &pair);

    void resetSprite();

    void setSpritePosition(const Point &point);
    void setSpriteAngle(double angle);
public:
    explicit SpriteHolder(const DataNode &dataNode);

    SpriteHolder(const SpriteHolder &sh);
    SpriteHolder(SpriteHolder &&sh) = default;

    ~SpriteHolder() override = default;

    void setVisibility(bool isVisible_);

    void draw(WindowHolder &window) const override;

    std::string serialize() const;
};


#endif //VISIONS_SPRITEHOLDER_H
