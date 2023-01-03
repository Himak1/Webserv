#include "gtest/gtest.h"
#include "../../src/server/Configuration.hpp"
#include "../../src/request/Request.hpp"
#include "../../src/response/Response.hpp"
#include "../../src/response/CGI.hpp"

class Configuration config;

TEST(CGITest, testFormEnv) 
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

