       
#include "package.hxx"         
#include "storage_types.hxx"    
#include "worker.hxx"
#include "loading_ramp.hxx"
#include "factory.hxx"
#include "report_notifier.hxx"
#include "storehouse.hxx"
#include "simulation.hxx"
#include <gtest/gtest.h>




TEST(PackageTest, IsAssignedIdLowest) {
    // przydzielanie ID o jeden większych -- utworzenie dwóch obiektów pod rząd

    Package p1;
    Package p2;

    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(PackageTest, IsIdReused) {
    // przydzielanie ID po zwolnionym obiekcie

    {
        Package p1;
    }
    Package p2;

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsMoveConstructorCorrect) {
    Package p1;
    Package p2(std::move(p1));

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsAssignmentOperatorCorrect) {
    Package p1;
    Package p2 = std::move(p1);

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageQueueTest, IsFifoCorrect) {
    PackageQueue q(PackageQueueType::FIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 1);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 2);
}

TEST(PackageQueueTest, IsLifoCorrect) {
    PackageQueue q(PackageQueueType::LIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 2);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 1);
}


TEST(FactoryTest, IsConsistentCorrect) {
    // R -> W -> S

    Factory factory;
    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(1));

    Ramp& r = *(factory.find_ramp_by_id(1));
    r.receiver_preferences().add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w = *(factory.find_worker_by_id(1));
    w.receiver_preferences().add_receiver(&(*factory.find_storehouse_by_id(1)));

    EXPECT_TRUE(factory.is_consistent());
}
/*
TEST(FactoryIOTest, ParseRamp) {
    std::istringstream iss("LOADING_RAMP id=1 delivery-interval=3");
    auto factory = load_factory_structure(iss);

    ASSERT_EQ(std::next(factory.ramp_cbegin(), 1), factory.ramp_cend());
    const auto& r = *(factory.ramp_cbegin());
    EXPECT_EQ(1, r.get_id());
    EXPECT_EQ(3, r.get_delivery_interval());
}
*/
TEST(WorkerTest, HasBuffer) {
    // Test scenariusza opisanego na stronie:
    // http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_nodes#bufor_aktualnie_przetwarzanego_polproduktu

    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    w.receive_package(Package(1));
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);
    auto& buffer = w.get_sending_buffer();

    ASSERT_TRUE(buffer.has_value());
    EXPECT_EQ(buffer.value().get_id(), 1);
}

//testy symulacji

//SpecificTurnsReportNotifier
TEST(ReportNotifierTest, SpecificTurns) {
    std::set<Time> turns{1, 3, 5};
    SpecificTurnsReportNotifier notifier(turns);

    EXPECT_TRUE(notifier.should_generate_report(1));
    EXPECT_FALSE(notifier.should_generate_report(2));
    EXPECT_TRUE(notifier.should_generate_report(3));
    EXPECT_FALSE(notifier.should_generate_report(4));
    EXPECT_TRUE(notifier.should_generate_report(5));
}

//IntervalReportNotifier
TEST(ReportNotifierTest, IntervalTurns) {
    IntervalReportNotifier notifier(2);

    EXPECT_TRUE(notifier.should_generate_report(1));
    EXPECT_FALSE(notifier.should_generate_report(2));
    EXPECT_TRUE(notifier.should_generate_report(3));
    EXPECT_FALSE(notifier.should_generate_report(4));
    EXPECT_TRUE(notifier.should_generate_report(5));
}

//jedna runda symulacji
TEST(SimulationTest, SingleRound) {
    Factory factory;

    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(
        Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO))
    );
    factory.add_storehouse(
        Storehouse(1, std::make_unique<PackageQueue>(PackageQueueType::FIFO))
    );

    Ramp& r = *factory.find_ramp_by_id(1);
    Worker& w = *factory.find_worker_by_id(1);
    Storehouse& s = *factory.find_storehouse_by_id(1);

    r.receiver_preferences().add_receiver(&w);
    w.receiver_preferences().add_receiver(&s);

    ASSERT_TRUE(factory.is_consistent());

    std::vector<Time> reported_times;
    auto report_stub = [&reported_times](Factory&, Time t) {
        reported_times.push_back(t);
    };

    simulate(factory, 1, report_stub);

    EXPECT_EQ(reported_times.size(), 1);
    EXPECT_EQ(reported_times[0], 1);
}


