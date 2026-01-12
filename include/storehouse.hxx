#pragma once

#include <memory>

#include "types.hxx"
#include "package.hxx"
#include "ipackage_receiver.hxx"
#include "storage_types.hxx"

class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> stockpile);

    ElementID get_id() const;

    void receive_package(Package&& package) override;

    const IPackageStockpile& get_stockpile() const;

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> stockpile_;
};