#pragma once
#include "package.hxx"

class IPackageStockpile {
public:
    virtual void push(Package&& p) = 0;
    virtual ~IPackageStockpile() = default;
};
