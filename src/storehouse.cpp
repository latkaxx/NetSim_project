#include "storehouse.hxx"

Storehouse::Storehouse(ElementID id,
                       std::unique_ptr<IPackageStockpile> stockpile)
    : id_(id), stockpile_(std::move(stockpile)) {}

ElementID Storehouse::get_id() const {
    return id_;
}

void Storehouse::receive_package(Package&& package) {
    stockpile_->push(std::move(package));
}

const IPackageStockpile& Storehouse::get_stockpile() const {
    return *stockpile_;
}