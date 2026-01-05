#include "loading_ramp.hxx"
#include "worker.hxx"

#include <random>

LoadingRamp::LoadingRamp(ElementID id, std::size_t delivery_interval)
    : id_(id), delivery_interval_(delivery_interval) {}

ElementID LoadingRamp::get_id() const {
    return id_;
}

//dodawanie odbiorcy
void LoadingRamp::add_receiver(Worker* worker) {
    receivers_.push_back(worker);
}

//sprawdzanie czy w tej turze jest dostawa
void LoadingRamp::deliver_package(std::size_t current_round) {
    if ((current_round-1)%delivery_interval_ !=0) {
        return;
    }
    if (receivers_.empty()) {
        return;
    }
    Package new_package;
    Worker* receiver = choose_receiver();
    receiver->receive_package(std::move(new_package));
}

//losowanie odbiorcy
Worker* LoadingRamp::choose_receiver() {
    static std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, receivers_.size() - 1);
    return receivers_[dist(gen)];
}