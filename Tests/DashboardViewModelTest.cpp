#include "pch.h"

#include "../Clerk/Data/ViewModels/DashboardViewModel.h"
#include "Environment.cpp"

class DashboardViewModelTest : public ::testing::Test {
public:
    DashboardViewModelTest() {
        auto context = Environment::Instance().GetContext();

        viewModel = new DashboardViewModel(context->GetAccountingService(), context->GetTransactionsService(), context->GetAccountsService(),
            context->GetBudgetsService(), context->GetSchedulersService(), context->GetGoalsService(), *context->GetCurrenciesRepository().GetBaseCurrency());
    }

    ~DashboardViewModelTest() {
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

    EXPECT_EQ(currency.id, 152);
}

TEST_F(DashboardViewModelTest, GetAccounts) {
    auto accounts = viewModel->GetAccounts();

    EXPECT_EQ(accounts.size(), 10);
}

TEST_F(DashboardViewModelTest, GetBudgets) {
    auto budgets = viewModel->GetBudgets();

    EXPECT_EQ(budgets.size(), 2);
}

TEST_F(DashboardViewModelTest, GetDepts) {
    auto depts = viewModel->GetDepts();

    EXPECT_EQ(depts.size(), 2);
}

TEST_F(DashboardViewModelTest, GetGoals) {
    auto goals = viewModel->GetGoals();

    EXPECT_EQ(goals.size(), 0);
}