#include "pch.h"

#include "../Clerk/UI/ViewModels/StatusViewModel.h"
#include "Fixture.cpp"

class StatusViewModelTest : public Fixture {
public:
    void SetUp() override {
        Fixture::SetUp();

        std::vector<int> ratesIds{ 180 };
        viewModel = new StatusViewModel(context->GetAccountingService(), context->GetExchangeRatesRepository(), context->GetCurrenciesService(), ratesIds);
    }

    void TearDown() override {
        Fixture::TearDown();
        delete viewModel;
    }

protected:    
    StatusViewModel* viewModel;
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