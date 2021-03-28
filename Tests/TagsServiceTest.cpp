#include "pch.h"

#include "../Clerk/Data/Repositories/TagsRepository.h"
#include "../Clerk/Data/Services/TagsService.h"

class TagsServiceTest : public ::testing::Test {
public:
    TagsServiceTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::TagsRepository(*connection);
        service = new Clerk::Data::TagsService(*repository);
    }

    ~TagsServiceTest() {
        delete connection;
        delete repository;
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
    Clerk::Data::TagsRepository* repository;
    Clerk::Data::TagsService* service;
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