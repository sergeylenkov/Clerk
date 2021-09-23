#include "pch.h"

#include "../Clerk/Data/ViewModels/TransactionEditViewModel.h"
#include "../Clerk/Data/ViewModels/TagViewModel.h"
#include "../Clerk/Data/Services/AccountsService.h"
#include "../Clerk/Data/Repositories/ExchangeRatesRepository.h"
#include "../Clerk/Data//Settings.h"

class TransactionEditViewModelTest : public ::testing::Test {
public:
    TransactionEditViewModelTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        accountsRepository = new Clerk::Data::AccountsRepository(*connection);
        exchangeRatesRepository = new Clerk::Data::ExchangeRatesRepository(*connection);
        currenciesRepository = new Clerk::Data::CurrenciesRepository(*connection);

        service = new AccountsService(*accountsRepository, *currenciesRepository);

        viewModel = new TransactionEditViewModel(*service, *exchangeRatesRepository);

        Settings::GetInstance().SetConvertCurrency(true);
    }

    ~TransactionEditViewModelTest() {
        delete connection;
        delete accountsRepository;
        delete exchangeRatesRepository;
        delete service;
        delete viewModel;
        delete currenciesRepository;
    }

    void SetUp() {
        connection->Open();
    }

    void TearDown() {
        connection->Close();
    }

protected:
    Clerk::Data::DataConnection* connection;
    Clerk::Data::TransactionEditViewModel* viewModel;
    Clerk::Data::AccountsRepository* accountsRepository;
    Clerk::Data::ExchangeRatesRepository* exchangeRatesRepository;
    Clerk::Data::AccountsService* service;
    Clerk::Data::CurrenciesRepository* currenciesRepository;
};

TEST_F(TransactionEditViewModelTest, GetFromAccounts) {
    auto accounts = viewModel->GetFromAccounts();

    EXPECT_EQ(accounts.size(), 7);
}

TEST_F(TransactionEditViewModelTest, GetToAccounts) {
    auto accounts = viewModel->GetToAccounts();

    EXPECT_EQ(accounts.size(), 5);
}

TEST_F(TransactionEditViewModelTest, SetFromAmount) {
    viewModel->SetFromAmount(10);
    float value = viewModel->GetFromAmount();

    EXPECT_EQ(static_cast<int>(value), 10);
}

TEST_F(TransactionEditViewModelTest, SetToAmount) {
    viewModel->SetToAmount(10);
    float value = viewModel->GetToAmount();

    EXPECT_EQ(static_cast<int>(value), 10);
}

TEST_F(TransactionEditViewModelTest, AddTag) {
    auto tag = std::make_shared<TagViewModel>();
    tag->name = "Test";

    viewModel->AddTag(tag);
    auto tags = viewModel->GetTags();
    wxString str = viewModel->GetTagsString();

    EXPECT_EQ(tags.size(), 1);
    EXPECT_TRUE(str == "Test");
}