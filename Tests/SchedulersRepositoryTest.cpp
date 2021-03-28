#include "pch.h"

#include "../Clerk/Data/Repositories/SchedulersRepository.h"

class SchedulersRepositoryTest : public ::testing::Test {
public:
    SchedulersRepositoryTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::SchedulersRepository(*connection);
    }

    ~SchedulersRepositoryTest() {
        delete connection;
        delete repository;
    }

    void SetUp() {
        connection->Open();
    }

    void TearDown() {
        connection->Close();
    }

protected:
    Clerk::Data::DataConnection* connection;
    Clerk::Data::SchedulersRepository* repository;
};

TEST_F(SchedulersRepositoryTest, GetAll) {
    auto schedulers = repository->GetAll();

    EXPECT_EQ(schedulers.size(), 4);
}

TEST_F(SchedulersRepositoryTest, GetByIdExists) {
    auto scheduler = repository->GetById(10);

    ASSERT_TRUE(scheduler != nullptr);
    EXPECT_EQ(scheduler->id, 10);
}

TEST_F(SchedulersRepositoryTest, GetByIdNotExists) {
    auto scheduler = repository->GetById(10000);

    EXPECT_TRUE(scheduler == nullptr);
}

TEST_F(SchedulersRepositoryTest, CreateAndDelete) {
    int count = repository->GetAll().size();

    SchedulerModel* newScheduler = new SchedulerModel();

    repository->Save(*newScheduler);

    EXPECT_NE(newScheduler->id, -1);

    auto scheduler = repository->GetById(newScheduler->id);

    ASSERT_TRUE(scheduler != nullptr);

    repository->Delete(*newScheduler);

    delete newScheduler;

    EXPECT_EQ(repository->GetAll().size(), count);
}

TEST_F(SchedulersRepositoryTest, Update) {
    auto scheduler = repository->GetById(5);

    ASSERT_TRUE(scheduler != nullptr);

    int amount = rand();
    scheduler->fromAmount = amount;

    repository->Save(*scheduler);

    auto newScheduler = repository->GetById(5);

    EXPECT_EQ(newScheduler->fromAmount, amount);
}