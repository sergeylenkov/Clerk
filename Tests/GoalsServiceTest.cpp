#include "pch.h"

#include "../Clerk/Data/Services/GoalsService.h"
#include "Environment.cpp"

class GoalsServiceTest : public ::testing::Test {
public:
    GoalsServiceTest() {
        auto context = Environment::GetInstance().GetContext();
        service = new GoalsService(context->GetGoalsRepository());
    }

    ~GoalsServiceTest() {
        delete service;
    }

protected:
    Clerk::Data::GoalsService* service;
};

TEST_F(GoalsServiceTest, GetAll) {
    auto goals = service->GetAll();

    EXPECT_EQ(goals.size(), 1);
}

TEST_F(GoalsServiceTest, GetByIdExists) {
    auto goal = service->GetById(1);

    EXPECT_TRUE(goal != nullptr);
    EXPECT_EQ(goal->id, 1);
}

TEST_F(GoalsServiceTest, GetByIdNotExists) {
    auto goal = service->GetById(10000);

    EXPECT_TRUE(goal == nullptr);
}