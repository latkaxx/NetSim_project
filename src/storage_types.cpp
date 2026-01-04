#include "storage_types.hxx"
#include <stdexcept>

//PackageQueue
PackageQueue::PackageQueue(PackageQueueType type)
    : type_(type) {}

void PackageQueue::push(Package&& package) {
    container_.push_back(std::move(package));
}

Package PackageQueue::pop() {
    if (container_.empty()) {
        throw std::logic_error("PackageQueue::pop() on empty queue");
    }

    Package result = (type_ == PackageQueueType::FIFO)
        ? std::move(container_.front())
        : std::move(container_.back());

    if (type_ == PackageQueueType::FIFO) {
        container_.pop_front();
    } else {
        container_.pop_back();
    }

    return result;
}

//iteratory
IPackageStockpile::const_iterator PackageQueue::begin() const {
    return container_.begin();
}

IPackageStockpile::const_iterator PackageQueue::end() const {
    return container_.end();
}

IPackageStockpile::const_iterator PackageQueue::cbegin() const {
    return container_.cbegin();
}

IPackageStockpile::const_iterator PackageQueue::cend() const {
    return container_.cend();
}

//info o rozmiarze
std::size_t PackageQueue::size() const {
    return container_.size();
}

bool PackageQueue::empty() const {
    return container_.empty();
}

PackageQueueType PackageQueue::get_queue_type() const {
    return type_;
}
