#ifndef HTTP_H
#define HTTP_H

enum http_mime_category {
    MimeCategory_None,
    MimeCategory_Application,
    MimeCategory_Audio,
    MimeCategory_Example,
    MimeCategory_Font,
    MimeCategory_Haptics,
    MimeCategory_Image,
    MimeCategory_Message,
    MimeCategory_Model,
    MimeCategory_Multipart,
    MimeCategory_Text, // NOTE(Tyler): Needs CRLF for newlines
    MimeCategory_Video,
    
    MimeCategory_TOTAL,
    MimeCategory_Unkown = MimeCategory_TOTAL, 
};

const char *MIME_CATEGORIES[] = {
    0,
    "application",
    "audio",
    "example",
    "font",
    "haptics",
    "image",
    "message",
    "model",
    "multipart",
    "text",
    "video"
};

enum http_mime_subtype {
    MimeType_None,
    
    MimeTypeText_HTML,
    MimeTypeText_Javscript,
    MimeTypeText_CSS,
    MimeTypeText_Plain,
    
    MimeType_TOTAL,
    MimeType_Unkown = MimeType_TOTAL, 
};

struct http_mime_type {
    http_mime_category Category;
    http_mime_subtype  Sub;
    const char *Extension;
};

enum http_connection_type {
    HTTPConnection_None,
    HTTPConnection_Close,
    HTTPConnection_KeepAlive,
    HTTPConnection_TOTAL
};

const char *HTTP_CONNECTION_TYPES[HTTPConnection_TOTAL] = {
    0,
    "close",
    "keep-alive",
};

#define HEADERS() \
HEADER(Host,           "Host",            string) \
HEADER(Connection,     "Connection",      http_connection_type) \
HEADER(UserAgent,      "User-Agent",      string) \
HEADER(DNT,            "DNT",             string) \
HEADER(Accept,         "Accept",          array<string>) \
HEADER(Referer,        "Referer",         string) \
HEADER(AcceptEncoding, "Accept-Encoding", array<string>) \
HEADER(AcceptLanguage, "Accept-Language", array<string>) \
// 

enum http_headers {
    HTTPHeader_None,
    
#define HEADER(_Name, _String, _Type) HTTPHeader_##_Name,
    HEADERS()
#undef HEADER
    
    HTTPHeader_TOTAL,
};

const char *HTTP_HEADER_TABLE[HTTPHeader_TOTAL] {
    0,
    
#define HEADER(Name, String, Type) String
    HEADERS()
#undef HEADER
};

struct http_get_request {
    b8 IsValid;
    string File;
    string Version;
    
#define HEADER(Name, String, Type) Type Name;
    HEADERS()
#undef HEADER
};

struct http_connection {
    f32 TimerStart;
    os_socket Socket;
};

#endif //HTTP_H
