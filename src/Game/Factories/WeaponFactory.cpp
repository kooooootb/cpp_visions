#include <filesystem>

#include "Weapon.h"
#include "WeaponFactory.h"
#include "PathController.h"
#include "JSONDataNode.h"

std::vector<std::unique_ptr<Weapon>> WeaponFactory::construct(const DataNode &dataNode, Polygon::Array &polygons, EntityStorage &entityStorage) {
    auto points = dataNode.getPointsVector(POSITIONSKEY);
    auto folder = PathController::getEntityConfig(dataNode.getString(FOLDERKEY));
    auto weaponNode = MainDataNode(folder);

    std::vector<std::unique_ptr<Weapon>> result;

    if(!points.empty()){
        result.push_back(std::make_unique<Weapon>(weaponNode, polygons, entityStorage));
        Weapon &refWeapon = *result.back();
        refWeapon.setPosition(points.front());

        for(int i = 1;i < points.size();++i){
            result.push_back(std::make_unique<Weapon>(refWeapon));
            result.back()->setPosition(points[i]);
        }
    }

    return result;
}
