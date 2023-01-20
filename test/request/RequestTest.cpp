#include "gtest/gtest.h"
#include "../../src/request/Request.hpp"
#include "../../src/defines.hpp"
#include "../../src/utils/strings.hpp"

using namespace std;

class Request request;


TEST(RequestTest, copyConstructor)
{
	string test_string = "GET /hello.html HTTP/1.1\n" \
		"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n" \
		"Host: www.tutorialspoint.com\n" \
		"Accept-Language: en-us\n" \
		"Accept-Encoding: gzip, deflate\n" \
		"Connection: Keep-Alive";

	request.initRequest(test_string);

	class Request request_copy(request);

	EXPECT_EQ(request.getMethod(), request_copy.getMethod());
	EXPECT_EQ(request.getURI(), request_copy.getURI());
	EXPECT_EQ(request.getHTTPVersion(), request_copy.getHTTPVersion());
	EXPECT_EQ(request.getExtension(), request_copy.getExtension());
	EXPECT_EQ(request.isCGI(), request_copy.isCGI());
	EXPECT_EQ(request.getUploadSucces(), request_copy.getUploadSucces());
}


TEST(RequestTest, GET)
{
	string test_string = "GET /hello.html HTTP/1.1\n" \
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
	EXPECT_EQ(request.isCGI(), false);
}

TEST(RequestTest, POST)
{
	string test_string = "POST /cgi-bin/process HTTP/1.1\n" \
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
	EXPECT_EQ(request.getExtension(), ".php");
	EXPECT_EQ(request.isCGI(), true);
}

TEST(RequestTest, DELETE)
{
	string test_string = "DELETE /echo/delete/json HTTP/1.1\n" \
		"Authorization: Bearer mt0dgHmLJMVQhvjpNXDyA83vA_PxH23Y\n" \
		"Accept: application/json\n" \
		"Content-Type: application/json\n" \
		"Content-Length: 19\n" \
		"Host: reqbin.com";

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "DELETE");
	EXPECT_EQ(request.getURI(), "/echo/delete/json");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".php");
	EXPECT_EQ(request.isCGI(), true);
}

void print(map<string, string> const &myMap)
{
    for(map<string, string>::const_iterator it = myMap.begin(); it != myMap.end(); ++it)
	    std::cout << it->first << " " << it->second << endl;
}

TEST(RequestTest, envGET)
{
string test_string = "GET /cgi-bin/form.php?first_name=Milan&last_name=Weitenberg HTTP/1.1\n" \
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
	EXPECT_EQ(request.getURI(), "/cgi-bin/form.php?first_name=Milan&last_name=Weitenberg");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".php");
	EXPECT_EQ(request.isCGI(), true);

	map<string, string>	env = request.getEnv();
	print(env);
	auto it = env.cbegin();
	EXPECT_EQ((*it).first, "first_name");
	EXPECT_EQ((*it).second, "Milan");
	it++;
	EXPECT_EQ((*it).first, "last_name");
	EXPECT_EQ((*it).second, "Weitenberg");
}

TEST(RequestTest, envGETempty)
{
string test_string = "GET /cgi-bin/form.php?first_name=&last_name= HTTP/1.1\n" \
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
	EXPECT_EQ(request.getURI(), "/cgi-bin/form.php?first_name=&last_name=");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".php");
	EXPECT_EQ(request.isCGI(), true);

	map<string, string>	env = request.getEnv();
	print(env);
	auto it = env.cbegin();
	EXPECT_EQ((*it).first, "first_name");
	EXPECT_EQ((*it).second, "");
	it++;
	EXPECT_EQ((*it).first, "last_name");
	EXPECT_EQ((*it).second, "");
}

TEST(RequestTest, envPOST)
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

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "POST");
	EXPECT_EQ(request.getURI(), "/cgi-bin/form.cgi");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".cgi");

	map<string, string>	env = request.getEnv();
	print(env);
	auto it = env.cbegin();
	EXPECT_EQ((*it).first, "first_name");
	EXPECT_EQ((*it).second, "Milan");
	it++;
	EXPECT_EQ((*it).first, "last_name");
	EXPECT_EQ((*it).second, "Nivard");
}

// TO DO? will not work if input fields are empty. 
// Currently solved with 'required' tag in html form

TEST(RequestTest, MultipleExtensions)
{
	string test_string = "GET /hello.cgi.php.html HTTP/1.1\n" \
		"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n" \
		"Host: www.tutorialspoint.com\n" \
		"Accept-Language: en-us\n" \
		"Accept-Encoding: gzip, deflate\n" \
		"Connection: Keep-Alive";

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getURI(), "/hello.cgi.php.html");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".html");
	EXPECT_EQ(request.isCGI(), false);
}

TEST(RequestTest, cookies)
{
	string test_string = "GET index.html HTTP/1.1" \
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
		"Cookie: _ga=GA1.1.885127528.1671626776; _scid=06d1678e-0192-4660-b27b-6fc06cb31016; hubspotutk=19b15014c9b9b1756dd5d5aefd63497d; __hstc=181257784.19b15014c9b9b1756dd5d5aefd63497d.1671626777281.1671629297789.1671641955265.3; collect_chat_page_load=10; _ga_LX73YYEH7W=GS1.1.1671641952.3.1.1671643490.0.0.0\n\n";

	request.initRequest(test_string);

	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getURI(), "index.html");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".html");

	map<string, string>	cookies = request.getCookies();
	print(cookies);
	auto it = cookies.cbegin();
	EXPECT_EQ((*it).first, "__hstc");
	EXPECT_EQ((*it).second, "181257784.19b15014c9b9b1756dd5d5aefd63497d.1671626777281.1671629297789.1671641955265.3");
	it++;
	EXPECT_EQ((*it).first, "_ga");
	EXPECT_EQ((*it).second, "GA1.1.885127528.1671626776");
	it++;
	EXPECT_EQ((*it).first, "_ga_LX73YYEH7W");
	EXPECT_EQ((*it).second, "GS1.1.1671641952.3.1.1671643490.0.0.0");
	it++;
	EXPECT_EQ((*it).first, "_scid");
	EXPECT_EQ((*it).second, "06d1678e-0192-4660-b27b-6fc06cb31016");
	it++;
	EXPECT_EQ((*it).first, "collect_chat_page_load");
	EXPECT_EQ((*it).second, "10");
	it++;
	EXPECT_EQ((*it).first, "hubspotutk");
	EXPECT_EQ((*it).second, "19b15014c9b9b1756dd5d5aefd63497d");
}