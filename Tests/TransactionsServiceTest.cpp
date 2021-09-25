#include "pch.h"

#include "../Clerk/Data/Services/TransactionsService.h"
#include "Environment.cpp"

class TransactionsServiceTest : public ::testing::Test {
public:
    TransactionsServiceTest() {
        auto context = Environment::Instance().GetContext();

        service = new TransactionsService(context->GetTransactionsRepository(), context->GetAccountsService(), context->GetTagsService());
    }

    ~TransactionsServiceTest() {
        delete service;
    }

protected:
    TransactionsService* service;
};

TEST_F(TransactionsServiceTest, GetForPeriod) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(1);

    wxDateTime toDate = wxDateTime::Now();
    toDate.SetYear(2021);
    toDate.SetMonth(wxDateTime::Jan);
    toDate.SetDay(31);

    auto transactions = service->GetForPeriod(fromDate, toDate);

    EXPECT_EQ(transactions.size(), 90);
}

TEST_F(TransactionsServiceTest, GetByIdExists) {
    auto transaction = service->GetById(14670);

    EXPECT_TRUE(transaction != nullptr);
    EXPECT_EQ(transaction->id, 14670);
}

TEST_F(TransactionsServiceTest, GetByIdNotExists) {
    auto transaction = service->GetById(100000);

    EXPECT_TRUE(transaction == nullptr);
}

TEST_F(TransactionsServiceTest, GetRecents) {
    auto transactions = service->GetRecents(10);

    EXPECT_EQ(transactions.size(), 10);
}

TEST_F(TransactionsServiceTest, GetRecentsForAccount) {
    auto context = Environment::Instance().GetContext();
    auto model = context->GetAccountsRepository().GetById(2);

    AccountViewModel* account = new AccountViewModel(*model);
    account->id = 2;

    auto transactions = service->GetRecents(*account, 10);

    EXPECT_EQ(transactions.size(), 10);

    delete account;
}

TEST_F(TransactionsServiceTest, GetDeleted) {
    auto transactions = service->GetDeleted();

    EXPECT_EQ(transactions.size(), 4);
}