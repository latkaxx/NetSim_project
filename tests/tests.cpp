       
#include "package.hxx"         
#include "storage_types.hxx"    
#include "worker.hxx"
#include "loading_ramp.hxx"
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

//klasa pomocnicza do testów
class DummyWorker : public Worker {
public:
    DummyWorker(ElementID id)
        : Worker(id, PackageQueueType::FIFO, 1), received(false) {}
    
    void receive_package(Package&& package) override {
        received = true;
    }
    
    bool received;
};

//czy rampa dostarcza półprodukt do pracownika co turę
TEST(LoadingRampTest, DeliversPackage) {
    LoadingRamp ramp(1, 1); 
    DummyWorker worker(1);
    ramp.add_receiver(&worker);

    ramp.deliver_package(1);
    EXPECT_TRUE(worker.received);
}

//sprawdza czy pracownik pobiera ppaczkę i przetwarza przez określoną liczbe tur
TEST(WorkerTest, PicksUpPackageAndProcesses) {
    Worker worker(1, PackageQueueType::FIFO, 2);
    Package p;
    worker.receive_package(std::move(p));

    EXPECT_FALSE(worker.is_busy());
    worker.do_work(); // bierze paczkę
    EXPECT_TRUE(worker.is_busy());

    worker.do_work(); // 2 tura
    EXPECT_FALSE(worker.is_busy()); // skończył przetwarzanie
}