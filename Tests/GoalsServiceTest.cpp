#include "pch.h"

#include "../Clerk/Data/Services/GoalsService.h"
#include "Fixture.cpp"

class GoalsServiceTest : public Fixture {
public:
    void SetUp() override {
        service = new GoalsService(context->GetGoalsRepository());
    }

    void TearDown() override {
        delete service;
    }

protected:
    GoalsService* service;
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