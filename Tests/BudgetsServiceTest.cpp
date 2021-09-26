#include "pch.h"

#include "../Clerk/Data/Services/BudgetsService.h"
#include "Fixture.cpp"

class BudgetsServiceTest : public Fixture {
public:
    void SetUp() override {
        Fixture::SetUp();

        service = new BudgetsService(context->GetBudgetsRepository());
    }

    void TearDown() override {
        Fixture::TearDown();
        delete service;
    }

protected:
    BudgetsService* service;
};

TEST_F(BudgetsServiceTest, GetAll) {
    auto budgets = service->GetAll();

    EXPECT_EQ(budgets.size(), 3);
}

TEST_F(BudgetsServiceTest, GetByIdExists) {
    auto budget = service->GetById(1);

    ASSERT_TRUE(budget != nullptr);
    EXPECT_EQ(budget->id, 1);
}

TEST_F(BudgetsServiceTest, GetByIdNotExists) {
    auto budget = service->GetById(10000);

    EXPECT_TRUE(budget == nullptr);
}