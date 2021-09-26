#include "pch.h"
#include "Environment.cpp"

class TagsServiceTest : public ::testing::Test {
public:
    TagsServiceTest() {
        auto context = Environment::GetInstance().GetContext();

        service = new TagsService(context->GetTagsRepository());
    }

    ~TagsServiceTest() {
        delete service;
    }


protected:
    TagsService* service;
};

TEST_F(TagsServiceTest, GetAll) {
    auto tags = service->GetAll();

    EXPECT_EQ(tags.size(), 1554);
}

TEST_F(TagsServiceTest, GetByIdExists) {
    auto tag = service->GetById(1);

    EXPECT_TRUE(tag != nullptr);
    EXPECT_EQ(tag->id, 1);
}

TEST_F(TagsServiceTest, GetByIdNotExists) {
    auto tag = service->GetById(10000);

    EXPECT_TRUE(tag == nullptr);
}

TEST_F(TagsServiceTest, GetBySearch) {
    wxString search = "Разное";

    auto tags = service->GetBySearch(search);

    EXPECT_EQ(tags.size(), 1);
}