#include "factory.hxx"

// konstruktory/destruktory
void Factory::add_ramp(Ramp&& r) {
    ramps_.add(std::move(r));
}

void Factory::remove_ramp(ElementID id) {
    ramps_.remove_by_id(id);
}

void Factory::add_worker(Worker&& w) {
    workers_.add(std::move(w));
}

void Factory::remove_worker(ElementID id) {
    remove_receiver(workers_, id);
}

void Factory::add_storehouse(Storehouse&& s) {
    storehouses_.add(std::move(s));
}

void Factory::remove_storehouse(ElementID id) {
    remove_receiver(storehouses_, id);
}

// find
NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id) {
    return ramps_.find_by_id(id);
}

NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const {
    return ramps_.find_by_id(id);
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id) {
    return workers_.find_by_id(id);
}

NodeCollection<Worker>::const_iterator Factory::find_worker_by_id(ElementID id) const {
    return workers_.find_by_id(id);
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id) {
    return storehouses_.find_by_id(id);
}

NodeCollection<Storehouse>::const_iterator Factory::find_storehouse_by_id(ElementID id) const {
    return storehouses_.find_by_id(id);
}

// iteratory
NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() const {
    return ramps_.cbegin();
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cend() const {
    return ramps_.cend();
}

NodeCollection<Worker>::const_iterator Factory::worker_cbegin() const {
    return workers_.cbegin();
}

NodeCollection<Worker>::const_iterator Factory::worker_cend() const {
    return workers_.cend();
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin() const {
    return storehouses_.cbegin();
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend() const {
    return storehouses_.cend();
}


template <typename Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    collection.remove_by_id(id);

    for (auto& w : workers_) {
        w.remove_receiver(id);
    }
    for (auto& r : ramps_) {
        r.remove_receiver(id);
    }
}


bool Factory::is_consistent() const {
    for (const auto& r : ramps_) {
        if (r.receiver_preferences_.get_preferences().empty()) {
            return false;
        }
    }
    for (const auto& w : workers_) {
        if (w.receiver_preferences_.get_preferences().empty()) {
            return false;
        }
    }
    return true;
}

// symulacja
void Factory::do_deliveries(Time t) {
    for (auto& r : ramps_) {
        r.deliver_goods(t);
    }
}

void Factory::do_work(Time t) {
    for (auto& w : workers_) {
        w.do_work(t);
    }
}

void Factory::do_package_passing() {
    for (auto& r : ramps_) {
        r.send_package();
    }
    for (auto& w : workers_) {
        w.send_package();
    }
}
