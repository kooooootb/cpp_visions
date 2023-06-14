#ifndef VISIONS_WATCHER_H
#define VISIONS_WATCHER_H

#include <stack>

class Watcher {
public:
    using Index = int;

private:
    std::stack<Index> ids;

public:
    Watcher() = default;

    void pushId(Index id);
    Index popId();
    bool empty() const;
};


#endif //VISIONS_WATCHER_H
