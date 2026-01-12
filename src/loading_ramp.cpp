#include "loading_ramp.hxx"

#include <stdexcept>

Ramp::Ramp(ElementID id, TimeOffset delivery_interval)
    : id_(id), delivery_interval_(delivery_interval) {}

ElementID Ramp::get_id() const {
    return id_;
}
TimeOffset Ramp::get_delivery_interval() const {
    return delivery_interval_;
}
ReceiverPreferences& Ramp::get_receiver_preferences() {
    return receiver_preferences_;
}
const ReceiverPreferences& Ramp::get_receiver_preferences() const {
    return receiver_preferences_;
}
void Ramp::add_receiver(Worker* worker) {
    receivers_.push_back(worker);
    receiver_preferences_.add_receiver(worker); 
}
void Ramp::remove_receiver(IPackageReceiver* receiver) {
    receiver_preferences_.remove_receiver(receiver);
}



void Ramp::deliver_goods(Time current_time) {
    if ((current_time % delivery_interval_) != 0) return;
    if (receivers_.empty()) return;

    Package p;
    push_package(std::move(p));  
    send_package();             
}

Worker* Ramp::choose_receiver() {
    if (receivers_.empty()) return nullptr;
    double r = probability_generator();
    size_t idx = static_cast<size_t>(r * receivers_.size());
    if (idx >= receivers_.size()) idx = receivers_.size() - 1;
    return receivers_[idx];
}
