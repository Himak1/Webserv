#include "gtest/gtest.h"
#include "../../src/request/Request.hpp"
#include "../../src/defines.hpp"

class Request request;


TEST(RequestTest, GET)
{
	std::string test_string = "GET /hello.html HTTP/1.1\n" \
		"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n" \
		"Host: www.tutorialspoint.com\n" \
		"Accept-Language: en-us\n" \
		"Accept-Encoding: gzip, deflate\n" \
		"Connection: Keep-Alive";

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getURI(), "/hello.html");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".html");
	EXPECT_EQ(request.getStatus(), OK);
}

TEST(RequestTest, POST)
{
	std::string test_string = "POST /cgi-bin/process HTTP/1.1\n" \
		"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n" \
		"Host: www.tutorialspoint.com\n" \
		"Content-Type: application/x-www-form-urlencoded\n" \
		"Content-Length: length\n" \
		"Accept-Language: en-us\n" \
		"Accept-Encoding: gzip, deflate\n" \
		"Connection: Keep-Alive\n\n" \
		"licenseID=string&content=string&/paramsXML=string";

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "POST");
	EXPECT_EQ(request.getURI(), "/cgi-bin/process");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".html");
	EXPECT_EQ(request.getStatus(), OK);
}

TEST(RequestTest, DELETE)
{
	std::string test_string = "DELETE /echo/delete/json HTTP/1.1\n" \
		"Authorization: Bearer mt0dgHmLJMVQhvjpNXDyA83vA_PxH23Y\n" \
		"Accept: application/json\n" \
		"Content-Type: application/json\n" \
		"Content-Length: 19\n" \
		"Host: reqbin.com";

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "DELETE");
	EXPECT_EQ(request.getURI(), "/echo/delete/json");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".html");
	EXPECT_EQ(request.getStatus(), OK);
}

TEST(RequestTest, invalid_method)
{
	std::string test_string = "HEAD /echo/delete/json HTTP/1.1\n" \
		"Authorization: Bearer mt0dgHmLJMVQhvjpNXDyA83vA_PxH23Y\n" \
		"Accept: application/json\n" \
		"Content-Type: application/json\n" \
		"Content-Length: 19\n" \
		"Host: reqbin.com";

	request.initRequest(test_string);
	EXPECT_EQ(request.getStatus(), NOT_IMPLEMENTED);
}

TEST(RequestTest, invalid_httpversion)
{
	std::string test_string = "GET /echo/delete/json HTTP/2\n" \
		"Authorization: Bearer mt0dgHmLJMVQhvjpNXDyA83vA_PxH23Y\n" \
		"Accept: application/json\n" \
		"Content-Type: application/json\n" \
		"Content-Length: 19\n" \
		"Host: reqbin.com";

	request.initRequest(test_string);
	EXPECT_EQ(request.getStatus(), HTTP_VERSION_NOT_SUPPORTED);
}

void print(std::list<std::string> const &list)
{
    for (auto const &i: list) {
        std::cout << i << std::endl;
    }
}

TEST(RequestTest, env_list_GET)
{
std::string test_string = "GET /cgi-bin/form.cgi?first_name=Milan&last_name=Weitenberg HTTP/1.1\n" \
		"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n" \
		"Host: www.tutorialspoint.com\n" \
		"Content-Type: application/x-www-form-urlencoded\n" \
		"Content-Length: length\n" \
		"Accept-Language: en-us\n" \
		"Accept-Encoding: gzip, deflate\n" \
		"Connection: Keep-Alive\n\n" \
		"licenseID=string&content=string&/paramsXML=string";

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getURI(), "/cgi-bin/form.cgi?first_name=Milan&last_name=Weitenberg");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".cgi");
	EXPECT_EQ(request.getStatus(), OK);

	list<string>	env_list = request.getEnv();
	print(env_list);
	auto it = env_list.cbegin();
	EXPECT_EQ((*it), "first_name=Milan");
	it++;
	EXPECT_EQ(*it, "last_name=Weitenberg");
}

TEST(RequestTest, env_list_GET_empty)
{
std::string test_string = "GET /cgi-bin/form.cgi?first_name=&last_name= HTTP/1.1\n" \
		"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n" \
		"Host: www.tutorialspoint.com\n" \
		"Content-Type: application/x-www-form-urlencoded\n" \
		"Content-Length: length\n" \
		"Accept-Language: en-us\n" \
		"Accept-Encoding: gzip, deflate\n" \
		"Connection: Keep-Alive\n\n" \
		"licenseID=string&content=string&/paramsXML=string";

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getURI(), "/cgi-bin/form.cgi?first_name=&last_name=");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".cgi");
	EXPECT_EQ(request.getStatus(), OK);

	list<string>	env_list = request.getEnv();
	print(env_list);
	auto it = env_list.cbegin();
	EXPECT_EQ((*it), "first_name=");
	it++;
	EXPECT_EQ(*it, "last_name=");
}

TEST(RequestTest, env_list_POST)
{
	std::string test_string = "POST /cgi-bin/form.cgi HTTP/1.1" \
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

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "POST");
	EXPECT_EQ(request.getURI(), "/cgi-bin/form.cgi");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".cgi");
	EXPECT_EQ(request.getStatus(), OK);

	list<string>	env_list_2 = request.getEnv();
	print(env_list_2);
	auto it2 = env_list_2.cbegin();
	EXPECT_EQ((*it2), "first_name=Milan");
	it2++;
	EXPECT_EQ(*it2, "last_name=Nivard");
}

// TO DO? will not work if input fields are empty. 
// Currently solved with 'required' tag in html form
// TEST(RequestTest, env_list_POST_empty)
// {
// 	std::string test_string = "POST /cgi-bin/form.cgi HTTP/1.1" \
// 		"Host: localhost:8002\n" \
// 		"Connection: keep-alive\n" \
// 		"Content-Length: 252\n" \
// 		"Cache-Control: max-age=0\n" \
// 		"sec-ch-ua: \"Not?A_Brand\";v=\"8\", \"Chromium\";v=\"108\", \"Google Chrome\";v=\"108\"\n" \
// 		"sec-ch-ua-mobile: ?0\n" \
// 		"sec-ch-ua-platform: \"macOS\"\n" \
// 		"Origin: http://localhost:8002\n" \
// 		"DNT: 1\n" \
// 		"Upgrade-Insecure-Requests: 1\n" \
// 		"Content-Type: multipart/form-data; boundary=----WebKitFormBoundarycFDkSdFGBfMmUHBF\n" \
// 		"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36\n" \
// 		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n" \
// 		"Sec-Fetch-Site: same-origin\n" \
// 		"Sec-Fetch-Mode: navigate\n" \
// 		"Sec-Fetch-User: ?1\n" \
// 		"Sec-Fetch-Dest: document\n" \
// 		"Referer: http://localhost:8002/form_post\n" \
// 		"Accept-Encoding: gzip, deflate, br\n" \
// 		"Accept-Language: nl-NL,nl;q=0.9,en-US;q=0.8,en;q=0.7\n\n" \
// 		"Cookie: _ga=GA1.1.885127528.1671626776; _scid=06d1678e-0192-4660-b27b-6fc06cb31016; hubspotutk=19b15014c9b9b1756dd5d5aefd63497d; __hstc=181257784.19b15014c9b9b1756dd5d5aefd63497d.1671626777281.1671629297789.1671641955265.3; collect_chat_page_load=10; _ga_LX73YYEH7W=GS1.1.1671641952.3.1.1671643490.0.0.0\n\n" \
// 		"------WebKitFormBoundarycFDkSdFGBfMmUHBF\n" \
// 		"Content-Disposition: form-data; name=\"first_name\"\n\n" \
// 		"------WebKitFormBoundarycFDkSdFGBfMmUHBF\n" \
// 		"Content-Disposition: form-data; name=\"last_name\"\n\n" \
// 		"------WebKitFormBoundarycFDkSdFGBfMmUHBF--";

// 	request.initRequest(test_string);

// 	EXPECT_EQ(request.getMethod(), "POST");
// 	EXPECT_EQ(request.getURI(), "/cgi-bin/form.cgi");
// 	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
// 	EXPECT_EQ(request.getExtension(), ".cgi");
// 	EXPECT_EQ(request.getStatus(), OK);

// 	list<string>	env_list_2 = request.getEnv();
// 	print(env_list_2);
// 	auto it2 = env_list_2.cbegin();
// 	EXPECT_EQ((*it2), "first_name=Milan");
// 	it2++;
// 	EXPECT_EQ(*it2, "last_name=Nivard");
// }

