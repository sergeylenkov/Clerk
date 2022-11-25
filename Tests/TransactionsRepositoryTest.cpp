#include "pch.h"

#include "../Clerk/Data/Repositories/TransactionsRepository.h"
#include "Fixture.cpp"

class TransactionsRepositoryTest : public Fixture {
public:
    void SetUp() override {
        repository = new TransactionsRepository(*connection);
    }

    void TearDown() override {
        delete repository;
    }

protected:
    TransactionsRepository* repository;
};

TEST_F(TransactionsRepositoryTest, GetAll) {
    auto transactions = repository->GetAll();

    EXPECT_EQ(transactions.size(), 16206);
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

    EXPECT_EQ(transactions.size(), 1);
}

TEST_F(TransactionsRepositoryTest, GetRecents) {
    auto transactions = repository->GetRecents(10);

    EXPECT_EQ(transactions.size(), 10);
}

TEST_F(TransactionsRepositoryTest, GetRecentsForAccount) {
    auto transactions = repository->GetRecents(2, 10);

    EXPECT_EQ(transactions.size(), 10);
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

    EXPECT_EQ(transactions.size(), 75);
}

TEST_F(TransactionsRepositoryTest, GetInitialTransactionForAccount) {
    auto transaction = repository->GetInitialTransactionForAccount(70);

    EXPECT_TRUE(transaction != nullptr);
    EXPECT_EQ(transaction->id, 8287);
}

TEST_F(TransactionsRepositoryTest, CreateAndDelete) {
    int count = repository->GetAll().size();

    auto newTransaction = std::make_shared<TransactionModel>();

    auto savedTransaction = repository->Save(newTransaction);

    EXPECT_NE(savedTransaction->id, -1);
    ASSERT_TRUE(repository->GetById(savedTransaction->id) != nullptr);

    repository->Delete(savedTransaction);

    EXPECT_EQ(repository->GetAll().size(), count);
}

TEST_F(TransactionsRepositoryTest, Update) {
    auto transaction = repository->GetById(1);

    ASSERT_TRUE(transaction != nullptr);

    int amount = rand();
    transaction->fromAmount = amount;

    repository->Save(transaction);

    auto newTransaction = repository->GetById(1);

    EXPECT_EQ(newTransaction->fromAmount, amount);
}