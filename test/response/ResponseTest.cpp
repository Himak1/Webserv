#include "gtest/gtest.h"
#include "../../src/server/Configuration.hpp"
#include "../../src/request/Request.hpp"
#include "../../src/response/Response.hpp"

class Request request;
class Configuration config;

// TO DO: test with costum _config path
TEST(ResponseTest, testPath) 
{
	{
		request.initRequest("GET / HTTP/1.1");
		class Response respons(request, config);
		EXPECT_EQ(respons.getFilename(), "www/index.html");
	}

	{
		request.initRequest("GET /favicon.ico HTTP/1.1");
		class Response respons(request, config);
		EXPECT_EQ(respons.getFilename(), "www/favicon.ico");
	}

	{
		request.initRequest("GET /test HTTP/1.1");
		class Response respons(request, config);
		EXPECT_EQ(respons.getFilename(), "www/test.html");
	}
}

// TO DO: test for costum 404 error html
TEST(ResponseTest, fileNotFound_default)
{
	request.initRequest("GET /non_existing_file HTTP/1.1");

	std::string expected_respons = "HTTP/1.1 404 Not Found\n" \
		"Content-Type: text/html\nContent-Length: " \
		"131\n\n" \
		"<!DOCTYPE html>" \
		"<html lang=\"en\">" \
		"<head><title>404 Not Found</title></head>" \
		"<body><center><h1>404 Not Found</h1></center>" \
		"</body></html>";

	class Response respons(request, config);
	EXPECT_EQ(respons.getMessage(), expected_respons);
}

// TO DO: 
/*
TEST(ResponseTest, upload_files)
{
	request.initRequest("GET /upload_files HTTP/1.1");

	std::string expected_respons = "HTTP/1.1 200 OK\n" \
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