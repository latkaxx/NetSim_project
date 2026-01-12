#include "factory_io.hxx"

#include <sstream>
#include <string>
#include <map>
#include <stdexcept>

static std::map<std::string, std::string> parse_params(const std::string& line) {
    std::map<std::string, std::string> params;
    std::istringstream iss(line);
    std::string token;
    iss >> token;
    while (iss >> token) {
        auto pos = token.find('=');
        params[token.substr(0, pos)] = token.substr(pos + 1);
    }
    return params;
}
static std::pair<std::string, ElementID> parse_node_id(const std::string& s) {
    auto pos = s.find('-');
    return {
        s.substr(0, pos),
        static_cast<ElementID>(std::stoul(s.substr(pos + 1)))
    };
}
Factory load_factory_structure(std::istream& is) {
    Factory factory;
    std::string line;

    struct Link { std::string src, dest; };
    std::vector<Link> links;

    while (std::getline(is, line)) {
        if (line.empty()) continue;

        if (line.find("LOADING_RAMP")==0) {
            auto p = parse_params(line);
            factory.add_ramp(Ramp(std::stoi(p["id"]), std::stoi(p["delivery-interval"])));
        }
        else if (line.find("WORKER")==0) {
            auto p = parse_params(line);
            auto queue = std::make_unique<PackageQueue>(p["queue-type"] == "FIFO" ? PackageQueueType::FIFO: PackageQueueType::LIFO);

            factory.add_worker(
                Worker(
                    std::stoi(p["id"]),
                    std::stoi(p["processing-time"]),
                    std::move(queue)
                )
            );
        }
        else if (line.find("STOREHOUSE")==0) {
            auto p = parse_params(line);
            factory.add_storehouse(Storehouse(std::stoi(p["id"])));
        }
        else if (line.find("LINK")==0) {
            auto p = parse_params(line);
            links.push_back(Link{p["src"], p["dest"]});
        }
    }

    // linkowanie
    for (const auto& l : links) {
        auto [src_type, src_id] = parse_node_id(l.src);
        auto [dst_type, dst_id] = parse_node_id(l.dest);

        IPackageReceiver* receiver =
            dst_type == "worker"
                ? static_cast<IPackageReceiver*>(&*factory.find_worker_by_id(dst_id))
                : static_cast<IPackageReceiver*>(&*factory.find_storehouse_by_id(dst_id));

        if (src_type == "ramp") {
            factory.find_ramp_by_id(src_id)
                ->get_receiver_preferences()
                .add_receiver(receiver);
        }
        else {
            factory.find_worker_by_id(src_id)
                ->get_receiver_preferences()
                .add_receiver(receiver);
        }
    }

    return factory;
}


void save_factory_structure(const Factory& factory, std::ostream& os) {

    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING_RAMP id=" << it->get_id()
           << " delivery-interval=" << it->get_delivery_interval() << "\n";
    }

    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        os << "WORKER id=" << it->get_id()
           << " processing-time=" << it->get_processing_duration()
           << " queue-type=" << (it->get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO")
           << "\n";
    }

    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE id=" << it->get_id() << "\n";
    }
}
