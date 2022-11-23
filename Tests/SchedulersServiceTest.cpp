#include "pch.h"

#include "../Clerk/Data/Services/SchedulersService.h"
#include "Fixture.cpp"

class SchedulersServiceTest : public Fixture {
public:
    void SetUp() override {
        service = new SchedulersService(context->GetSchedulersRepository(), context->GetAccountsRepository(), context->GetExchangeRatesRepository());
    }

    void TearDown() override {
        delete service;
    }

protected:
    SchedulersService* service;
};

TEST_F(SchedulersServiceTest, GetAll) {
    auto schedulers = service->GetAll();

    EXPECT_EQ(schedulers.size(), 4);
}

TEST_F(SchedulersServiceTest, GetByIdExists) {
    auto scheduler = service->GetById(10);

    ASSERT_TRUE(scheduler != nullptr);
    EXPECT_EQ(scheduler->id, 10);
}

TEST_F(SchedulersServiceTest, GetByIdNotExists) {
    auto scheduler = service->GetById(10000);

    EXPECT_TRUE(scheduler == nullptr);
}

TEST_F(SchedulersServiceTest, GetByPeriod) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Feb);
    fromDate.SetDay(1);

    wxDateTime toDate = wxDateTime::Now();
    toDate.SetYear(2021);
    toDate.SetMonth(wxDateTime::Mar);
    toDate.SetDay(31);

    auto schedulers = service->GetByPeriod(fromDate, toDate);

    EXPECT_EQ(schedulers.size(), 3);
}
