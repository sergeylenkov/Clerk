#include "pch.h"

#include "../Clerk/UI/ViewModels/TransactionViewModel.h"
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

    EXPECT_EQ(accounts.size(), 6);
}

TEST_F(TransactionViewModelTest, GetToAccounts) {
    auto accounts = viewModel->GetToAccounts();

    EXPECT_EQ(accounts.size(), 3);
}


TEST_F(TransactionViewModelTest, SetFromAccount) {
    viewModel->SetFromAccount(2);

    EXPECT_EQ(viewModel->GetFromAccountIndex(), 2);
}

TEST_F(TransactionViewModelTest, SetToAccount) {
    viewModel->SetToAccount(2);

    EXPECT_EQ(viewModel->GetToAccountIndex(), 2);
}

TEST_F(TransactionViewModelTest, SetFromAmount) {
    viewModel->SetFromAmount(10);

    EXPECT_EQ(static_cast<int>(viewModel->GetFromAmount()), 10);
}

TEST_F(TransactionViewModelTest, SetToAmount) {
    viewModel->SetToAmount(10);

    EXPECT_EQ(static_cast<int>(viewModel->GetToAmount()), 10);
}

TEST_F(TransactionViewModelTest, SetNote) {
    viewModel->SetNote("Test");

    EXPECT_TRUE(viewModel->GetNote() == "Test");
}

TEST_F(TransactionViewModelTest, SetDate) {
    wxDateTime date = wxDateTime::Now();
    date.SetYear(2022);
    date.SetMonth(wxDateTime::Jan);
    date.SetDay(1);

    viewModel->SetDate(date);

    wxDateTime value = viewModel->GetDate();

    EXPECT_EQ(value.GetYear(), date.GetYear());
    EXPECT_EQ(value.GetMonth(), date.GetMonth());
    EXPECT_EQ(value.GetDay(), date.GetDay());
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

TEST_F(TransactionViewModelTest, OnUpdate) {
    viewModel->OnUpdate = ([&](TransactionViewModelField field) {
        if (field == TransactionViewModelField::Note) {
            EXPECT_TRUE(viewModel->GetNote() == "Update");
        }
        else {
            FAIL() << "We shouldn't get here.";
        }
     });

    viewModel->SetNote("Update");
}