#include "pch.h"

#include "../Clerk/UI/ViewModels/TreeMenuViewModel.h"
#include "Fixture.cpp"

class TreeMenuViewModelTest : public Fixture {
public:
    void SetUp() override {
        viewModel = new TreeMenuViewModel(context->GetAccountsService(), context->GetReportsService(), context->GetTransactionsService());
    }

    void TearDown() override {
        delete viewModel;
    }

protected:
    TreeMenuViewModel* viewModel;
};

TEST_F(TreeMenuViewModelTest, GetReceiptsAccounts) {
    auto accounts = viewModel->GetReceiptsAccounts();

    EXPECT_EQ(accounts.size(), 3);
}

TEST_F(TreeMenuViewModelTest, GetDepositsAccounts) {
    auto accounts = viewModel->GetDepositsAccounts();

    EXPECT_EQ(accounts.size(), 4);
}

TEST_F(TreeMenuViewModelTest, GetExpensesAccounts) {
    auto accounts = viewModel->GetExpensesAccounts();

    EXPECT_EQ(accounts.size(), 22);
}

TEST_F(TreeMenuViewModelTest, GetDebtsAccounts) {
    auto accounts = viewModel->GetDebtsAccounts();

    EXPECT_EQ(accounts.size(), 1);
}

TEST_F(TreeMenuViewModelTest, GetVirtualsAccounts) {
    auto accounts = viewModel->GetVirtualsAccounts();

    EXPECT_EQ(accounts.size(), 1);
}

TEST_F(TreeMenuViewModelTest, GetArchiveAccounts) {
    auto accounts = viewModel->GetArchiveAccounts();

    EXPECT_EQ(accounts.size(), 47);
}

TEST_F(TreeMenuViewModelTest, GetReports) {
    auto reports = viewModel->GetReports();

    EXPECT_EQ(reports.size(), 3);
}

TEST_F(TreeMenuViewModelTest, GetRecentsTransactions) {
    AccountPresentationModel* account = new AccountPresentationModel();
    account->id = 69;

    auto transactions = viewModel->GetRecentsTransactions(*account);

    EXPECT_EQ(transactions.size(), 10);
}