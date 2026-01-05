#pragma once //chyba lepsze niż include guards?

#include "types.hxx"
#include "storage_types.hxx"
#include "package.hxx"

#include <optional>
#include <vector>
#include <random>

class Storehouse; 

class Worker{
public:
    Worker(ElementID id, PackageQueueType queue_type, std::size_t processing_time);
    ElementID get_id() const;

    //odbiorcy
    void add_worker_receiver(Worker* worker);
    void add_storehouse_receiver(Storehouse* storehouse);

    virtual void receive_package(Package&& package);
    void do_work();
    bool is_busy() const;
private:
    ElementID id_;
    PackageQueue queue_;
    std::size_t processing_time_;
    std::optional<Package> current_package_;
    std::size_t remaining_time_;

    std::vector<Worker*> worker_receivers_;
    std::vector<Storehouse*> storehouse_receivers_;

    std::mt19937 rng_;

    void send_package();
};