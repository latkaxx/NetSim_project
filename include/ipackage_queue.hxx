#pragma once
#include "package.hxx"

class IPackageQueue {
public:
    virtual void push(Package&& p) = 0;
    virtual Package pop() = 0;
    virtual bool empty() const = 0;
    virtual ~IPackageQueue() = default;
};