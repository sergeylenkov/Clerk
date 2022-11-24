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

    EXPECT_EQ(schedulers.size(), 2);
}

TEST_F(SchedulersServiceTest, GetByIdExists) {
    auto scheduler = service->GetById(11);

    ASSERT_TRUE(scheduler != nullptr);
    EXPECT_EQ(scheduler->id, 11);
}

TEST_F(SchedulersServiceTest, GetByIdNotExists) {
    auto scheduler = service->GetById(10000);

    EXPECT_TRUE(scheduler == nullptr);
}

TEST_F(SchedulersServiceTest, GetByPeriod) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(1);

    wxDateTime toDate = wxDateTime::Now();
    toDate.SetYear(2022);
    toDate.SetMonth(wxDateTime::Dec);
    toDate.SetDay(31);

    auto schedulers = service->GetByPeriod(fromDate, toDate);

    EXPECT_EQ(schedulers.size(), 1);
}
