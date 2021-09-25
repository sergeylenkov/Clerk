#include "pch.h"

#include "../Clerk/Data/Repositories/TagsRepository.h"

class TagsRepositoryTest : public ::testing::Test {
public:
    TagsRepositoryTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::TagsRepository(*connection);
    }

    ~TagsRepositoryTest() {
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
    Clerk::Data::TagsRepository* repository;
};

TEST_F(TagsRepositoryTest, GetAll) {
    auto tags = repository->GetAll();

    EXPECT_EQ(tags.size(), 1554);
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

    EXPECT_EQ(count, 4279);
}

TEST_F(TagsRepositoryTest, CreateAndDelete) {
    int count = repository->GetAll().size();

    TagModel* newTag = new TagModel();

    repository->Save(*newTag);

    EXPECT_NE(newTag->id, -1);

    auto tag = repository->GetById(newTag->id);

    ASSERT_TRUE(tag != nullptr);

    repository->Delete(*newTag);

    delete newTag;

    EXPECT_EQ(repository->GetAll().size(), count);
}

TEST_F(TagsRepositoryTest, Update) {
    auto tag = repository->GetById(100);

    ASSERT_TRUE(tag != nullptr);

    std::string newName = wxString::Format("%d", rand()).ToUTF8();
    
    tag->name = newName;

    repository->Save(*tag);

    auto newTag = repository->GetById(100);

    EXPECT_TRUE(newTag->name == newName);
}