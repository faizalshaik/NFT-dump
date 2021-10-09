#include "webservice.h"
#include <curl/curl.h>


static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	MemoryStruct_t *mem = (struct MemoryStruct *)userp;

	if (mem->size + realsize + 1 > mem->capacity)
		return 0;

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	return realsize;
}



int doPostJson(const char* path, const char* jsonData, char* result, int resultLen)
{
	CURL *curl;
	CURLcode res;
	MemoryStruct_t chunk;
	struct curl_slist *headers = NULL;
	char url[512];

	result[0] = 0;
	chunk.memory = result;
	chunk.size = 0;
	chunk.capacity = resultLen;

	curl = curl_easy_init();
	if (curl == NULL)
		return -1;

	sprintf(url, "http://121.165.242.78:8181/nfdump/Cms_api/%s", path);
	
	//sprintf(url, "http://nfdump.viaom.com/Cms_api/%s", path);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
	headers = curl_slist_append(headers, "Expect:");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "sn=terminal486&password=123456");
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"sn\":\"terminal486\",\"password\":\"123456\"}");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);

	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
		return -1;
			//curl_easy_strerror(res));

	curl_easy_cleanup(curl);
	return 0;
}

json_val_t* postJson(const char* path, const char* jsonData)
{
	json_val_t* root = NULL;
	char buff[4098];

	if (doPostJson(path, jsonData, buff, 4096)==0)
	{
		root = parseJson(buff);
	}
	return root;    
}
