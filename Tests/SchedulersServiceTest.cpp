#include "pch.h"

#include "../Clerk/Data/Services/SchedulersService.h"

class SchedulersServiceTest : public ::testing::Test {
public:
    SchedulersServiceTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        schedulersRepository = new SchedulersRepository(*connection);
        accountsRepository = new Clerk::Data::AccountsRepository(*connection);
        exchangeRatesRepository = new Clerk::Data::ExchangeRatesRepository(*connection);

        service = new SchedulersService(*schedulersRepository, *accountsRepository, *exchangeRatesRepository);
    }

    ~SchedulersServiceTest() {
        delete connection;
        delete schedulersRepository;
        delete accountsRepository;
        delete exchangeRatesRepository;
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
    Clerk::Data::SchedulersRepository* schedulersRepository;
    Clerk::Data::AccountsRepository* accountsRepository;
    Clerk::Data::ExchangeRatesRepository* exchangeRatesRepository;
    Clerk::Data::SchedulersService* service;
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
