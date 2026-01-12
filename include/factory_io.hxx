#pragma once

#include "factory.hxx"

#include <istream>


Factory load_factory_structure(std::istream& is);
void save_factory_structure(const Factory& factory, std::ostream& os);