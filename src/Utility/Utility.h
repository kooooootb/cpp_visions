#ifndef VISIONS_UTILITY_H
#define VISIONS_UTILITY_H

#include "Constants.h"

namespace Util{
    constexpr double radToDeg(double rad){
        return rad * 180 / PI_VALUE;
    }

    constexpr double degToRad(double deg){
        return deg * PI_VALUE / 180;
    }

    template<typename T>
    bool almostEqual(T el1 ,T el2){
        return std::abs(el1 - el2) <= std::numeric_limits<T>::epsilon() * std::abs(el1 + el2) * 200. ||
               std::abs(el1 - el2) <= std::numeric_limits<T>::min();
    }
}


#endif //VISIONS_UTILITY_H
