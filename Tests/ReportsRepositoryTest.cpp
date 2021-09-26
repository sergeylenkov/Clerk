#include "pch.h"

#include "../Clerk/Data/Repositories/ReportsRepository.h"
#include "Fixture.cpp"

class ReportsRepositoryTest : public Fixture {
public:
    void SetUp() override {
        SetUpConnection();
        repository = new Clerk::Data::ReportsRepository(*connection);
    }

    void TearDown() override {
        Fixture::TearDown();
    }

protected:
    ReportsRepository* repository;
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