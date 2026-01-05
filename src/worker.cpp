#include "worker.hxx"
#include "storehouse.hxx"

#include <random>
#include <stdexcept>

Worker::Worker(ElementID id, PackageQueueType queue_type, std::size_t processing_time)
    : id_(id), queue_(queue_type), processing_time_(processing_time),
      current_package_(std::nullopt), remaining_time_(0), rng_(std::random_device{}()) {}

ElementID Worker::get_id() const {
    return id_;
}

void Worker::add_worker_receiver(Worker* worker) {
    worker_receivers_.push_back(worker);
}

void Worker::add_storehouse_receiver(Storehouse* storehouse) {
    storehouse_receivers_.push_back(storehouse);
}

void Worker::receive_package(Package&& package) {
    queue_.push(std::move(package));
}

bool Worker::is_busy() const {
    return current_package_.has_value();
}

void Worker::do_work() {
    //jeśli pracownik nie pracuje i ma coś w kolejce, to bierze kolejny półprodukt
    if (!is_busy() && !queue_.empty()) {
        current_package_ = queue_.pop();
        remaining_time_ = processing_time_;
    }

    //nadal pracuje, zmniejsza licznik
    if (is_busy()) {
        --remaining_time_;

        //skończył pracę nad półproduktem
        if (remaining_time_ == 0) {
            send_package();
            current_package_.reset();
        }
    }
}

//coś tu śmierdzi z logiką chyba, musze przekminić
//TODO: receive_package będzie dziedziczyć po IPackageReceiver, którego jeszcze nie ma

void Worker::send_package() {
    if (!current_package_.has_value()) {
        return;
    }

    std::vector<void*> all_receivers;
    for (auto* worker : worker_receivers_) {
        all_receivers.push_back(static_cast<void*>(worker));
    }
    for (auto* storehouse : storehouse_receivers_) {
        all_receivers.push_back(static_cast<void*>(storehouse));
    }

    if (all_receivers.empty()) {
        return;
    }

    std::uniform_int_distribution<> dist(0, all_receivers.size() - 1);
    void* chosen = all_receivers[dist(rng_)];

    if(Worker* worker = static_cast<Worker*>(chosen)) {
        worker->receive_package(std::move(*current_package_));
    } //else if(Storehouse* storehouse = static_cast<Storehouse*>(chosen)) {
       // storehouse->receive_package(std::move(*current_package_));
   // } do odkomentowania po zbudowaniu storehouse
}