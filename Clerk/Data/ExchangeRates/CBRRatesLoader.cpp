#include "CBRRatesLoader.h"

static int writer(char *data, size_t size, size_t nmemb, string *writerData)
{
	if (writerData == NULL) return 0;

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

CBRRatesLoader::CBRRatesLoader(sqlite3 *db) : ExchangeRatesLoader(db) {
	url = "http://www.cbr.ru/scripts/XML_daily.asp";
}

void CBRRatesLoader::Load() {
	wxDateTime today = wxDateTime::Now();
	wxDateTime lastDate = wxDateTime();

	char *sql = "SELECT MAX(date) FROM exchange_rates";
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(db, sql, -1, &statement, NULL) == SQLITE_OK) {
		if (sqlite3_step(statement) == SQLITE_ROW) {			
			lastDate.ParseISODate(wxString::FromUTF8((char *)sqlite3_column_text(statement, 0)));
		}
	}

	sqlite3_finalize(statement);

	if (!today.IsSameDate(lastDate)) {
		CURL *curl;
		CURLcode result;

		curl = curl_easy_init();

		if (curl) {
			string buffer;

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

			result = curl_easy_perform(curl);

			if (result == CURLE_OK) {
				Parse(&buffer);
			}

			curl_easy_cleanup(curl);
		}
	}
}

void CBRRatesLoader::Parse(string *data) {
	xml_document<> document;
	document.parse<0>((char *)data->c_str());

	xml_node<> *rootNode = document.first_node("ValCurs");
	char *dateValue = rootNode->first_attribute("Date")->value();

	wxString::const_iterator end;

	wxDateTime date = wxDateTime::Now();
	date.ParseFormat(dateValue, "DD.MM.YYYY", &end);

	for (xml_node<> *child = rootNode->first_node(); child; child = child->next_sibling())
	{
		xml_node<> *codeNode = child->first_node("CharCode");
		xml_node<> *rateNode = child->first_node("Value");
		xml_node<> *countNode = child->first_node("Nominal");

		char *codeValue = codeNode->value();
		char *rateValue = rateNode->value();
		char *countValue = countNode->value();

		string valueStr(rateValue);

		std::replace(valueStr.begin(), valueStr.end(), ',', '.');

		float rate = atof(valueStr.c_str());
		int count = atoi(countValue);

		string to("RUB");
		string from(codeValue);

		UpdateValue(&date, &from, &to, rate, count);
	}
}