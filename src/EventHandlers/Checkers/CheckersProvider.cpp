#include <utility>

#include "CheckersProvider.h"

CheckersProvider::CheckersProvider(std::unique_ptr<DataNode> &&dataNode) : m_dataNode(std::move(dataNode))
{}

void CheckersProvider::addChecker(std::unique_ptr<Checker> &&checker) {
    m_checkers.push_back(std::move(checker));
}

void CheckersProvider::executeCheckers() {
    for(const auto &checker : m_checkers){
        checker->checkAndExecute();
    }
}
