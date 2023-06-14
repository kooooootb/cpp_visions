#ifndef VISIONS_SERIALIZABLE_H
#define VISIONS_SERIALIZABLE_H

#include <string>

class Serializable{
public:
    virtual std::string serialize() const = 0;
    // should also be static deserialize
};

#endif //VISIONS_SERIALIZABLE_H
