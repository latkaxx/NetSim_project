#include "worker.hxx"

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue)
    : PackageSender()
    , id_(id)
    , queue_(std::move(queue))
    , processing_duration_(pd)
    , processing_start_time_(std::nullopt)
    , current_package_(std::nullopt)
{}

ElementID Worker::get_id() const {
    return id_;
}

void Worker::receive_package(Package&& package) {
    queue_->push(std::move(package));
}

void Worker::do_work(Time t) {
    // 1. jeśli NIC nie przetwarza i coś jest w kolejce → rozpocznij przetwarzanie
    if (!current_package_.has_value() && !queue_->empty()) {
        current_package_ = queue_->pop();
        processing_start_time_ = t;
    }

    // 2. jeśli przetwarza i minął czas → zakończ przetwarzanie
    if (current_package_.has_value() && processing_start_time_.has_value()) {
        if (t - *processing_start_time_ + 1 >= processing_duration_) {
            // 🔥 kluczowe dla testu HasBuffer
            push_package(std::move(*current_package_));

            current_package_.reset();
            processing_start_time_.reset();
        }
    }
}

Time Worker::get_package_processing_start_time() const {
    return processing_start_time_.value_or(0);
}

TimeOffset Worker::get_processing_duration() const {
    return processing_duration_;
}
