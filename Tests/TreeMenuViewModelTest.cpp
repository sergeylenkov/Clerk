#include "pch.h"

#include "../Clerk/Data/ViewModels/TreeMenuViewModel.h"
#include "../Clerk/Data/Repositories/TransactionsRepository.h"
#include "../Clerk/Data/Repositories/AccountsRepository.h"
#include "../Clerk/Data/Repositories/CurrenciesRepository.h"
#include "../Clerk/Data/Services/AccountsService.h"
#include "../Clerk/Data/Services/ReportingService.h"
#include "../Clerk/Data/Services/TransactionsService.h"
#include "../Clerk/Data/Services/TagsService.h"

class TreeMenuViewModelTest : public ::testing::Test {
public:
    TreeMenuViewModelTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        accountsRepository = new Clerk::Data::AccountsRepository(*connection);
        reportsRepository = new  Clerk::Data::ReportsRepository(*connection);
        currenciesRepository = new Clerk::Data::CurrenciesRepository(*connection);
        transactionsRepository = new Clerk::Data::TransactionsRepository(*connection);
        tagsRepository = new Clerk::Data::TagsRepository(*connection);

        accountsService = new Clerk::Data::AccountsService(*accountsRepository, *currenciesRepository);
        reportsService = new Clerk::Data::ReportsService(*reportsRepository);
        tagsService = new Clerk::Data::TagsService(*tagsRepository);
        transactionsService = new Clerk::Data::TransactionsService(*transactionsRepository, *accountsService, *tagsService);

        viewModel = new Clerk::Data::TreeMenuViewModel(*accountsService, *reportsService, *transactionsService);
    }

    ~TreeMenuViewModelTest() {
        delete connection;
        delete accountsRepository;
        delete reportsRepository;
        delete transactionsRepository;        
        delete currenciesRepository;
        delete tagsRepository;
        delete accountsService;
        delete reportsService;
        delete transactionsService;
        delete tagsService;
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
    Clerk::Data::TreeMenuViewModel* viewModel;
    Clerk::Data::AccountsRepository* accountsRepository;
    Clerk::Data::ReportsRepository* reportsRepository;
    Clerk::Data::TransactionsRepository* transactionsRepository;
    Clerk::Data::CurrenciesRepository* currenciesRepository;
    Clerk::Data::TagsRepository* tagsRepository;
    Clerk::Data::AccountsService* accountsService;
    Clerk::Data::ReportsService *reportsService;
    Clerk::Data::TransactionsService *transactionsService;
    Clerk::Data::TagsService* tagsService;
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
    AccountViewModel* account = new AccountViewModel();
    account->id = 69;

    auto transactions = viewModel->GetRecentsTransactions(*account);

    EXPECT_EQ(transactions.size(), 10);
}