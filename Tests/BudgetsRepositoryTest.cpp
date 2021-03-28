#include "pch.h"

#include "../Clerk/Data/Repositories/BudgetsRepository.h"

class BudgetsRepositoryTest : public ::testing::Test {
public:
    BudgetsRepositoryTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::BudgetsRepository(*connection);
    }

    ~BudgetsRepositoryTest() {
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
    Clerk::Data::BudgetsRepository* repository;
};

TEST_F(BudgetsRepositoryTest, GetAll) {
    auto budgets = repository->GetAll();

    EXPECT_EQ(budgets.size(), 3);
}

TEST_F(BudgetsRepositoryTest, GetByIdExists) {
    auto budget = repository->GetById(1);

    ASSERT_TRUE(budget != nullptr);
    EXPECT_EQ(budget->id, 1);
}

TEST_F(BudgetsRepositoryTest, GetByIdNotExists) {
    auto budget = repository->GetById(10000);

    EXPECT_TRUE(budget == nullptr);
}

TEST_F(BudgetsRepositoryTest, GetExpenses) {
    auto budget = repository->GetById(1);
    float amount = 0;

    if (budget) {
        wxDateTime toDate = wxDateTime::Now();
        wxDateTime fromDate = wxDateTime::Now();

        fromDate.SetYear(2000);
        fromDate.SetMonth(wxDateTime::Month::Jan);
        fromDate.SetDay(1);

        amount = repository->GetExpenses(*budget, std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));
    }

    EXPECT_EQ(amount, 11534217.0);
}

TEST_F(BudgetsRepositoryTest, CreateAndDelete) {
    int count = repository->GetAll().size();

    BudgetModel* newBudget = new BudgetModel();
    
    repository->Save(*newBudget);

    EXPECT_NE(newBudget->id, -1);

    auto budget = repository->GetById(newBudget->id);

    ASSERT_TRUE(budget != nullptr);

    repository->Delete(*newBudget);

    delete newBudget;

    EXPECT_EQ(repository->GetAll().size(), count);
}

TEST_F(BudgetsRepositoryTest, Update) {
    auto budget = repository->GetById(1);

    ASSERT_TRUE(budget != nullptr);

    int amount = rand();
    budget->amount = amount;

    repository->Save(*budget);

    auto newBudget = repository->GetById(1);

    EXPECT_EQ(newBudget->amount, amount);
}