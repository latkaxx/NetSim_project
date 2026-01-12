#include "simulation.hxx"

#include <stdexcept>

void simulate(
    Factory& factory,
    TimeOffset duration,
    std::function<void(Factory&, Time)> report_callback
) {
    //sprawdzenie spójności
    if (!factory.is_consistent()) {
        throw std::logic_error("Factory network is not consistent");
    }

    for (Time t = 1; t <= duration; ++t) {

        //dostawa półproduktów do ramp
        factory.do_deliveries(t);

        //przekazanie półproduktów
        factory.do_package_passing();

        //przetwarzanie przez robotników
        factory.do_work(t);

        //raportowanie
        report_callback(factory, t);
    }
}
