#include "pch.h"

#include "../Clerk/Data/ViewModels/DashboardViewModel.h"
#include "../Clerk/Data/Services/AccountingService.h"
#include "../Clerk/Data/Services/TransactionsService.h"
#include "../Clerk/Data/Repositories/ExchangeRatesRepository.h"

class DashboardViewModelTest : public ::testing::Test {
public:
    DashboardViewModelTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        accountsRepository = new Clerk::Data::AccountsRepository(*connection);
        exchangeRatesRepository = new Clerk::Data::ExchangeRatesRepository(*connection);       
        transactionsRepository = new Clerk::Data::TransactionsRepository(*connection);
        tagsRepository = new Clerk::Data::TagsRepository(*connection);

        accountsService = new Clerk::Data::AccountingService(*accountsRepository, *exchangeRatesRepository);
        accountsService->SetBaseCurrency(152);

        tagsService = new  Clerk::Data::TagsService(*tagsRepository);
        transactionsService = new Clerk::Data::TransactionsService(*transactionsRepository, *accountsService, *tagsService);

        currency = new Currency();
        currency->id = 152;

        viewModel = new DashboardViewModel(*accountsService, *transactionsService , *currency);
    }

    ~DashboardViewModelTest() {
        delete connection;
        delete accountsRepository;
        delete exchangeRatesRepository;
        delete transactionsRepository;
        delete accountsService;
        delete transactionsService;
        delete tagsRepository;
        delete viewModel;
        delete currency;
    }

    void SetUp() {
        connection->Open();
    }

    void TearDown() {
        connection->Close();
    }

protected:
    Clerk::Data::DataConnection* connection;
    Clerk::Data::DashboardViewModel* viewModel;
    Clerk::Data::AccountsRepository* accountsRepository;
    Clerk::Data::ExchangeRatesRepository* exchangeRatesRepository;
    Clerk::Data::TransactionsRepository* transactionsRepository;
    Clerk::Data::TagsRepository* tagsRepository;
    Clerk::Data::AccountingService* accountsService;
    Clerk::Data::TransactionsService* transactionsService;
    Clerk::Data::TagsService* tagsService;
    Clerk::Data::Currency* currency;
};

TEST_F(DashboardViewModelTest, GetTotalFunds) {
    float value = viewModel->GetTotalFunds();

    EXPECT_EQ(static_cast<int>(value), 1216233);
}

TEST_F(DashboardViewModelTest, GetOwnFunds) {
    float value = viewModel->GetOwnFunds();

    EXPECT_EQ(static_cast<int>(value), 672233);
}

TEST_F(DashboardViewModelTest, GetCreditFunds) {
    float value = viewModel->GetCreditFunds();

    EXPECT_EQ(static_cast<int>(value), 544000);
}

TEST_F(DashboardViewModelTest, GetCurrency) {
    auto currency = viewModel->GetCurrency();

    EXPECT_EQ(currency.id, 152);
}