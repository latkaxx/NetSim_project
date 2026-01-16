#pragma once

#include <ostream>
#include "factory.hxx"
#include "types.hxx"



void generate_structure_report(const Factory& factory, std::ostream& os);


void generate_simulation_report(const Factory& factory, std::ostream& os, Time t);