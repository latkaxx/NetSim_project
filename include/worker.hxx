#pragma once

#include "types.hxx"
#include "package_sender.hxx"
#include "ipackage_receiver.hxx"
#include "storage_types.hxx"
#include "package.hxx"
#include "receiver_preferences.hxx"

#include <memory>
#include <optional>

class Worker : public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue);

    ElementID get_id() const override;

    void receive_package(Package&& package) override;
    void do_work(Time t);
    ReceiverPreferences& get_receiver_preferences();
    const ReceiverPreferences& get_receiver_preferences() const;

    Time get_package_processing_start_time() const;
    TimeOffset get_processing_duration() const;


    IPackageQueue* get_queue();
    const IPackageQueue* get_queue() const;

    void remove_receiver(IPackageReceiver* r);

    const std::optional<Package>& get_processing_package() const;
    

private:
    ElementID id_;
    std::unique_ptr<IPackageQueue> queue_;
    TimeOffset processing_duration_;
    std::optional<Time> processing_start_time_;
    std::optional<Package> current_package_; 
    ReceiverPreferences receiver_preferences_;
};
