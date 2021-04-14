#include "pch.h"

#include "../Clerk/Data/ViewModels/StatusViewModel.h"
#include "../Clerk/Data/Services/AccountingService.h"
#include "../Clerk/Data/Repositories/ExchangeRatesRepository.h"
#include "../Clerk/Data/Repositories/CurrenciesRepository.h"

class StatusViewModelTest : public ::testing::Test {
public:
    StatusViewModelTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        accountsRepository = new Clerk::Data::AccountsRepository(*connection);
        exchangeRatesRepository = new Clerk::Data::ExchangeRatesRepository(*connection);
        currenciesRepository = new Clerk::Data::CurrenciesRepository(*connection);
        currenciesRepository->SetBaseCurrency(152);

        service = new AccountingService(*accountsRepository, *exchangeRatesRepository);
        service->SetBaseCurrency(152);

        std::vector<int> ratesIds{ 180 };

        viewModel = new StatusViewModel(*service, *exchangeRatesRepository, *currenciesRepository, ratesIds);
    }

    ~StatusViewModelTest() {
        delete connection;
        delete accountsRepository;
        delete exchangeRatesRepository;
        delete currenciesRepository;
        delete service;
        delete viewModel;
    }

    void SetUp() {
        connection->Open();
    }

    void TearDown() {
        connection->Close();
    }

protected:
    Clerk::Data::DataConnection* connection;
    Clerk::Data::StatusViewModel* viewModel;
    Clerk::Data::AccountsRepository* accountsRepository;
    Clerk::Data::ExchangeRatesRepository* exchangeRatesRepository;
    Clerk::Data::CurrenciesRepository* currenciesRepository;
    Clerk::Data::AccountingService* service;
};

TEST_F(StatusViewModelTest, GetBalance) {
    float value = viewModel->GetBalance();

    EXPECT_EQ(static_cast<int>(value), 672233);
}

TEST_F(StatusViewModelTest, GetReceipts) {
    float value = viewModel->GetReceipts();

    EXPECT_EQ(static_cast<int>(value), 0);
}

TEST_F(StatusViewModelTest, GetExpenses) {
    float value = viewModel->GetExpenses();

    EXPECT_EQ(static_cast<int>(value), 0);
}

TEST_F(StatusViewModelTest, GetExchangeRates) {
    auto rates = viewModel->GetExchangeRates();

    EXPECT_TRUE(rates == wxString("75.51 USD"));
}