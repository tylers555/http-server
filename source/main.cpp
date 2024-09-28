// TODO(Tyler): Implement an allocator for the stb libraries
#define STB_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "third_party/stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "third_party/stb_truetype.h"
#define STB_SPRINTF_IMPLEMENTATION
#include "third_party/stb_sprintf.h"

#include "main.h"

#include "logging.cpp"
#include "os.cpp"
#include "file_processing.cpp"
#include "stream.cpp"
#include "debug.cpp"
#include "asset.cpp"
#include "asset_loading.cpp"

#include "http.h"

internal inline string
StringGetExtension(string A){
    return StringSplitLast(&A, '.');
}

internal s32
StringTableFindEnumValue(const char **S, s32 Total, string A){
    for(s32 I=0; I<Total; I++){
        if(StringEquals(A, String(S[I]))) return I;
    }
    return 0;
}

internal inline http_mime_type
HTTPParseMimeType(const char *S){
    http_mime_type Result = {};
    
    s32 SlashIndex = CStringIndexOfFirstChar(S, '/');
    for(s32 I=0; I<MimeCategory_TOTAL; I++){
        if(CStringsEqualUpTo(MIME_CATEGORIES[I], S, SlashIndex)){
            Result.Category = (http_mime_category)I;
        }
    }
    
    return Result;
}

internal http_get_request 
HTTPParseGetRequest(memory_arena *Memory, const char *Raw, u32 RawSize){
    http_get_request Result = {};
    if(RawSize == 0) return Result;
    Result.IsValid = true;
    
    string Request = String(Raw, RawSize);
    
    {
        string FirstLine = StringSplitFirstLine(&Request);
        array<string> Parts = StringSplit(Memory, FirstLine, ' ');
        if(Parts[0] != String("GET")){
            LogMessage("Not a get request!");
            Result.IsValid = false;
            return Result;
        }
        Result.File    = Parts[1];
        Result.Version = Parts[2];
    }
    
    array<string> Lines = StringSplitIntoLines(Memory, Request);
    FOR_EACH(Line, &Lines){
        LogMessage("'%.*s'", FSTR(&Line));
        string Header = StringSplitFirst(&Line, ':');
        StringTrim(&Line);
        if(Header == String("Host")){
            Result.Host = Line;
        }else if(Header == String("Connection")){
            Result.Connection = (http_connection_type)StringTableFindEnumValue(HTTP_CONNECTION_TYPES, HTTPConnection_TOTAL, Line);
        }else if(Header == String("User-Agent")){
            Result.UserAgent = Line;
        }else if(Header == String("DNT")){
            Result.DNT = Line;
        }else if(Header == String("Accept")){
            string Rest = StringSplitFirst(&Line, ';');
            Result.Accept = StringSplit(Memory, Rest, ',');
        }else if(Header == String("Referer")){
            Result.Referer = Line;
        }else if(Header == String("Accept-Encoding")){
            string Rest = StringSplitFirst(&Line, ';');
            Result.AcceptEncoding = StringSplit(Memory, Rest, ',');
        }else if(Header == String("Accept-Language")){
            string Rest = StringSplitFirst(&Line, ';');
            Result.AcceptLanguage = StringSplit(Memory, Rest, ',');
        }
    }
    
    return Result;
}

internal int
Main(){
    {
        umw Size = Megabytes(500);
        void *Memory = OSVirtualAlloc(Size);
        Assert(Memory);
        InitializeArena(&GlobalPermanentMemory, Memory, Size);
    }{
        umw Size = Gigabytes(1);
        void *Memory = OSVirtualAlloc(Size);
        Assert(Memory);
        InitializeArena(&GlobalTransientMemory, Memory, Size);
    }
    
#if 1
    os_socket Socket = ListenSocket("tis0", 5500);
    if(!Socket.Valid){
        LogMessage("Failed to create port!");
        return -1;
    };
    
    LogMessage("Listening...");
    
    u32 Counter = 0;
    dynamic_array<os_socket> OpenSockets;
    InitializeArray(&OpenSockets, 5);
    while(true){
        ArenaClear(&GlobalTransientMemory);
        
        LogMessage("Searching for clients...");
        {
            os_socket Client = ClientSocket(Socket);
            if(Client.Valid){
                ArrayAdd(&OpenSockets, Client);
            }
        }
        
        FOR_EACH_(Client, Index, &OpenSockets){
            LogMessage("%u: Serving client #%u", Counter, Index);
            
            if(!SocketPollReceive(&Client)){
                LogMessage("Not waiting on anything!");
                continue;
            }
            
            socket_recv Recv = SocketReceive(&Client, &GlobalTransientMemory);
            if(!Recv.Success){
                LogMessage("Failed to receive!");
                continue;
            }
            
            http_get_request Get = HTTPParseGetRequest(&GlobalTransientMemory, Recv.Buffer, CStringLength(Recv.Buffer));
            //LogMessage("Request: \r\n'%s'", Recv.Buffer);
            if(!Get.IsValid) continue;
            LogMessage("File: %s", Get.File);
            LogMessage("UserAgent: %s", Get.UserAgent);
            
            {
                ARENA_SCOPED_MARKER(&GlobalTransientMemory);
                memory_arena *Memory = &GlobalTransientMemory;
                
                string FilePath = Get.File;
                string Extension = StringGetExtension(FilePath);
                const char *ContentType = "Content-Type: text/html; charset=utf-8\r\n";
                if(FilePath == "/"){
                    FilePath = "index.html";
                }else if(Extension == "jpg"){
                    ContentType = "Content-Type: image/jpg\r\n";
                    StringAdvance(&FilePath);
                }else if(Extension == "png"){
                    ContentType = "Content-Type: image/png\r\n";
                    StringAdvance(&FilePath);
                }else if(Extension == "ico"){
                    ContentType = "Content-Type: image/x-icon\r\n";
                    StringAdvance(&FilePath);
                }
                
                entire_file File = ReadEntireFile(Memory, CString(Memory, FilePath));
                
                string_builder Builder = BeginResizeableStringBuilder(&GlobalTransientMemory, 1024);
                BuilderAdd(&Builder, "HTTP/1.1 200 OK\r\n");
                BuilderAdd(&Builder, ContentType);
                BuilderAdd(&Builder, "Content-Length: %u\r\n", File.Size);
                BuilderAdd(&Builder, "\r\n");
                BuilderAddData(&Builder, File.Data, File.Size);
                
                
                const char *Response = EndStringBuilder(&Builder);
                LogMessage("Response:", Response);
                socket_send Send = SocketSendBuilderData(&Client, &Builder);
                //socket_send Send = SocketSendCString(&Client, Response);
                LogMessage("Sent: %u / %u", Send.BytesSent, Builder.BufferSize);
                if(!Send.Success){
                    LogMessage("Failed to send!");
                }
            }
            
#if 0            
            if(CStringsEqual(Get.Connection, "close")){
                SocketClose(&Client);
            }
#endif
            
        }
        
        Counter++;
        OSSleepMilliseconds(1000);
    }
    
    SocketClose(&Socket);
#endif
    
    
    return 0;
}