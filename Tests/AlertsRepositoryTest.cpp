#include "pch.h"

#include "../Clerk/Data/Repositories/AlertsRepository.h"

class AlertsRepositoryTest : public ::testing::Test {
public:
    AlertsRepositoryTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::AlertsRepository(*connection);
    }

    ~AlertsRepositoryTest() {
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
    Clerk::Data::AlertsRepository* repository;
};

TEST_F(AlertsRepositoryTest, GetAll) {
    auto alerts = repository->GetAll();

    EXPECT_EQ(alerts.size(), 1);
}

TEST_F(AlertsRepositoryTest, GetByIdExists) {
    auto alert = repository->GetById(1);

    EXPECT_TRUE(alert != nullptr);
    EXPECT_EQ(alert->id, 1);
}

TEST_F(AlertsRepositoryTest, GetByIdNotExists) {
    auto alert = repository->GetById(10000);

    EXPECT_TRUE(alert == nullptr);
}

TEST_F(AlertsRepositoryTest, GetBalance) {
    auto alert = repository->GetById(1);
    float balance = repository->GetBalance(*alert);

    EXPECT_EQ(balance, 1583);
}

TEST_F(AlertsRepositoryTest, CreateAndDelete) {
    int count = repository->GetAll().size();

    AlertModel* newAlert = new AlertModel();

    repository->Save(*newAlert);

    EXPECT_NE(newAlert->id, -1);

    auto alert = repository->GetById(newAlert->id);

    ASSERT_TRUE(alert != nullptr);

    repository->Delete(*newAlert);

    delete newAlert;

    EXPECT_EQ(repository->GetAll().size(), count);
}

TEST_F(AlertsRepositoryTest, Update) {
    auto alert = repository->GetById(1);

    ASSERT_TRUE(alert != nullptr);

    int amount = rand();
    alert->amount = amount;

    repository->Save(*alert);

    auto newAlert = repository->GetById(1);

    EXPECT_EQ(newAlert->amount, amount);
}