#pragma once

#include <functional>
#include "factory.hxx"
#include "types.hxx"

void simulate(
    Factory& factory,
    TimeOffset duration,
    std::function<void(Factory&, Time)> report_callback
);
