#include "ExchangeRatesLoader.h"

static int writer(char *data, size_t size, size_t nmemb, std::string *writerData)
{
	if (writerData == NULL) return 0;

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

ExchangeRatesLoader::ExchangeRatesLoader(sqlite3 *db) {
	this->db = db;
}

void ExchangeRatesLoader::Load() {
	std::string url = "http://www.cbr.ru/scripts/XML_daily.asp";

	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		
		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);

		if (res == CURLE_OK) {
			std::string buffer;
			res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
			
			if (res == CURLE_OK) {
				res = curl_easy_perform(curl);

				if (res == CURLE_OK) {
					Parse(buffer);
				}
			}
		}

		curl_easy_cleanup(curl);
	}
}

void ExchangeRatesLoader::Parse(std::string data) {
	rapidxml::xml_document<> doc;
	doc.parse<0>((char *)data.c_str());

	rapidxml::xml_node<> *node = doc.first_node("ValCurs");

	//while (node->next_sibling("Valute") != 0) {

	for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling())
	{
			// do stuff with child
	}
}