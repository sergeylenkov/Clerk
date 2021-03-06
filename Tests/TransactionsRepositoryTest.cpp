#include "pch.h"

#include "../Clerk/Data/Repositories/TransactionsRepository.h"

class TransactionsRepositoryTest : public ::testing::Test {
public:
    TransactionsRepositoryTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::TransactionsRepository(*connection);
    }

    ~TransactionsRepositoryTest() {
        delete connection;
        delete repository;
    }

    void SetUp() {
        connection->Open();
    }

    void TearDown() {
        connection->Close();
    }

protected:
    Clerk::Data::DataConnection* connection;
    Clerk::Data::TransactionsRepository* repository;
};

TEST_F(TransactionsRepositoryTest, GetAll) {
    auto transactions = repository->GetAll();

    EXPECT_EQ(transactions.size(), 14387);
}

TEST_F(TransactionsRepositoryTest, GetByIdExists) {
    auto transaction = repository->GetById(1);

    EXPECT_TRUE(transaction != nullptr);
    EXPECT_EQ(transaction->id, 1);
}

TEST_F(TransactionsRepositoryTest, GetByIdNotExists) {
    auto transaction = repository->GetById(100000);

    EXPECT_TRUE(transaction == nullptr);
}

TEST_F(TransactionsRepositoryTest, GetDeleted) {
    auto transactions = repository->GetDeleted();

    EXPECT_EQ(transactions.size(), 4);
}

TEST_F(TransactionsRepositoryTest, GetRecents) {
    auto transactions = repository->GetRecents(10);

    EXPECT_EQ(transactions.size(), 10);
}

TEST_F(TransactionsRepositoryTest, GetRecentsForAccount) {
    AccountModel* account = new AccountModel();
    account->id = 2;

    auto transactions = repository->GetRecents(account->id, 10);

    EXPECT_EQ(transactions.size(), 10);

    delete account;
}

TEST_F(TransactionsRepositoryTest, GetForPeriod) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(1);

    wxDateTime toDate = wxDateTime::Now();
    toDate.SetYear(2021);
    toDate.SetMonth(wxDateTime::Jan);
    toDate.SetDay(31);

    auto transactions = repository->GetForPeriod(std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));

    EXPECT_EQ(transactions.size(), 90);
}

TEST_F(TransactionsRepositoryTest, GetInitialTransactionForAccount) {
    auto transaction = repository->GetInitialTransactionForAccount(70);

    EXPECT_TRUE(transaction != nullptr);
    EXPECT_EQ(transaction->id, 8287);
}

TEST_F(TransactionsRepositoryTest, CreateAndDelete) {
    int count = repository->GetAll().size();

    TransactionModel* newTransaction = new TransactionModel();

    repository->Save(*newTransaction);

    EXPECT_NE(newTransaction->id, -1);

    auto transaction = repository->GetById(newTransaction->id);

    ASSERT_TRUE(transaction != nullptr);

    repository->Delete(*newTransaction);

    delete newTransaction;

    EXPECT_EQ(repository->GetAll().size(), count);
}

TEST_F(TransactionsRepositoryTest, Update) {
    auto transaction = repository->GetById(1);

    ASSERT_TRUE(transaction != nullptr);

    int amount = rand();
    transaction->fromAmount = amount;

    repository->Save(*transaction);

    auto newTransaction = repository->GetById(1);

    EXPECT_EQ(newTransaction->fromAmount, amount);
}