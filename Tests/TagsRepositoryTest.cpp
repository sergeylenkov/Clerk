#include "pch.h"

#include "../Clerk/Data/Repositories/TagsRepository.h"
#include "Fixture.cpp"

class TagsRepositoryTest : public Fixture {
public:
    void SetUp() override {
        repository = new TagsRepository(*connection);
    }

    void TearDown() {}

protected:   
    TagsRepository* repository;
};

TEST_F(TagsRepositoryTest, GetAll) {
    auto tags = repository->GetAll();

    EXPECT_EQ(tags.size(), 1578);
}

TEST_F(TagsRepositoryTest, GetByIdExists) {
    auto tag = repository->GetById(1);

    EXPECT_TRUE(tag != nullptr);
    EXPECT_EQ(tag->id, 1);
}

TEST_F(TagsRepositoryTest, GetByIdNotExists) {
    auto tag = repository->GetById(10000);

    EXPECT_TRUE(tag == nullptr);
}

TEST_F(TagsRepositoryTest, GetBySearch) {
    wxString search = "Yamaha HS8";

    auto tags = repository->GetBySearch(std::wstring(search.ToStdWstring()));

    EXPECT_EQ(tags.size(), 1);
}

TEST_F(TagsRepositoryTest, GetCount) {
    int count = repository->GetCount(1);

    EXPECT_EQ(count, 5178);
}

TEST_F(TagsRepositoryTest, CreateAndDelete) {
    int count = repository->GetAll().size();

    auto newTag = std::make_shared<TagModel>();

    auto savedTag = repository->Save(newTag);

    EXPECT_NE(savedTag->id, -1);

    auto tag = repository->GetById(savedTag->id);

    ASSERT_TRUE(tag != nullptr);

    repository->Delete(newTag);

    EXPECT_EQ(repository->GetAll().size(), count);
}

TEST_F(TagsRepositoryTest, Update) {
    auto tag = repository->GetById(100);

    ASSERT_TRUE(tag != nullptr);

    std::wstring newName = wxString::Format("%d", rand()).ToStdWstring();
    
    tag->name = newName;

    repository->Save(tag);

    auto newTag = repository->GetById(100);

    EXPECT_TRUE(newTag->name == newName);
}