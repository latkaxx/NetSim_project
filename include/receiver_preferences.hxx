#pragma once

#include "helpers.hxx"
#include "ipackage_receiver.hxx"

#include <map>

using preferences_t = std::map<IPackageReceiver*, double>;
using ProbabilityGenerator = std::function<double()>;

class ReceiverPreferences {
public:
    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator);

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    preferences_t& get_preferences();

private:
    void normalize_preferences();

    preferences_t preferences_;
    ProbabilityGenerator pg_;
};
