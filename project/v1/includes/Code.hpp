#ifndef CODE_HPP
#define CODE_HPP

#include <map>
#include <string>

/*
** Const
*/

#define OK_200							200
#define CREATED_201						201
#define ACCEPTED_202					202
#define NO_CONTENT_204					204
#define BAD_REQUEST_400					400
#define UNAUTHORIZED_401				401
#define NOT_FOUND_404 					404
#define METHOD_NOT_ALLOWED_405			405
#define REQUEST_ENTITY_TOO_LARGE_413	413
#define SERVICE_UNAVAILABLE_503			503
#define NOT_IMPLEMENTED_501				501
#define INTERNAL_ERROR_500				500

/*
** status_code => reason_phrase
*/

static std::map<int, std::string> code_to_reason = {

		{ 100, "Continue" },
		{ 101, "Switching Protocols" },
		{ 102, "Processing" },
		{ 103, "Early hints" },

		{ 200, "OK" },
		{ 201, "Created" },
		{ 202, "Accepted" },
		{ 203, "Non-Authoritative Information" },
		{ 204, "No Content" },
		{ 205, "Reset Content" },
		{ 206, "Partial Content" },
		{ 207, "Multi-Status" },
		{ 208, "Already Reported" },
		{ 210, "Content Different" },
		{ 226, "IM Used" },

		{ 300, "Multiple Choices" },
		{ 301, "Moved permanently" },
		{ 302, "Found" },
		{ 303, "See Other" },
		{ 304, "Not Modified" },
		{ 305, "Use Proxy" },
		{ 306, "Switch Proxy" },
		{ 307, "Temporary Redirect" },
		{ 308, "Permanent Redirect" },
		{ 310, "Too Many Redirects" },

		{ 400, "Bad Request" },
		{ 401, "Unauthorized" },
		{ 402, "Payment Required" },
		{ 403, "Forbidden" },
		{ 404, "Not Found" },
		{ 405, "Method Not Allowed" },
		{ 406, "Not Acceptable" },
		{ 407, "Proxy Authentication Required" },
		{ 408, "Request Time-Out" },
		{ 409, "Conflict" },
		{ 410, "Gone" },
		{ 411, "Length Required" },
		{ 412, "Precondition Failed" },
		{ 413, "Request Entity Too Large" },
		{ 414, "Request-URI Too Long" },
		{ 415, "Unsupported Media Type" },
		{ 416, "Requested range unsatisfiable" },
		{ 417, "Expectation failed" },
		{ 418, "I'm a teapot" },
		{ 421, "Bad mapping" },
		{ 422, "Unprocessable entity" },
		{ 423, "Locked" },
		{ 424, "Method failure" },
		{ 425, "Unordered Collection" },
		{ 426, "Upgrade Required" },
		{ 428, "Precondition Required" },
		{ 429, "Too Many Requests" },
		{ 431, "Request Header Fileds Too Large" },
		{ 449, "Retry With" },
		{ 450, "Blocked by Windows Parental Control" },
		{ 451, "Unavailable for Legal Reasons" },
		{ 456, "Unrecoverable Error" },
		
		{ 444, "No Response" },
		{ 495, "SSL Certificate Error" },
		{ 496, "SSL Certificate Required" },
		{ 497, "HTTP Request Sent to HTTPS Port" },
		{ 498, "Token Expired/Invalid" },
		{ 499, "Client Closed Request" },

		{ 500, "Internal Server Error" },
		{ 501, "Not Implemented" },
		{ 502, "Bad Gateway" },
		{ 503, "Service Unavailable" },
		{ 504, "Gateway Time-Out" },
		{ 505, "HTTP Version non supported" },
		{ 506, "Variant Also Negotiates" },
		{ 507, "Insufficient storage" },
		{ 508, "Loop Detected" },
		{ 509, "Bandwidth Limit Exceeded" },
		{ 510, "Not extended" },
		{ 511, "Network authentication required" },

		{ 520, "Unknown Error" },
		{ 521, "Web Server Is Down" },
		{ 522, "Connection Timed Out" },
		{ 523, "Origin is Unreachable" },
		{ 524, "A Timeout Occurred" },
		{ 525, "SSL Handshake Failed" },
		{ 526, "Invalid SSL Certificate" },
		{ 527, "Railgun Error" }

};

#endif
