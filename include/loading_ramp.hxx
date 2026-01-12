#pragma once
#include "types.hxx"
#include "package.hxx"
#include "worker.hxx"
#include "package_sender.hxx"
#include "helpers.hxx"
#include "receiver_preferences.hxx"

#include <vector>
#include <memory>
#include <random>

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset delivery_interval);

    ElementID get_id() const;

    void add_receiver(Worker* worker);

    void deliver_goods(Time current_time);
    
    void remove_receiver(IPackageReceiver* r) {
        receiver_preferences_.remove_receiver(r);
    }
    
    const ReceiverPreferences& get_receiver_preferences() const {
        return receiver_preferences_;
    }

private:
    ElementID id_;
    TimeOffset delivery_interval_;
    std::vector<Worker*> receivers_;

    Worker* choose_receiver();
};
