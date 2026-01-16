#include "reports.hxx"

void generate_structure_report(Factory& factory, std::ostream& os) {

    os << ";== LOADING RAMPS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING RAMP id=" << it->get_id() << " deliver-interval=" << it->get_delivery_interval() << "\n";
    }

    os << "\n;== WORKERS ==\n\n";
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        os << "WORKER id=" << it->get_id() << " processing-time=" << it->get_processing_duration();
        auto queue_type = it->get_queue()->get_queue_type();
        os << " queue-type=" << (queue_type == PackageQueueType::FIFO ? "FIFO" : "LIFO") << "\n";
    }

    os << "\n;== STOREHOUSES ==\n\n";
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE id=" << it->get_id() << "\n";
    }

    os << "\n;== LINKS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        const auto& prefs = it->receiver_preferences().get_preferences();
        for (const auto& [receiver, prob] : prefs) {
            if (auto w = dynamic_cast<const Worker*>(receiver)) {
                os << "LINK src=ramp-" << it->get_id() << " dest=worker-" << w->get_id() << "\n";
            } else if (auto s = dynamic_cast<const Storehouse*>(receiver)) {
                os << "LINK src=ramp-" << it->get_id() << " dest=store-" << s->get_id() << "\n";
            }
        }
    }
}   
