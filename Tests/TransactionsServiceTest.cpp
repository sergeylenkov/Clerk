#pragma once

#include "pch.h"

#include "../Clerk/Data/Services/TransactionsService.h"
#include "Fixture.cpp"

class TransactionsServiceTest : public Fixture {
public:
    void SetUp() override {
        Fixture::SetUp();
        
        service = new TransactionsService(context->GetTransactionsRepository(), context->GetAccountsService(), context->GetTagsService());
    }

    void TearDown() override {
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
    auto context = Fixture::context;
    auto model = context->GetAccountsRepository().GetById(2);

    AccountPresentationModel* account = new AccountPresentationModel(*model);
    account->id = 2;

    auto transactions = service->GetRecents(*account, 10);

    EXPECT_EQ(transactions.size(), 10);

    delete account;
}

TEST_F(TransactionsServiceTest, GetDeleted) {
    auto transactions = service->GetDeleted();

    EXPECT_EQ(transactions.size(), 4);
}

TEST_F(TransactionsServiceTest, Update) {
    auto transaction = service->GetById(14670);
    transaction->note = "Test";

    service->Save(*transaction);

    transaction = service->GetById(14670);

    EXPECT_TRUE(transaction->note == "Test");
}

TEST_F(TransactionsServiceTest, New) {
    auto transaction = service->GetById(14670);

    transaction->id = -1;    
    transaction->date = wxDateTime::Today();
    service->Save(*transaction);

    wxDateTime fromDate = wxDateTime::Today();    

    wxDateTime toDate = wxDateTime::Today();
    toDate.SetHour(23);

    auto transactions = service->GetRecents(1);

    EXPECT_EQ(transactions.size(), 1);

    if (transactions.size() > 0) {
        auto transaction = transactions[0];
        service->Delete(*transaction);
    }
}