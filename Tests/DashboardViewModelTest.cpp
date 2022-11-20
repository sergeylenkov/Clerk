#include "pch.h"

#include "../Clerk/UI/ViewModels/DashboardViewModel.h"
#include "Fixture.cpp"

class DashboardViewModelTest : public Fixture {
public:
    void SetUp() override {
        Fixture::SetUp();

        viewModel = new DashboardViewModel(context->GetAccountingService(), context->GetTransactionsService(), context->GetAccountsService(),
            context->GetBudgetsService(), context->GetSchedulersService(), context->GetGoalsService(), context->GetCurrenciesService());
    }

    void TearDown() override {
        Fixture::TearDown();
        delete viewModel;
    }

protected:
    DashboardViewModel* viewModel;
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

    EXPECT_EQ(currency->id, 152);
}

TEST_F(DashboardViewModelTest, GetAccounts) {
    auto accounts = viewModel->GetAccounts();

    EXPECT_EQ(accounts.size(), 5);
}

TEST_F(DashboardViewModelTest, GetBudgets) {
    auto budgets = viewModel->GetBudgets();

    EXPECT_EQ(budgets.size(), 3);
}

TEST_F(DashboardViewModelTest, GetDepts) {
    auto depts = viewModel->GetDepts();

    EXPECT_EQ(depts.size(), 1);
}

TEST_F(DashboardViewModelTest, GetGoals) {
    auto goals = viewModel->GetGoals();

    EXPECT_EQ(goals.size(), 1);
}