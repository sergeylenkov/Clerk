#include "pch.h"

#include "../Clerk/Data/Services/BudgetsService.h"

class BudgetsServiceTest : public ::testing::Test {
public:
    BudgetsServiceTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        budgetsRepository = new BudgetsRepository(*connection);
        service = new BudgetsService(*budgetsRepository);
    }

    ~BudgetsServiceTest() {
        delete connection;
        delete budgetsRepository;
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
    Clerk::Data::BudgetsRepository* budgetsRepository;
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