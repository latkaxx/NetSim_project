#pragma once
#include <map>
#include <random>
#include "ipackage_receiver.hxx"

using ProbabilityGenerator = std::mt19937;
using preferences_t = std::map<IPackageReceiver*, double>;

class ReceiverPreferences {
public:
    explicit ReceiverPreferences(ProbabilityGenerator pg);

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    preferences_t& get_preferences();

private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;
};
