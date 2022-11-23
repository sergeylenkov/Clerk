#include "pch.h"

#include "../Clerk/UI/PresentationModels/AccountPresentationModel.h"
#include "Fixture.cpp"

class AccountPresentationModelTest : public Fixture {
public:
    void SetUp() override {
        Fixture::SetUp();

        service = new AccountsService(context->GetAccountsRepository(), context->GetCurrenciesService());
    }

    void TearDown() override {
        delete service;
    }

protected:
    AccountsService* service;
};

TEST_F(AccountPresentationModelTest, GetById) {
    auto account = service->GetById(2);

    EXPECT_EQ(account->id, 2);
    EXPECT_EQ(account->note, "");
    EXPECT_EQ(account->type, AccountType::Deposit);
    EXPECT_EQ(account->icon, 10);
    EXPECT_EQ(account->order, 41);
    EXPECT_EQ(account->currency->id, 152);
    EXPECT_EQ(account->isActive, true);
    EXPECT_EQ(account->isCredit, false);
    EXPECT_EQ(account->date.GetYear(), 2009);
}