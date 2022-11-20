#include "pch.h"

#include "../Clerk/UI/ViewModels/TransactionEditViewModel.h"
#include "../Clerk/UI/PresentationModels/TagPresentationModel.h"
#include "../Clerk/Data/Settings.h"
#include "Fixture.cpp"

class TransactionEditViewModelTest : public Fixture {
public:
    void SetUp() override {
        Fixture::SetUp();

        viewModel = new TransactionEditViewModel(context->GetAccountsService(), context->GetTransactionsService(), context->GetExchangeRatesRepository(), context->GetTagsService());
    }

    void TearDown() override {
        Fixture::TearDown();
        delete viewModel;
    }

protected:    
    TransactionEditViewModel* viewModel;
};

TEST_F(TransactionEditViewModelTest, GetFromAccounts) {
    auto accounts = viewModel->GetFromAccounts();

    EXPECT_EQ(accounts.size(), 7);
}

TEST_F(TransactionEditViewModelTest, GetToAccounts) {
    auto accounts = viewModel->GetToAccounts();

    EXPECT_EQ(accounts.size(), 5);
}

TEST_F(TransactionEditViewModelTest, SetFromAmount) {
    viewModel->SetFromAmount(10);
    float value = viewModel->GetFromAmount();

    EXPECT_EQ(static_cast<int>(value), 10);
}

TEST_F(TransactionEditViewModelTest, SetToAmount) {
    viewModel->SetToAmount(10);
    float value = viewModel->GetToAmount();

    EXPECT_EQ(static_cast<int>(value), 10);
}

TEST_F(TransactionEditViewModelTest, AddTag) {
    auto tag = std::make_shared<TagPresentationModel>();
    tag->name = "Test";

    viewModel->AddTag(tag);
    auto tags = viewModel->GetTags();
    wxString str = viewModel->GetTagsString();

    EXPECT_EQ(tags.size(), 1);
    EXPECT_TRUE(str == "Test");
}

TEST_F(TransactionEditViewModelTest, SetTagsString) {
    viewModel->SetTagsString("Tag 1, Tag 2, ");

    auto tags = viewModel->GetTags();
    wxString str = viewModel->GetTagsString();

    EXPECT_EQ(tags.size(), 2);
    EXPECT_TRUE(str == "Tag 1, Tag 2");
}