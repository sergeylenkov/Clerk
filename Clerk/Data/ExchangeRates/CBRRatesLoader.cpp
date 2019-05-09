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
		xml_node<> *valueNode = child->first_node("Value");

		char *code = codeNode->value();
		char *value = valueNode->value();
		
		string valueStr(value);

		std::replace(valueStr.begin(), valueStr.end(), ',', '.');

		float rate = atof(valueStr.c_str());

		string to("RUB");
		string from(code);

		UpdateValue(&date, &from, &to, rate);
	}
}