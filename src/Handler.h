#ifndef HANDLER_H
#define HANDLER_H

#include <functional>

#include "Types.h"

namespace nbrecords {

template<typename T>
class Handler
{
protected:
    Handler() = default;
    template <typename Function>
    auto asHandler(Function function) {
        return [function, this](auto arg){
            std::invoke(function, static_cast<T*>(this), arg);
        };
    }
};

} // namespace nbrecords
#endif // HANDLER_H
