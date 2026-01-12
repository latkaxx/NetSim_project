#ifndef TYPES_HXX
#define TYPES_HXX

#include <cstddef>
#include <functional>

//Typy proste
using ElementID = std::size_t;
using Time = std::size_t;
using TimeOffset = std::size_t;

//Typ wyliczeniowy kolejek
enum class PackageQueueType {
    FIFO,
    LIFO
};

//Typ generatora prawdopodobieństwa
using ProbabilityGenerator = std::function<double()>;

#endif