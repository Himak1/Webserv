#include "gtest/gtest.h"
#include "../../src/request/Request.hpp"

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
	EXPECT_EQ(request.getStatus(), 200);
}

TEST(RequestTest, POST)
{
	std::string test_string = "POST /cgi-bin/process.cgi HTTP/1.1\n" \
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
	EXPECT_EQ(request.getURI(), "/cgi-bin/process.cgi");
	EXPECT_EQ(request.getHTTPVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getExtension(), ".cgi");
	EXPECT_EQ(request.getStatus(), 200);
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
	EXPECT_EQ(request.getStatus(), 200);
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
	EXPECT_EQ(request.getStatus(), 400);
}

