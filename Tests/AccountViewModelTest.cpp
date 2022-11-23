#include "pch.h"

#include "../Clerk/UI/ViewModels/AccountViewModel.h"
#include "../Clerk/Data/Settings.h"
#include "Fixture.cpp"

class AccountViewModelTest : public Fixture {
public:
    void SetUp() override {
        viewModel = new AccountViewModel(context->GetAccountsService(), context->GetCurrenciesService());

        viewModel->OnUpdate = ([&](AccountViewModelField field) {
            this->testId = 100;
        });
    }

    void TearDown() override {
        delete viewModel;
    }

protected:
    AccountViewModel* viewModel;
    int testId = 0;
};

TEST_F(AccountViewModelTest, IsNew) {
    bool isNew = viewModel->IsNew();

    EXPECT_EQ(isNew, true);
}

TEST_F(AccountViewModelTest, GetCurrencies) {
    auto currencies = viewModel->GetCurrencies();

    EXPECT_EQ(currencies.size(), 204);
}

TEST_F(AccountViewModelTest, GetTypes) {
    auto types = viewModel->GetTypes();

    EXPECT_EQ(types.size(), 5);
}

TEST_F(AccountViewModelTest, SetName) {
    viewModel->SetName("Test");

    EXPECT_TRUE(viewModel->GetName() == "Test");
}

TEST_F(AccountViewModelTest, SetType) {
    viewModel->SetType(AccountType::Deposit);

    EXPECT_EQ(viewModel->GetType(), AccountType::Deposit);
}

TEST_F(AccountViewModelTest, SetAmount) {
    viewModel->SetAmount(100);

    EXPECT_EQ(viewModel->GetAmount(), 100);
}

TEST_F(AccountViewModelTest, SetCreditLimit) {
    viewModel->SetCreditLimit(100);

    EXPECT_EQ(viewModel->GetCreditLimit(), 100);
}

TEST_F(AccountViewModelTest, SetCurrency) {
    auto currency = context->GetCurrenciesService().GetBaseCurrency();
    viewModel->SetCurrency(currency);

    EXPECT_EQ(viewModel->GetCurrency()->id, currency->id);
    EXPECT_EQ(viewModel->GetCurrencyIndex(), 0);
}

TEST_F(AccountViewModelTest, SetNote) {
    viewModel->SetNote("Test");

    EXPECT_TRUE(viewModel->GetNote() == "Test");
}

TEST_F(AccountViewModelTest, SetIconIndex) {
    viewModel->SetIconIndex(10);

    EXPECT_EQ(viewModel->GetIconIndex(), 10);
}

TEST_F(AccountViewModelTest, OnUpdate) {
    EXPECT_EQ(this->testId, 100);    
}