#include "pch.h"

#include "../Clerk/Data/Repositories/AccountsRepository.h"
#include "Fixture.cpp"

class AccountsRepositoryTest : public Fixture {
public:
    void SetUp() override {
        repository = new AccountsRepository(*connection);
    }

    void TearDown() {}

protected:    
    AccountsRepository* repository;
};

TEST_F(AccountsRepositoryTest, GetAll) {
	auto accounts = repository->GetAll();

	EXPECT_EQ(accounts.size(), 82);
}

TEST_F(AccountsRepositoryTest, GetByIdExists) {
    auto account = repository->GetById(2);

    EXPECT_TRUE(account != nullptr);
    EXPECT_EQ(account->id, 2);
}

TEST_F(AccountsRepositoryTest, GetByIdNotExists) {
    auto account = repository->GetById(10000);

    EXPECT_TRUE(account == nullptr);
}

TEST_F(AccountsRepositoryTest, GetBalance) {
    auto account = repository->GetById(2);
    float amount = repository->GetBalance(account->id, account->type);

    EXPECT_EQ(static_cast<int>(amount), 3542);
}

TEST_F(AccountsRepositoryTest, GetBalanceForDate) {
    wxDateTime date = wxDateTime::Now();
    date.SetYear(2021);
    date.SetMonth(wxDateTime::Jan);
    date.SetDay(1);

    auto account = repository->GetById(2);
    float amount = repository->GetBalanceForDate(account->id, account->type, std::string(date.FormatISODate().ToUTF8()));

    EXPECT_EQ(static_cast<int>(amount), 5476528);
}

TEST_F(AccountsRepositoryTest, GetReceipts) {
    auto account = repository->GetById(3);
    float amount = repository->GetReceipts(account->id);

    EXPECT_EQ(static_cast<int>(amount), 3030285);
}

TEST_F(AccountsRepositoryTest, GetReceiptsForPeriod) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(1);

    wxDateTime toDate = wxDateTime::Now();
    toDate.SetYear(2021);
    toDate.SetMonth(wxDateTime::Jan);
    toDate.SetDay(31);

    auto account = repository->GetById(84);
    float amount = repository->GetReceipts(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));

    EXPECT_EQ(static_cast<int>(amount), 194010);
}

TEST_F(AccountsRepositoryTest, GetExpenses) {
    auto account = repository->GetById(78);
    float amount = repository->GetExpenses(account->id);

    EXPECT_EQ(static_cast<int>(amount), 1090922);
}

TEST_F(AccountsRepositoryTest, GetExpensesFroPeriod) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(1);

    wxDateTime toDate = wxDateTime::Now();
    toDate.SetYear(2021);
    toDate.SetMonth(wxDateTime::Jan);
    toDate.SetDay(31);

    auto account = repository->GetById(3);
    float amount = repository->GetExpenses(account->id, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));

    EXPECT_EQ(static_cast<int>(amount), 34928);
}

TEST_F(AccountsRepositoryTest, GetActive) {
    auto accounts = repository->GetActive();

    EXPECT_EQ(accounts.size(), 29);
}

TEST_F(AccountsRepositoryTest, GetByTypeDebt) {
    auto accounts = repository->GetByType(AccountType::Debt);

    EXPECT_EQ(accounts.size(), 1);
}

TEST_F(AccountsRepositoryTest, GetByTypeReceipt) {
    auto accounts = repository->GetByType(AccountType::Receipt);

    EXPECT_EQ(accounts.size(), 3);
}

TEST_F(AccountsRepositoryTest, GetByTypeExpens) {
    auto accounts = repository->GetByType(AccountType::Expens);

    EXPECT_EQ(accounts.size(), 22);
}

TEST_F(AccountsRepositoryTest, GetByTypeDeposit) {
    auto accounts = repository->GetByType(AccountType::Deposit);

    EXPECT_EQ(accounts.size(), 3);
}

TEST_F(AccountsRepositoryTest, GetArchives) {
    auto accounts = repository->GetArchive();

    EXPECT_EQ(accounts.size(), 53);
}

TEST_F(AccountsRepositoryTest, CreateAndDelete) {
    int count = repository->GetAll().size();

    auto newAccount = std::make_shared<AccountModel>();

    auto savedModel = repository->Save(newAccount);

    EXPECT_NE(savedModel->id, -1);

    auto account = repository->GetById(savedModel->id);

    ASSERT_TRUE(account != nullptr);

    repository->Delete(savedModel);

    EXPECT_EQ(repository->GetAll().size(), count);
}

TEST_F(AccountsRepositoryTest, Update) {
    int orderId = rand();

    auto account = repository->GetById(2);

    ASSERT_TRUE(account != nullptr);

    account->orderId = orderId;

    auto savedAccount = repository->Save(account);

    EXPECT_EQ(savedAccount->orderId, orderId);

    auto newAccount = repository->GetById(2);

    EXPECT_EQ(newAccount->orderId, orderId);
}

TEST_F(AccountsRepositoryTest, GetPairAccountId) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(31);

    int id = repository->GetPairAccountId(2, AccountType::Deposit, std::string(fromDate.FormatISODate().ToUTF8()));

    EXPECT_EQ(id, 65);
}

TEST_F(AccountsRepositoryTest, GetLastUsedAccountId) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(31);

    int id = repository->GetLastUsedAccountId(std::string(fromDate.FormatISODate().ToUTF8()));

    EXPECT_EQ(id, 80);
}