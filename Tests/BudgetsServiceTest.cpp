#include "pch.h"

#include "../Clerk/Data/Services/BudgetsService.h"
#include "Environment.cpp"

class BudgetsServiceTest : public ::testing::Test {
public:
    BudgetsServiceTest() {
        auto context = Environment::GetInstance().GetContext();

        service = new BudgetsService(context->GetBudgetsRepository());
    }

    ~BudgetsServiceTest() {
        delete service;
    }

protected:
    Clerk::Data::BudgetsService* service;
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