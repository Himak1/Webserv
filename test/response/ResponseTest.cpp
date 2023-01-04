#include "gtest/gtest.h"
#include "../../src/server/Configuration.hpp"
#include "../../src/request/Request.hpp"
#include "../../src/response/Response.hpp"

class Configuration config;

using namespace std;

// TO DO: test with costum _config path
TEST(ResponseTest, testPath) 
{
	class Request request;
	{
		request.initRequest("GET / HTTP/1.1\n");
		class Response respons(request, config);
		EXPECT_EQ(respons.getFilepath(), "www/index.html");
	}

	{
		request.initRequest("GET /favicon.ico HTTP/1.1\n");
		class Response respons(request, config);
		EXPECT_EQ(respons.getFilepath(), "www/favicon.ico");
	}

	{
		request.initRequest("GET /test HTTP/1.1\n");
		class Response respons(request, config);
		EXPECT_EQ(respons.getFilepath(), "www/test.html");
	}
}

// TO DO: test for costum 404 error html
TEST(ResponseTest, fileNotFound_default)
{
	class Request request;
	request.initRequest("GET /non_existing_file HTTP/1.1\n");

	string expected_respons = "HTTP/1.1 404 Not Found\n" \
		"Content-Type: text/html; charset=utf-8\nContent-Length: " \
		"133\n\n" \
		"<!DOCTYPE html>" \
		"<html lang=\"en\">" \
		"<head><title>404 Not Found\n</title></head>" \
		"<body><center><h1>404 Not Found\n</h1></center>" \
		"</body></html>";

	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

// TO DO: test for costum 301
TEST(ResponseTest, redirect301)
{
	class Request request;
	request.initRequest("GET /permanently_moved HTTP/1.1\n");
	string expected_respons = "HTTP/1.1 301 Moved Permanently\nContent-Type: text/html; charset=utf-8\nContent-Length: 219\n\n<!DOCTYPE html><html lang=\"en\"><head><title>301 Moved Permanently\n</title><meta charset=\"utf-8\"/><meta http-equiv=\"refresh\" content=\"5; url=/\"/></head><body><center><h1>301 Moved Permanently\n</h1></center></body></html>";
	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

// TO DO: test for costum 302
TEST(ResponseTest, redirect302)
{
	class Request request;
	request.initRequest("GET /temporary_unavailable HTTP/1.1\n");
	string expected_respons = "HTTP/1.1 302 Found\nContent-Type: text/html; charset=utf-8\nContent-Length: 195\n\n<!DOCTYPE html><html lang=\"en\"><head><title>302 Found\n</title><meta charset=\"utf-8\"/><meta http-equiv=\"refresh\" content=\"5; url=/\"/></head><body><center><h1>302 Found\n</h1></center></body></html>";
	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

TEST(ResponseTest, unsupported_media_type)
{
	class Request request;
	request.initRequest("GET /favicon.iico HTTP/1.1\n");
	string expected_respons = "HTTP/1.1 415 Unsupported Media Type\nContent-Length: 159\n\n<!DOCTYPE html><html lang=\"en\"><head><title>415 Unsupported Media Type\n</title></head><body><center><h1>415 Unsupported Media Type\n</h1></center></body></html>";
	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

// TO DO: 
/*
TEST(ResponseTest, upload_files)
{
	request.initRequest("GET /upload_files HTTP/1.1");

	string expected_respons = "HTTP/1.1 200 OK\n" \
		"Content-Type: text/html\nContent-Length: " \
		"39\n\n" \
		"<form action = \"/cgi-bin/upload_files.cgi\" method=\"POST\" enctype=\"multipart/form-data\">\n" \
		"<label for=\"fileInput\">Select a file:</label><br>\">\n" \
		"<input type=\"file\" id=\"fileInput\" name=\"file\"><br>\">\n" \
		"<input type=\"submit\" value=\"Upload\">\">\n" \
		"</form>";

	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}
*/