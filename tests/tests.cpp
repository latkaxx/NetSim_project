       


#include "package.hxx"         
#include "storage_types.hxx"    
#include "worker.hxx"
#include "loading_ramp.hxx"
#include "factory.hxx"
#include "report_notifier.hxx"
#include "storehouse.hxx"
#include "simulation.hxx"
#include "factory_io.hxx"
#include "reports.hxx"
#include <gtest/gtest.h>
#include <sstream>



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

TEST(FactoryIOTest, ParseRamp) {
    std::istringstream iss("LOADING_RAMP id=1 delivery-interval=3");
    auto factory = load_factory_structure(iss);

    ASSERT_EQ(std::next(factory.ramp_cbegin(), 1), factory.ramp_cend());
    const auto& r = *(factory.ramp_cbegin());
    EXPECT_EQ(1, r.get_id());
    EXPECT_EQ(3, r.get_delivery_interval());
}

//dodane testy przez norberta
TEST(FactoryIOTest, ParseWorker) {
    std::istringstream iss(
        "WORKER id=2 processing-time=5 queue-type=FIFO"
    );

    auto factory = load_factory_structure(iss);

    ASSERT_EQ(std::next(factory.worker_cbegin(), 1), factory.worker_cend());

    const auto& w = *(factory.worker_cbegin());
    EXPECT_EQ(2, w.get_id());
    EXPECT_EQ(5, w.get_processing_duration());
}
TEST(FactoryIOTest, ParseStorehouse) {
    std::istringstream iss(
        "STOREHOUSE id=7"
    );

    auto factory = load_factory_structure(iss);

    ASSERT_EQ(std::next(factory.storehouse_cbegin(), 1), factory.storehouse_cend());

    const auto& s = *(factory.storehouse_cbegin());
    EXPECT_EQ(7, s.get_id());
}
TEST(FactoryIOTest, ParseRampToWorkerLink) {
    std::istringstream iss(
        "LOADING_RAMP id=1 delivery-interval=2\n"
        "WORKER id=2 processing-time=3 queue-type=FIFO\n"
        "LINK src=ramp-1 dest=worker-2\n"
    );

    auto factory = load_factory_structure(iss);

    const auto& r = *(factory.ramp_cbegin());
    EXPECT_FALSE(r.get_receiver_preferences().get_preferences().empty());
}
TEST(FactoryIOTest, ParseFullFactory) {
    std::istringstream iss(
        "LOADING_RAMP id=1 delivery-interval=1\n"
        "WORKER id=2 processing-time=2 queue-type=FIFO\n"
        "STOREHOUSE id=3\n"
        "LINK src=ramp-1 dest=worker-2\n"
        "LINK src=worker-2 dest=store-3\n"
    );

    auto factory = load_factory_structure(iss);

    EXPECT_EQ(std::distance(factory.ramp_cbegin(), factory.ramp_cend()), 1);
    EXPECT_EQ(std::distance(factory.worker_cbegin(), factory.worker_cend()), 1);
    EXPECT_EQ(std::distance(factory.storehouse_cbegin(), factory.storehouse_cend()), 1);

    const auto& r = *(factory.ramp_cbegin());
    const auto& w = *(factory.worker_cbegin());

    EXPECT_FALSE(r.get_receiver_preferences().get_preferences().empty());
    EXPECT_FALSE(w.get_receiver_preferences().get_preferences().empty());
}
TEST(FactoryIOTest, SaveFactoryStructure) {
    Factory factory;
    factory.add_ramp(Ramp(1, 3));
    
    auto queue = std::make_unique<PackageQueue>(PackageQueueType::FIFO);
    factory.add_worker(Worker(2, 4, std::move(queue)));
    
    factory.add_storehouse(Storehouse(5));

    std::ostringstream oss;
    save_factory_structure(factory, oss);

    const std::string output = oss.str();

    EXPECT_NE(output.find("LOADING_RAMP"), std::string::npos);
    EXPECT_NE(output.find("WORKER"), std::string::npos);
    EXPECT_NE(output.find("STOREHOUSE"), std::string::npos);
}

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
TEST(StructureReportTest, GeneratesCorrectStructureReport) {

    Factory factory;

    Ramp r1(1, 1);

    Worker w1(
        1,
        1,
        std::make_unique<PackageQueue>(PackageQueueType::FIFO)
    );

    Storehouse s1(1);

    r1.receiver_preferences().add_receiver(&w1);
    w1.receiver_preferences().add_receiver(&s1);

    factory.add_ramp(std::move(r1));
    factory.add_worker(std::move(w1));
    factory.add_storehouse(std::move(s1));

    std::stringstream ss;
    generate_structure_report(factory, ss);

    std::string report = ss.str();

    EXPECT_NE(report.find("LOADING RAMP #1"), std::string::npos);
    EXPECT_NE(report.find("WORKER #1"), std::string::npos);
    EXPECT_NE(report.find("STOREHOUSE #1"), std::string::npos);
}