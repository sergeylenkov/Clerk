#include "pch.h"

#include "../Clerk/Data/Repositories/ReportsRepository.h"

class ReportsRepositoryTest : public ::testing::Test {
public:
    ReportsRepositoryTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::ReportsRepository(*connection);
    }

    ~ReportsRepositoryTest() {
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
    Clerk::Data::ReportsRepository* repository;
};

TEST_F(ReportsRepositoryTest, GetAll) {
    auto reports = repository->GetAll();

    EXPECT_EQ(reports.size(), 3);
}

TEST_F(ReportsRepositoryTest, GetByIdExists) {
    auto report = repository->GetById(1);

    EXPECT_TRUE(report != nullptr);
    EXPECT_EQ(report->id, 1);
}

TEST_F(ReportsRepositoryTest, GetByIdNotExists) {
    auto report = repository->GetById(10000);

    EXPECT_TRUE(report == nullptr);
}