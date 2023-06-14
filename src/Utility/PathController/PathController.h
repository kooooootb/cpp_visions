#ifndef VISIONS_PATHCONTROLLER_H
#define VISIONS_PATHCONTROLLER_H

#include <filesystem>
#include <memory>

class PathController {
private:
    std::filesystem::path m_rootFolder;

    static std::unique_ptr<PathController> m_object;

    // settings
    static constexpr char SETTINGSFOLDER[] = "Settings";
    static constexpr char KEYBINDINGSFILE[] = "keyBindings.json";
    static constexpr char WINDOWCONFIGFILE[] = "windowConfig.json";

    // levels
    static constexpr char LEVELFOLDER[] = "Levels";
    static constexpr char LEVELCONFIG[] = "levelConfig.json";

    // entities
    static constexpr char ENTITIESFOLDER[] = "Entities";
    static constexpr char ENTITIESCONFIG[] = "entityConfig.json";

    static bool isInitialized();
public:
    explicit PathController(std::filesystem::path rootFolder); // TODO make private
    PathController(const PathController &) = delete;
    PathController(PathController &&) = delete;

    static void initialize(std::filesystem::path rootFolder);

    static std::filesystem::path getKeyBindings();
    static std::filesystem::path getWindowSettings();

    static std::filesystem::path getEntityConfig(const std::string &entityName);
    static std::filesystem::path getEntityFolder(const std::string &entityName);

    static std::filesystem::path getLevelConfig(const std::string &levelName);
    static std::filesystem::path getLevelFolder(const std::string &levelName);
};


#endif //VISIONS_PATHCONTROLLER_H
