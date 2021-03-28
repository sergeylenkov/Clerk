#include "pch.h"

#include "../Clerk/Data/Repositories/ReportsRepository.h"
#include "../Clerk/Data/Services/ReportsService.h"

class ReportsServiceTest : public ::testing::Test {
public:
    ReportsServiceTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::ReportsRepository(*connection);
        service = new Clerk::Data::ReportsService(*repository);
    }

    ~ReportsServiceTest() {
        delete connection;
        delete repository;
        delete service;
    }

    void SetUp() {
        connection->Open();
    }

    void TearDown() {
        connection->Close();
    }

protected:
    Clerk::Data::DataConnection* connection;
    Clerk::Data::ReportsRepository* repository;
    Clerk::Data::ReportsService* service;
};

TEST_F(ReportsServiceTest, GetAll) {
    auto reports = service->GetAll();

    EXPECT_EQ(reports.size(), 3);
}

TEST_F(ReportsServiceTest, GetByIdExists) {
    auto report = service->GetById(1);

    EXPECT_TRUE(report != nullptr);
    EXPECT_EQ(report->id, 1);
}

TEST_F(ReportsServiceTest, GetByIdNotExists) {
    auto report = service->GetById(10000);

    EXPECT_TRUE(report == nullptr);
}