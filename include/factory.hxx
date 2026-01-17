#pragma once

#include "loading_ramp.hxx"
#include "worker.hxx"
#include "storehouse.hxx"
#include "types.hxx"

#include <list>
#include <algorithm>

template <typename Node>
class NodeCollection {
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() { return container_.begin(); }
    iterator end() { return container_.end(); }

    const_iterator begin() const { return container_.begin(); }
    const_iterator end() const { return container_.end(); }

    const_iterator cbegin() const { return container_.cbegin(); }
    const_iterator cend() const { return container_.cend(); }

    void add(Node&& node) {
        container_.push_back(std::move(node));
    }

    void remove_by_id(ElementID id) {
        container_.remove_if(
            [id](const Node& n) { return n.get_id() == id; }
        );
    }

    iterator find_by_id(ElementID id) {
        return std::find_if(container_.begin(), container_.end(),
            [id](const Node& n) { return n.get_id() == id; });
    }

    const_iterator find_by_id(ElementID id) const {
        return std::find_if(container_.cbegin(), container_.cend(),
            [id](const Node& n) { return n.get_id() == id; });
    }

private:
    container_t container_;
};

class Factory {
public:
    // dodawanie / usuwanie
    void add_ramp(Ramp&&);
    void remove_ramp(ElementID);

    void add_worker(Worker&&);
    void remove_worker(ElementID);

    void add_storehouse(Storehouse&&);
    void remove_storehouse(ElementID);

    // wyszukiwanie 
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID);
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID) const;

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID);
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID) const;

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID);
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID) const;

    // teratory
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const;
    NodeCollection<Ramp>::const_iterator ramp_cend() const;

    NodeCollection<Worker>::const_iterator worker_cbegin() const;
    NodeCollection<Worker>::const_iterator worker_cend() const;

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const;
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const;

    // symulacja
    bool is_consistent() const;

    void do_deliveries(Time);
    void do_work(Time);
    void do_package_passing();

private:
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;

    template <typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
};
