#include "receiver_preferences.hxx"

ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator pg)
    : pg_(pg) {}

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    preferences_[r] = 1.0;
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    preferences_.erase(r);
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    if (preferences_.empty()) return nullptr;
    return preferences_.begin()->first; // NA RAZIE NAJPROŚCIEJ
}

preferences_t& ReceiverPreferences::get_preferences() {
    return preferences_;
}
