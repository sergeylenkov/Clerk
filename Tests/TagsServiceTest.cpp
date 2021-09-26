#include "pch.h"
#include "Fixture.cpp"

class TagsServiceTest : public Fixture {
public:
    void SetUp() override {
        Fixture::SetUp();

        service = new TagsService(context->GetTagsRepository());
    }

    void TearDown() override {
        Fixture::TearDown();
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
    wxString search = wxString("te");

    auto tags = service->GetBySearch(search);

    EXPECT_EQ(tags.size(), 38);
}