#include "pch.h"

#include "../Clerk/Data/Services/AccountsService.h"

class AccountsServiceTest : public ::testing::Test {
public:
    AccountsServiceTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        accountsRepository = new Clerk::Data::AccountsRepository(*connection);
        currenciesRepository = new Clerk::Data::CurrenciesRepository(*connection);

        service = new AccountsService(*accountsRepository, *currenciesRepository);        
    }

    ~AccountsServiceTest() {
        delete connection;
        delete accountsRepository;
        delete service;
    }

    void SetUp() {
        connection->Open();
    }

    void TearDown() {
        connection->Close();
    }

protected:
    Clerk::Data::DataConnection* connection;
    Clerk::Data::AccountsRepository* accountsRepository;
    Clerk::Data::CurrenciesRepository* currenciesRepository;
    Clerk::Data::AccountsService* service;
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

    EXPECT_EQ(accounts.size(), 31);
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

    EXPECT_EQ(accounts.size(), 4);
}

TEST_F(AccountsServiceTest, GetArchives) {
    auto accounts = service->GetArchive();

    EXPECT_EQ(accounts.size(), 47);
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