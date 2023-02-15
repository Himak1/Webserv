#ifndef MAIN_HPP
# define MAIN_HPP

// STATUS CODES
# define OK 200
# define ACCEPTED 202
# define NO_CONTENT 204
# define MOVED_PERMANENTLY 301
# define FOUND 302
# define BAD_REQUEST 400
# define NOT_FOUND 404
# define METHOD_NOT_ALLOWED 405
# define REQUEST_ENTITY_TOO_LARGE 413
# define UNSUPPORTED_MEDIA_TYPE 415
# define NOT_IMPLEMENTED 501
# define HTTP_VERSION_NOT_SUPPORTED 505

// TEMPORARY 
#define CGI_BUFSIZE 307200

#define COSTUM_301 "/templates/301.html"
#define COSTUM_302 "/templates/302.html"
#define COSTUM_404 "/templates/404.html"
// #define COSTUM_301 "default"
// #define COSTUM_302 "default"
// #define COSTUM_404 "default"

#define UPLOAD_FOLDER "uploads"
#define DIRECTORY_LISTING "true"

#define CASE_301 "/permanently_moved"
#define CASE_302 "/temporary_unavailable"

// CODAM
#define PATH_TO_PHP_CGI "/Users/mweitenb/.brew/bin/php-cgi"
#define PATH_TO_PHP_CGI_LENGTH 33
#define PATH_TO_PY_CGI "/usr/bin/python"
#define PATH_TO_PY_CGI_LENGTH 15

// THUIS
// #define PATH_TO_PHP_CGI "/usr/bin/php-cgi"
// #define PATH_TO_PHP_CGI_LENGTH 16
// #define PATH_TO_PY_CGI "/usr/bin/python3"
// #define PATH_TO_PY_CGI_LENGTH 16

// TO DO:
// - accepted methods (GET POST) moet per location gecheckt worden (geldt dan voor directories en subdirectories).

#endif