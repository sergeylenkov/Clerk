#include "pch.h"

#include "../Clerk/Data/Services/AccountingService.h"

class AccountingServiceTest : public ::testing::Test {
public:
    AccountingServiceTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        accountsRepository = new Clerk::Data::AccountsRepository(*connection);        
        exchangeRatesRepository = new Clerk::Data::ExchangeRatesRepository(*connection);

        service = new AccountingService(*accountsRepository, *exchangeRatesRepository);
        service->SetBaseCurrency(152);
    }

    ~AccountingServiceTest() {
        delete connection;
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
    Clerk::Data::AccountsRepository* accountsRepository;
    Clerk::Data::ExchangeRatesRepository* exchangeRatesRepository;
    Clerk::Data::AccountingService* service;
};

TEST_F(AccountingServiceTest, GetBalance) {
    float amount = service->GetBalance();

    EXPECT_EQ(static_cast<int>(amount), 672233);
}

TEST_F(AccountingServiceTest, GetReceipts) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(1);

    wxDateTime toDate = wxDateTime::Now();
    toDate.SetYear(2021);
    toDate.SetMonth(wxDateTime::Jan);
    toDate.SetDay(31);

    float amount = service->GetReceipts(fromDate, toDate);

    EXPECT_EQ(static_cast<int>(amount), 194509);
}

TEST_F(AccountingServiceTest, GetExpenses) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(1);

    wxDateTime toDate = wxDateTime::Now();
    toDate.SetYear(2021);
    toDate.SetMonth(wxDateTime::Jan);
    toDate.SetDay(31);

    float amount = service->GetExpenses(fromDate, toDate);

    EXPECT_EQ(static_cast<int>(amount), 161789);
}

TEST_F(AccountingServiceTest, GetCredit) {
    float amount = service->GetCredit();

    EXPECT_EQ(static_cast<int>(amount), 544000);
}
