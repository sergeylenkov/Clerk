#include "pch.h"

#include "../Clerk/UI/PresentationModels/TransactionPresentationModel.h"
#include "Fixture.cpp"

class TransactionPresentationModelTest : public Fixture {
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

TEST_F(TransactionPresentationModelTest, GetById) {
    auto transaction = service->GetById(16500);
    
    EXPECT_EQ(transaction->id, 16500);
    EXPECT_EQ(transaction->fromAmount, 2700);
    EXPECT_EQ(transaction->toAmount, 2700);
    EXPECT_EQ(transaction->tags.size(), 1);
    EXPECT_EQ(transaction->fromAccount->id, 90);
    EXPECT_EQ(transaction->toAccount->id, 18);
    EXPECT_EQ(transaction->date.GetYear(), 2022);
    EXPECT_EQ(transaction->created.GetYear(), 2022);
}