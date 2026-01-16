#include "reports.hxx"
#include "worker.hxx"
#include "storehouse.hxx"
#include "factory.hxx"
#include "package.hxx"

    #include <sstream>

void generate_structure_report(const Factory& factory, std::ostream& os) {

    os << "== LOADING RAMPS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING RAMP #" << it->get_id() << "\n";
        os << "  Delivery interval: " << it->get_delivery_interval() << "\n";
        os << "  Receivers:\n";

        const auto& prefs = it->receiver_preferences().get_preferences();
        for (const auto& [receiver, prob] : prefs) {
            if (auto w = dynamic_cast<const Worker*>(receiver)) {
                os << "    worker #" << w->get_id() << "\n";
            }
        }
        os << "\n";
    }

    os << "\n== WORKERS ==\n\n";
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        os << "WORKER #" << it->get_id() << "\n";
        os << "  Processing time: " << it->get_processing_duration() << "\n";
        os << "  Queue type: "
           << (it->get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO")
           << "\n";
        os << "  Receivers:\n";

        const auto& prefs = it->receiver_preferences().get_preferences();
        for (const auto& [receiver, prob] : prefs) {
            if (auto w = dynamic_cast<const Worker*>(receiver)) {
                os << "    worker #" << w->get_id() << "\n";
            } else if (auto s = dynamic_cast<const Storehouse*>(receiver)) {
                os << "    storehouse #" << s->get_id() << "\n";
            }
        }
        os << "\n";
    }

    os << "\n== STOREHOUSES ==\n\n";
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE #" << it->get_id() << "\n\n";
    }
}


void generate_simulation_report(const Factory& factory, std::ostream& os, Time t) {

    os << "=== [ Turn: " << t << " ] ===\n\n";
    os << "== WORKERS ==\n\n";

    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        const Worker& w = *it;

        os << "WORKER #" << w.get_id() << "\n";

        const auto& pbuf = w.get_processing_package();
        if (pbuf.has_value()) {
            os << "  PBuffer: #"
            << pbuf->get_id()
            << " (pt = " << w.get_processing_duration() << ")\n";
        } else {
            os << "  PBuffer: (empty)\n";
}

        const auto* q = w.get_queue();
        if (q->empty()) {
            os << "  Queue: (empty)\n";
        } else {
            os << "  Queue: ";
            bool first = true;
            for (const auto& pkg : *q) {
                if (!first) os << ", ";
                os << "#" << pkg.get_id();
                first = false;
            }
            os << "\n";
        }

        auto& sbuf = const_cast<Worker&>(w).get_sending_buffer();

        if (sbuf.has_value()) {
            os << "  SBuffer: #"
               << sbuf->get_id() << "\n";
        } else {
            os << "  SBuffer: (empty)\n";
        }

        os << "\n";
    }


    os << "\n== STOREHOUSES ==\n\n";

    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        const Storehouse& s = *it;

        os << "STOREHOUSE #" << s.get_id() << "\n";

        const auto& stock = s.get_stockpile();

        if (stock.empty()) {
            os << "  Stock: (empty)\n";
        } else {
            os << "  Stock: ";
            bool first = true;
            for (const auto& pkg : stock) {
                if (!first) os << ", ";
                os << "#" << pkg.get_id();
                first = false;
            }
            os << "\n";
        }

        os << "\n";
    }
}