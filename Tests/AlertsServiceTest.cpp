#include "pch.h"

#include "../Clerk/Data/Repositories/AlertsRepository.h"
#include "../Clerk/Data/Services/AlertsService.h"

class AlertsServiceTest : public ::testing::Test {
public:
    AlertsServiceTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::AlertsRepository(*connection);
        service = new Clerk::Data::AlertsService(*repository);
    }

    ~AlertsServiceTest() {
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
    Clerk::Data::AlertsService* service;
};

TEST_F(AlertsServiceTest, GetAll) {
    auto alerts = service->GetAll();

    EXPECT_EQ(alerts.size(), 1);
}

TEST_F(AlertsServiceTest, GetByIdExists) {
    auto alert = service->GetById(1);

    EXPECT_TRUE(alert != nullptr);
    EXPECT_EQ(alert->id, 1);
}

TEST_F(AlertsServiceTest, GetByIdNotExists) {
    auto alert = service->GetById(10000);

    EXPECT_TRUE(alert == nullptr);
}