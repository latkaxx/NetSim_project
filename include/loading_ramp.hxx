#ifndef LOADING_RAMP_HXX
#define LOADING_RAMP_HXX

#include "types.hxx"
#include "package.hxx"

#include <vector>
#include <memory>
#include <random>

class Worker; //agregacja

class LoadingRamp {
public:
    LoadingRamp(ElementID id, std::size_t delivery_interval);
    ElementID get_id() const;

    void add_receiver(Worker* worker);

    void deliver_package(std::size_t current_round);

private:
    ElementID id_;
    std::size_t delivery_interval_;
    std::vector<Worker*> receivers_;
    Worker* choose_receiver();
    std::mt19937 rng_;
};

#endif