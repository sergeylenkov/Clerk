#include "pch.h"

#include "../Clerk/Data/Repositories/GoalsRepository.h"

class GoalsRepositoryTest : public ::testing::Test {
public:
    GoalsRepositoryTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::GoalsRepository(*connection);
    }

    ~GoalsRepositoryTest() {
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
    Clerk::Data::GoalsRepository* repository;
};

TEST_F(GoalsRepositoryTest, GetAll) {
    auto goals = repository->GetAll();

    EXPECT_EQ(goals.size(), 1);
}

TEST_F(GoalsRepositoryTest, GetByIdExists) {
    auto goal = repository->GetById(1);

    EXPECT_TRUE(goal != nullptr);
    EXPECT_EQ(goal->id, 1);
}

TEST_F(GoalsRepositoryTest, GetByIdNotExists) {
    auto goal = repository->GetById(10000);

    EXPECT_TRUE(goal == nullptr);
}

TEST_F(GoalsRepositoryTest, GetBalance) {
    auto goal = repository->GetById(1);
    float balance = repository->GetBalance(goal->accountIds);

    EXPECT_EQ(static_cast<int>(balance), 184386);
}

TEST_F(GoalsRepositoryTest, CreateAndDelete) {
    int count = repository->GetAll().size();

    GoalModel* newGoal = new GoalModel();

    repository->Save(*newGoal);

    EXPECT_NE(newGoal->id, -1);

    auto goal = repository->GetById(newGoal->id);

    ASSERT_TRUE(goal != nullptr);

    repository->Delete(*newGoal);

    delete newGoal;

    EXPECT_EQ(repository->GetAll().size(), count);
}

TEST_F(GoalsRepositoryTest, Update) {
    auto goal = repository->GetById(1);

    ASSERT_TRUE(goal != nullptr);

    int amount = rand();
    goal->amount = amount;

    repository->Save(*goal);

    auto newGoal = repository->GetById(1);

    EXPECT_EQ(newGoal->amount, amount);
}