#include "TagsRepository.h"

using namespace Clerk::Data;

std::vector<TagModel*> TagsRepository::GetAll() {
	std::vector<TagModel*> result;

	char* sql = "SELECT id FROM tags ORDER BY name";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		while (sqlite3_step(statement) == SQLITE_ROW) {
			int id = sqlite3_column_int(statement, 0);
			result.push_back(Load(id));
		}
	}

	sqlite3_finalize(statement);

	return result;
}

TagModel* TagsRepository::GetById(int id) {
	return Load(id);
}

/*std::vector<std::shared_ptr<TagModel>> TagsRepository::GetBySearch(std::wstring search) {
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
}*/

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

TagModel* TagsRepository::Load(int id) {
	TagModel* tag = nullptr;

	char* sql = "SELECT id, name FROM tags WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, id);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			tag = new TagModel();

			tag->id = sqlite3_column_int(statement, 0);
			tag->name = std::wstring((wchar_t *)sqlite3_column_text16(statement, 1));
		}
	}

	sqlite3_finalize(statement);

	return tag;
}

int TagsRepository::Save(const TagModel& tag)
{
	int id = tag.id;

	if (tag.id == -1) {
		char* sql = "INSERT INTO tags (name) VALUES (?)";
		sqlite3_stmt* statement;

		if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
			sqlite3_bind_text16(statement, 1, tag.name.c_str(), -1, SQLITE_TRANSIENT);

			if (sqlite3_step(statement) == SQLITE_DONE) {
				id = static_cast<int>(sqlite3_last_insert_rowid(_connection.GetConnection()));
				Load(id);
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

	return id;
}

void TagsRepository::Delete(const TagModel& tag)
{
	char* sql = "DELETE FROM tags WHERE id = ?";
	sqlite3_stmt* statement;

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, tag.id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);

	sql = "DELETE FROM transactions_tags WHERE tag_id = ?";

	if (sqlite3_prepare_v2(_connection.GetConnection(), sql, -1, &statement, NULL) == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, tag.id);
		sqlite3_step(statement);
	}

	sqlite3_finalize(statement);
}