#include "pch.h"

#include "../Clerk/Data/Repositories/GoalsRepository.h"
#include "../Clerk/Data/Services/GoalsService.h"

class GoalsServiceTest : public ::testing::Test {
public:
    GoalsServiceTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::GoalsRepository(*connection);
        service = new Clerk::Data::GoalsService(*repository);
    }

    ~GoalsServiceTest() {
        delete connection;
        delete repository;
        delete service;
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