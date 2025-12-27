#include "package.hxx"
#include <stdexcept>

// pola statyczne
std::set<ElementID> Package::assigned_ids_;
std::set<ElementID> Package::freed_ids_;

//pomocnicze funkcje
ElementID Package::generate_id() {
    if (!freed_ids_.empty()) {
        auto it = freed_ids_.begin();
        ElementID id = *it;
        freed_ids_.erase(it);
        assigned_ids_.insert(id);
        return id;
    }

    ElementID new_id = assigned_ids_.empty() ? 1 : (*assigned_ids_.rbegin() + 1);
    assigned_ids_.insert(new_id);
    return new_id;
}

//kontruktory
Package::Package()
    : id_(generate_id()) {}

Package::Package(ElementID id)
    : id_(id) {
    assigned_ids_.insert(id_);
}

Package::Package(Package&& other) noexcept
    : id_(other.id_) {
    other.id_ = 0;
}

// operator przypisania
Package& Package::operator=(Package&& other) noexcept {
    if (this != &other) {
        // zwolnij swoje ID
        if (id_ != 0) {
            assigned_ids_.erase(id_);
            freed_ids_.insert(id_);
        }

        id_ = other.id_;
        other.id_ = 0;
    }
    return *this;
}

// metoda
ElementID Package::get_id() const {
    return id_;
}

// destruktor, jeśli ID nie jest zerowe, zwalnia je
Package::~Package() {
    if (id_ != 0) {
        assigned_ids_.erase(id_);
        freed_ids_.insert(id_);
    }
}
