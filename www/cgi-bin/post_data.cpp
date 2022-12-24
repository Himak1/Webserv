#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

int main ()
{
   cout << "<html>\n";
   cout << "<head>\n";
   cout << "<title>Using GET and POST Methods</title>\n";
   cout << "</head>\n";
   cout << "<body>\n";

   char* data = getenv("QUERY_STRING");
   if(data != NULL) {
	  char* first_name = strtok(data, "&");
	  char* last_name = strtok(NULL, "&");

	  if(first_name != NULL && last_name != NULL) {
		 cout << "First name: " << first_name << endl;
		 cout << "Last name: " << last_name << endl;
	  }
	  else
		 cout << "Error: Invalid form data" << endl;
   }
   else
	  cout << "Error: No form data received" << endl;
   
   cout << "<br/>\n";
   cout << "</body>\n";
   cout << "</html>\n";
   
   return 0;
}