#include "pch.h"

#include "../Clerk/Data/ViewModels/TransactionEditViewModel.h"
#include "../Clerk/Data/ViewModels/TagViewModel.h"
#include "../Clerk/Data/Settings.h"
#include "Environment.cpp"

class TransactionEditViewModelTest : public ::testing::Test {
public:
    TransactionEditViewModelTest() {
        Settings::GetInstance().SetConvertCurrency(true);
        auto context = Environment::GetInstance().GetContext();

        viewModel = new TransactionEditViewModel(context->GetAccountsService(), context->GetTransactionsService(), context->GetExchangeRatesRepository());
    }

    ~TransactionEditViewModelTest() {
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
    auto tag = std::make_shared<TagViewModel>();
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