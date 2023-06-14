#include "Watcher.h"

void Watcher::pushId(Index id) {
    ids.push(id);
}

Watcher::Index Watcher::popId() {
    Index result = ids.top();
    ids.pop();
    return result;
}

bool Watcher::empty() const {
    return ids.empty();
}
