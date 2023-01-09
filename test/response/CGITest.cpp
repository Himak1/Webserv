#include "gtest/gtest.h"
#include "../../src/server/Configuration.hpp"
#include "../../src/request/Request.hpp"
#include "../../src/response/Response.hpp"
#include "../../src/response/CGI.hpp"

class Configuration config;

TEST(CGITest, testFormEnvGET)
{
	class Request	request;
	request.initRequest("GET /cgi-bin/form.cgi?first_name=Milan&last_name=Weitenberg\n");
	class Response	respons(request, config);
	class CGI		cgi(request, config);

	char** env = cgi.getFormEnv();
	string env_0(env[0]);
	string env_1(env[1]);
	EXPECT_EQ(env_0, "first_name=Milan");
	EXPECT_EQ(env_1, "last_name=Weitenberg");
}

TEST(CGITest, testFormEnvPOST) 
{
	string test_string = "POST /cgi-bin/form.cgi HTTP/1.1" \
		"Host: localhost:8002\n" \
		"Connection: keep-alive\n" \
		"Content-Length: 252\n" \
		"Cache-Control: max-age=0\n" \
		"sec-ch-ua: \"Not?A_Brand\";v=\"8\", \"Chromium\";v=\"108\", \"Google Chrome\";v=\"108\"\n" \
		"sec-ch-ua-mobile: ?0\n" \
		"sec-ch-ua-platform: \"macOS\"\n" \
		"Origin: http://localhost:8002\n" \
		"DNT: 1\n" \
		"Upgrade-Insecure-Requests: 1\n" \
		"Content-Type: multipart/form-data; boundary=----WebKitFormBoundarycFDkSdFGBfMmUHBF\n" \
		"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36\n" \
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n" \
		"Sec-Fetch-Site: same-origin\n" \
		"Sec-Fetch-Mode: navigate\n" \
		"Sec-Fetch-User: ?1\n" \
		"Sec-Fetch-Dest: document\n" \
		"Referer: http://localhost:8002/form_post\n" \
		"Accept-Encoding: gzip, deflate, br\n" \
		"Accept-Language: nl-NL,nl;q=0.9,en-US;q=0.8,en;q=0.7\n\n" \
		"Cookie: _ga=GA1.1.885127528.1671626776; _scid=06d1678e-0192-4660-b27b-6fc06cb31016; hubspotutk=19b15014c9b9b1756dd5d5aefd63497d; __hstc=181257784.19b15014c9b9b1756dd5d5aefd63497d.1671626777281.1671629297789.1671641955265.3; collect_chat_page_load=10; _ga_LX73YYEH7W=GS1.1.1671641952.3.1.1671643490.0.0.0\n\n" \
		"------WebKitFormBoundarycFDkSdFGBfMmUHBF\n" \
		"Content-Disposition: form-data; name=\"first_name\"\n\n" \
		"Milan\n" \
		"------WebKitFormBoundarycFDkSdFGBfMmUHBF\n" \
		"Content-Disposition: form-data; name=\"last_name\"\n\n" \
		"Nivard\n" \
		"------WebKitFormBoundarycFDkSdFGBfMmUHBF--";
	
	class Request	request;
	request.initRequest(test_string);
	class Response	respons(request, config);
	class CGI		cgi(request, config);

	char** env = cgi.getFormEnv();
	string env_0(env[0]);
	string env_1(env[1]);
	EXPECT_EQ(env_0, "first_name=Milan");
	EXPECT_EQ(env_1, "last_name=Nivard");
}

// TO DO: test filename.cgi.php en filename.php.cgi