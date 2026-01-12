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

protected:
    ReceiverPreferences receiver_preferences_;
    std::optional<Package> sending_buffer_;
};
