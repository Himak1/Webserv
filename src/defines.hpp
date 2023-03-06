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
# define INTERNAL_SERVER_ERROR 500
# define NOT_IMPLEMENTED 501
# define BAD_GATEWAY 502
# define HTTP_VERSION_NOT_SUPPORTED 505

// TCPServer defines
# define POLL_TIMEOUT 200	 
# define BUFFERSIZE 420000

// CGI defines
# define MAX_BUFFERSIZE_CGI 420000
# define BUFFER_ERROR_SIZE 100
# define CGI_TIMEOUT 10
# define INTERNAL_SERVER_ERROR_MSG \
    "<!doctype html><html lang=\"en\"><head><title>" \
	"500 Internal Server Error\n</title></head><body><center><h1>" \
	"500 Internal Server Error\n</h1></center></body></html>"

# define BAD_GATEWAY_MSG \
    "<!doctype html><html lang=\"en\"><head><title>" \
	"502 Bad Gateway\n</title></head><body><center><h1>" \
	"502 Bad Gateway\n</h1></center></body></html>"

# define REQUEST_ENTITY_TOO_LARGE_MSG \
    "<!doctype html><html lang=\"en\"><head><title>" \
    "413 Request Entity Too Large\n</title></head><body><center><h1>" \
    "413 Request Entity Too Large\n</h1></center></body></html>"

#endif
