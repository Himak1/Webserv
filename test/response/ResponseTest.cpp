#include "gtest/gtest.h"
#include "../../src/configure/Configuration.hpp"
#include "../../src/request/Request.hpp"
#include "../../src/response/Response.hpp"
#include "../../src/utils/strings.hpp"

#include <fstream>
#include <iostream>
/*
class Configuration config;
class Request request;

using namespace std;

// TO DO: test with costum _config path
TEST(ResponseTest, testPath) 
{
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
		EXPECT_EQ(respons.getFilepath(), "www/test.php");
	}
}

// TO DO: test for costum 404 error html
TEST(ResponseTest, fileNotFound_default)
{
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
	request.initRequest("GET /permanently_moved HTTP/1.1\n");
	string expected_respons = "HTTP/1.1 301 Moved Permanently\nContent-Type: text/html; charset=utf-8\nContent-Length: 219\n\n<!DOCTYPE html><html lang=\"en\"><head><title>301 Moved Permanently\n</title><meta charset=\"utf-8\"/><meta http-equiv=\"refresh\" content=\"5; url=/\"/></head><body><center><h1>301 Moved Permanently\n</h1></center></body></html>";
	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

// TO DO: test for costum 302
TEST(ResponseTest, redirect302)
{
	request.initRequest("GET /temporary_unavailable HTTP/1.1\n");
	string expected_respons = "HTTP/1.1 302 Found\nContent-Type: text/html; charset=utf-8\nContent-Length: 195\n\n<!DOCTYPE html><html lang=\"en\"><head><title>302 Found\n</title><meta charset=\"utf-8\"/><meta http-equiv=\"refresh\" content=\"5; url=/\"/></head><body><center><h1>302 Found\n</h1></center></body></html>";
	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

TEST(ResponseTest, deleteRequestNonExistingFile)
{
	request.initRequest("DELETE /nonExistingFile.ico HTTP/1.1\n");
	string expected_respons = "HTTP/1.1 404 Not Found\nContent-type: image/vnd.microsoft.icon\nContent-Length: 133\n\n<!DOCTYPE html><html lang=\"en\"><head><title>404 Not Found\n</title></head><body><center><h1>404 Not Found\n</h1></center></body></html>";
	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

// TO DO: delete request

TEST(ResponseTest, unsupportedMediaType)
{
	request.initRequest("GET /favicon.iico HTTP/1.1\n");
	string expected_respons = "HTTP/1.1 415 Unsupported Media Type\nContent-Length: 159\n\n<!DOCTYPE html><html lang=\"en\"><head><title>415 Unsupported Media Type\n</title></head><body><center><h1>415 Unsupported Media Type\n</h1></center></body></html>";
	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

TEST(ResponseTest, invalidMethod)
{
	request.initRequest("HEAD /favicon.ico HTTP/1.1\n");
	string expected_respons = "HTTP/1.1 501 Not Implemented\nContent-type: image/vnd.microsoft.icon\nContent-Length: 145\n\n<!DOCTYPE html><html lang=\"en\"><head><title>501 Not Implemented\n</title></head><body><center><h1>501 Not Implemented\n</h1></center></body></html>";
	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}


TEST(ResponseTest, invalidHttpversion)
{
	request.initRequest("GET /favicon.ico HTTP/2.1\n");
	string expected_respons = "HTTP/2.1 505 HTTP Version Not Supported\nContent-type: image/vnd.microsoft.icon\nContent-Length: 167\n\n<!DOCTYPE html><html lang=\"en\"><head><title>505 HTTP Version Not Supported\n</title></head><body><center><h1>505 HTTP Version Not Supported\n</h1></center></body></html>";
	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

// TO DO: Test upload files
// TEST(ResponseTest, upload_files)
// {
	// request.initRequest("GET /upload_handler.php?file_to_upload=test.txt HTTP/1.1");
	// string expected_respons = "HTTP/2.1 505 HTTP Version Not Supported\nContent-type: image/vnd.microsoft.icon\nContent-Length: 167\n\n<!DOCTYPE html><html lang=\"en\"><head><title>505 HTTP Version Not Supported\n</title></head><body><center><h1>505 HTTP Version Not Supported\n</h1></center></body></html>";
	// class Response respons(request, config);
	// EXPECT_EQ(respons.getMessage(), expected_respons);
// }

// TO DO: test setCookie
*/