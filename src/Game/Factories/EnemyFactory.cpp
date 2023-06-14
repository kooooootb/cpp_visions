#include <filesystem>

#include "EnemyFactory.h"
#include "Enemy.h"
#include "PathController.h"
#include "JSONDataNode.h"

std::vector<std::unique_ptr<Enemy>> EnemyFactory::construct(const DataNode &dataNode, Polygon::Array &polygons, EntityStorage &entityStorage) {
    auto points = dataNode.getPointsVector(POSITIONSKEY);
    auto folder = PathController::getEntityConfig(dataNode.getString(FOLDERKEY));
    auto enemyNode = MainDataNode(folder);

    std::vector<std::unique_ptr<Enemy>> result;

    if(!points.empty()){
        result.push_back(std::make_unique<Enemy>(enemyNode, polygons, entityStorage));
        Enemy &refEnemy = *result.back();
        refEnemy.setPosition(points.front());

        for(int i = 1;i < points.size();++i){
            result.push_back(std::make_unique<Enemy>(refEnemy));
            result.back()->setPosition(points[i]);
        }
    }

    return result;
}
