#ifndef TYPES_HXX
#define TYPES_HXX

#include <cstddef>

//Typy proste
using ElementID = std::size_t;

//Typ wyliczeniowy kolejek
enum class PackageQueueType {
    FIFO,
    LIFO
};

#endif