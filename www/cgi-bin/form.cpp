#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

int main ()
{
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<style type='text/css'>";
	cout << "body {";
	cout << "	font-family: Verdana;";
	// cout << "	text-align: center;";
	cout << "	font-size:10pt;";
	cout << "	font-weight:bold;";
	cout << "	margin:50px;";
	cout << "	color:#09477d;";
	cout << "}";
	cout << "</style>";
	cout << "<title>FORM</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";
	cout << "<a href='/'>Back to webserv</a></br></br>\n";

	char* first_name = getenv("first_name");
	char* last_name = getenv("last_name");
	if(first_name != NULL && last_name != NULL) {
		cout << "First name: " << first_name << "</br></br>" << endl;
		cout << "Last name: " << last_name << endl;
	}
	else
		cout << "Error: Invalid form data" << endl;

	cout << "<br/>\n";
	cout << "</body>\n";
	cout << "</html>\n";
   
   return 0;
}