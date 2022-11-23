#include "pch.h"

#include "../Clerk/UI/ViewModels/TransactionViewModel.h"
#include "../Clerk/UI/PresentationModels/TagPresentationModel.h"
#include "../Clerk/Data/Settings.h"
#include "Fixture.cpp"

class TransactionViewModelTest : public Fixture {
public:
    void SetUp() override {
        viewModel = new TransactionViewModel(context->GetAccountsService(), context->GetTransactionsService(), context->GetExchangeRatesRepository(), context->GetTagsService());
    }

    void TearDown() override {
        delete viewModel;
    }

protected:    
    TransactionViewModel* viewModel;
};

TEST_F(TransactionViewModelTest, GetFromAccounts) {
    auto accounts = viewModel->GetFromAccounts();

    EXPECT_EQ(accounts.size(), 7);
}

TEST_F(TransactionViewModelTest, GetToAccounts) {
    auto accounts = viewModel->GetToAccounts();

    EXPECT_EQ(accounts.size(), 5);
}

TEST_F(TransactionViewModelTest, SetFromAmount) {
    viewModel->SetFromAmount(10);
    float value = viewModel->GetFromAmount();

    EXPECT_EQ(static_cast<int>(value), 10);
}

TEST_F(TransactionViewModelTest, SetToAmount) {
    viewModel->SetToAmount(10);
    float value = viewModel->GetToAmount();

    EXPECT_EQ(static_cast<int>(value), 10);
}

TEST_F(TransactionViewModelTest, AddTag) {
    auto tag = std::make_shared<TagPresentationModel>();
    tag->name = "Test";

    viewModel->AddTag(tag);
    auto tags = viewModel->GetTags();
    wxString str = viewModel->GetTagsString();

    EXPECT_EQ(tags.size(), 1);
    EXPECT_TRUE(str == "Test");
}

TEST_F(TransactionViewModelTest, SetTagsString) {
    viewModel->SetTagsString("Tag 1, Tag 2, ");

    auto tags = viewModel->GetTags();
    wxString str = viewModel->GetTagsString();

    EXPECT_EQ(tags.size(), 2);
    EXPECT_TRUE(str == "Tag 1, Tag 2");
}