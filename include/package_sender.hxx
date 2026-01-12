#pragma once
#include <optional>
#include "receiver_preferences.hxx"
#include "package.hxx"
#include "helpers.hxx"

class PackageSender {
public:
    PackageSender() : receiver_preferences_(ProbabilityGenerator()) {}
    PackageSender(PackageSender&&) = default;

    void send_package();
    void push_package(Package&& p);
    std::optional<Package>& get_sending_buffer();
    ReceiverPreferences& receiver_preferences() { return receiver_preferences_; }
    const ReceiverPreferences& receiver_preferences() const { return receiver_preferences_; }
    const ReceiverPreferences& get_receiver_preferences() const {
        return receiver_preferences_;
    }
    
    ReceiverPreferences& get_receiver_preferences() {
        return receiver_preferences_;
    }


protected:
    ReceiverPreferences receiver_preferences_;
    std::optional<Package> sending_buffer_;
};
