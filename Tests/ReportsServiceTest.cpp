#include "pch.h"

#include "../Clerk/Data/Services/ReportsService.h"
#include "Environment.cpp"

class ReportsServiceTest : public ::testing::Test {
public:
    ReportsServiceTest() {
        auto context = Environment::Instance().GetContext();
        service = new Clerk::Data::ReportsService(context->GetReportsRepository());
    }

    ~ReportsServiceTest() {
        delete service;
    }

protected:
    ReportsService* service;
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