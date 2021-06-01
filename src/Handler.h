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
        return [function, this](Message message){
            std::invoke(function, static_cast<T*>(this), message);
        };
    }
};

} // namespace nbrecords
#endif // HANDLER_H
