#include "receiver_preferences.hxx"

ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator pg)
    : pg_(pg) {}

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    preferences_[r] = 1.0;
    normalize_preferences();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    preferences_.erase(r);
    normalize_preferences();
}

void ReceiverPreferences::normalize_preferences() {
    if (preferences_.empty()) return;

    double p = 1.0 / preferences_.size();
    for (auto& [receiver, prob] : preferences_) {
        prob = p;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    if (preferences_.empty()) return nullptr;

    double r = pg_();          // [0,1)
    double acc = 0.0;

    for (const auto& [receiver, prob] : preferences_) {
        acc += prob;
        if (r < acc) {
            return receiver;
        }
    }

    // zabezpieczenie na błędy numeryczne
    return preferences_.begin()->first;
}

preferences_t& ReceiverPreferences::get_preferences() {
    return preferences_;
}
