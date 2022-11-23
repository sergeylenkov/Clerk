#include "pch.h"

#include "../Clerk/Data/Services/AccountsService.h"
#include "Fixture.cpp"

class AccountsServiceTest : public Fixture {
public:
    void SetUp() override {
        service = new AccountsService(context->GetAccountsRepository(), context->GetCurrenciesService());
    }

    void TearDown() override {
        delete service;
    }

protected:    
    AccountsService* service;
};

TEST_F(AccountsServiceTest, GetByIdExists) {
    auto account = service->GetById(2);

    ASSERT_TRUE(account != nullptr);
    EXPECT_EQ(account->id, 2);
}

TEST_F(AccountsServiceTest, GetByIdNotExists) {
    auto account = service->GetById(10000);

    EXPECT_TRUE(account == nullptr);
}

TEST_F(AccountsServiceTest, GetActive) {
    auto accounts = service->GetActive();

    EXPECT_EQ(accounts.size(), 33);
}

TEST_F(AccountsServiceTest, GetByTypeDebt) {
    auto accounts = service->GetByType(AccountType::Debt);

    EXPECT_EQ(accounts.size(), 1);
}

TEST_F(AccountsServiceTest, GetByTypeReceipt) {
    auto accounts = service->GetByType(AccountType::Receipt);

    EXPECT_EQ(accounts.size(), 3);
}

TEST_F(AccountsServiceTest, GetByTypeExpens) {
    auto accounts = service->GetByType(AccountType::Expens);

    EXPECT_EQ(accounts.size(), 22);
}

TEST_F(AccountsServiceTest, GetByTypeDeposit) {
    auto accounts = service->GetByType(AccountType::Deposit);

    EXPECT_EQ(accounts.size(), 7);
}

TEST_F(AccountsServiceTest, GetArchives) {
    auto accounts = service->GetArchive();

    EXPECT_EQ(accounts.size(), 53);
}

TEST_F(AccountsServiceTest, GetDebts) {
    auto accounts = service->GetDebts();

    EXPECT_EQ(accounts.size(), 1);
}

TEST_F(AccountsServiceTest, GetPairAccount) {
    auto account = service->GetById(2);
    auto pair = service->GetPairAccount(*account);

    ASSERT_TRUE(pair != nullptr);
    EXPECT_EQ(pair->id, 65);
}

TEST_F(AccountsServiceTest, GetLastUsedAccount) {
    auto account = service->GetLastUsedAccount();

    ASSERT_TRUE(account != nullptr);
}

TEST_F(AccountsServiceTest, Save) {
    auto account = service->GetById(2);

    account->id = -1;
    account->type = AccountType::Deposit;

    service->Save(*account);

    auto accounts = service->GetByType(AccountType::Deposit);

    EXPECT_EQ(accounts.size(), 7);

    if (accounts.size() > 0) {
        auto account = accounts.back();
        service->Delete(*account);
    }
}