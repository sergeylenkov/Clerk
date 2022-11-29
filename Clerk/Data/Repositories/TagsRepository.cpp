#include "TagsRepository.h"

using namespace Clerk::Data;

std::vector<std::shared_ptr<TagModel>> TagsRepository::GetAll() {
	if (GetHashList().empty()) {
		char* sql = "SELECT id FROM tags ORDER BY name";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			while (sqlite3_step(statement) == SQLITE_ROW) {
				int id = sqlite3_column_int(statement, 0);
				auto tag = Load(id);
				
				AddToHash(id, tag);
			}
		}

		sqlite3_finalize(statement);
	}

	return GetHashList();
}

std::shared_ptr<TagModel> TagsRepository::GetById(int id) {
	std::shared_ptr<TagModel> tag = GetFromHash(id);

	if (!tag) {
		tag = Load(id);
		AddToHash(id, tag);
	}

	return tag;
}

std::vector<std::shared_ptr<TagModel>> TagsRepository::GetBySearch(std::wstring search) {
	auto result = std::vector<std::shared_ptr<TagModel>>();

	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());

	f.tolower(&search[0], &search[0] + search.size());

	for (auto& tag : GetAll()) {
		std::wstring tagName = tag->name;
		f.tolower(&tagName[0], &tagName[0] + tagName.size());

		std::size_t found = tagName.find(search);

		if (found != std::string::npos) {
			result.push_back(tag);
		}
	}

	return result;
}

int TagsRepository::GetCount(int id) {
	int result = 0;

	char* sql = "SELECT COUNT(*) FROM transactions_tags WHERE tag_id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			result = sqlite3_column_int(statement, 0);
		}
	}

	sqlite3_finalize(statement);

	return result;
}

std::shared_ptr<TagModel> TagsRepository::Load(int id) {
	std::shared_ptr<TagModel> tag = nullptr;

	char* sql = "SELECT id, name FROM tags WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			tag = std::make_shared<TagModel>();

			tag->id = sqlite3_column_int(statement, 0);
			tag->name = std::wstring((wchar_t *)sqlite3_column_text16(statement, 1));
		}
	}

	sqlite3_finalize(statement);

	return tag;
}

void TagsRepository::Save(TagModel& tag)
{
	if (tag.id == -1) {
		char* sql = "INSERT INTO tags (name) VALUES (?)";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, tag.name.c_str(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				tag.id = static_cast<int>(sqlite3_last_insert_rowid(_connection.GetConnection()));
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		char* sql = "UPDATE tags SET name = ? WHERE id = ?";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, tag.name.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, tag.id);

			sqlite3_step(statement);
		}

		sqlite3_finalize(statement);
		
	}
}

void TagsRepository::Delete(const TagModel& tag)
{
	char* sql = "DELETE FROM tags WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, tag.id);

		if (sqlite3_step(statement) == SQLITE_DONE) {
			RemoveFromHash(tag.id);
		}
	}

	sqlite3_finalize(statement);

	sql = "DELETE FROM transactions_tags WHERE tag_id = ?";

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, tag.id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}