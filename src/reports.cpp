#include "reports.hxx"

void generate_structure_report(Factory& factory, std::ostream& os) {

    os << "== LOADING RAMPS ==\n\n";
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING RAMP #" << it->get_id() << "\n";
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

    os << "== WORKERS ==\n\n";
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        os << "WORKER #" << it->get_id() << "\n";
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

    os << "== STOREHOUSES ==\n\n";
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE #" << it->get_id() << "\n\n";
    }
}
