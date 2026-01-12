#pragma once

#include <memory>

#include "types.hxx"
#include "package.hxx"
#include "ipackage_receiver.hxx"
#include "storage_types.hxx"

class Storehouse : public IPackageReceiver {
public:
    //domyślnie FIFO
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> stockpile = 
        std::make_unique<PackageQueue>(PackageQueueType::FIFO));

    ElementID get_id() const override;

    void receive_package(Package&& package) override;

    const IPackageStockpile& get_stockpile() const;

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> stockpile_;
};