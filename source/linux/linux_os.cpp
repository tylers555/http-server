
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

global b32 LinuxRunning;

// NOTE(Tyler): This has a reason for staying as an interger, floats do not have enough accuracy
internal timespec
LinuxGetWallClock()
{
    timespec Result = {};
    clock_gettime(CLOCK_MONOTONIC_RAW, &Result);
    return(Result);
}

internal f32
LinuxSecondsElapsed(timespec Begin, timespec End){
    // NOTE(Tyler): The (f32) cast must be done after the subtraction, because of precision
    f32 Result = (f32)((End.tv_sec-Begin.tv_sec) + (End.tv_nsec-Begin.tv_nsec)/1E9f);
    return Result;
}

//~ OS API
internal os_file *
OSOpenFile(const char *Path, open_file_flags Flags){
    s32 FileFlags = 0;
    const char *Mode = "";
    if(Flags & OpenFile_Read & OpenFile_Write){
        FileFlags |= O_RDWR;
        Mode = "wb+";
    }else if(Flags & OpenFile_Read){
        FileFlags |= O_RDONLY;
        Mode = "rb";
    }else if(Flags & OpenFile_Write){
        FileFlags |= O_WRONLY;
        Mode = "wb";
    }else{
        Assert(0);
    }
    if(Flags & OpenFile_Clear){
        FileFlags |= O_CREAT | O_TRUNC;
    }
    
    int File = open(Path, FileFlags);
    if(File < 0){
        int A = errno;
        return 0;
    }
    os_file *Result = (os_file *)fdopen(File, Mode);
    
    return(Result);
}

internal u64 
OSGetFileSize(os_file *File){
    if(!File) return 0;
    u64 Result = 0;
    
    struct stat Stats;
    if(fstat(fileno((FILE *)File), &Stats) == 0){
        Result = Stats.st_size;
    }else{
        // TODO(Tyler): Logging
        Result = 0;
    }
    return(Result);
}

internal b32 
OSReadFile(os_file *File, u64 FileOffset, void *Buffer, umw BufferSize){
    if(!File) return 0;
    b32 Result = false;
    fseek((FILE *)File, FileOffset, SEEK_SET);
    if((umw)read(fileno((FILE *)File),
                 Buffer,
                 BufferSize) == BufferSize){
        // NOTE(Tyler): Success!!!
        Result = true;
    }else{
        Assert(0);
    }
    
    return(Result);
}

internal void 
OSCloseFile(os_file *File){
    if(!File) return;
    close(fileno((FILE *)File));
}

// TODO(Tyler): Proper WriteFile for 64-bits
internal u64 
OSWriteToFile(os_file *File, u64 FileOffset, const void *Buffer, umw BufferSize){
    if(!File) return 0;
    fseek((FILE *)File, FileOffset, SEEK_SET);
    u64 BytesWritten = write(fileno((FILE *)File), Buffer, BufferSize);
    return(BytesWritten);
}

internal u64
OSGetLastFileWriteTime(os_file *File){
    u64 Result = 0;
    
    struct stat Stats;
    if(fstat(fileno((FILE *)File), &Stats) == 0){
        Result = Stats.st_mtime;
    }else{
        Result = 0;
    }
    return(Result);
}

internal b8
OSDeleteFileAtPath(const char *Path){
    b8 Result = (remove(Path) > -1);
    return(Result);
}

internal void *
OSVirtualAlloc(umw Size){
    Size += sizeof(umw);
    umw *Memory = (umw *)mmap(0, Size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    Assert(Memory);
    Memory[0] = Size;
    return &Memory[1];
}

internal void 
OSVirtualFree(void *Pointer){
    umw *Memory = (umw *)Pointer;
    Memory -= 1;
    munmap(Memory, Memory[0]);
}

// TODO(Tyler): Make this use sbrk and brk to remove std C lib
internal void *
OSDefaultAlloc(umw Size){
    void *Result = calloc(Size, 1);
    return(Result);
}

internal void *
OSDefaultRealloc(void *Memory, umw Size){
    void *Result = realloc(Memory, Size);;
    return(Result);
}

internal void
OSDefaultFree(void *Pointer){
    free(Pointer);
}

// TODO(Tyler): I'm not sure why this is required, but I don't know how to get 4coder to see the output
#if defined(SNAIL_JUMPY_ASSET_PROCESSOR_BUILD)
internal void
OSVWriteToConsole(const char *Format, va_list VarArgs){
    vprintf(Format, VarArgs);
}
#else
internal void
OSVWriteToConsole(const char *Format, va_list VarArgs){
    char Buffer[DEFAULT_BUFFER_SIZE];
    stbsp_vsnprintf(Buffer, sizeof(Buffer), Format, VarArgs);
    write(fileno(stdout), Buffer, CStringLength(Buffer));
}
#endif // defined(SNAIL_JUMPY_ASSET_PROCESSOR_BUILD)

internal void
OSWriteToConsole(const char *Format, ...){
    va_list VarArgs;
    va_start(VarArgs, Format);
    OSVWriteToConsole(Format, VarArgs);
    va_end(VarArgs);
}

//~ Clipboard
#if 0
internal void
OSCopyChars(const char *Chars, u32 CharCount){
    if(CharCount == 0) return;
    
    if(!OpenClipboard(Win32MainWindow)){
        Assert(0);
        return;
    }
    Assert(EmptyClipboard());
    
    HGLOBAL CopiedCharsHandle = GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT, CharCount+1);
    Assert(CopiedCharsHandle);
    
    char *CopiedChars = (char *)GlobalLock(CopiedCharsHandle);
    CopyMemory(CopiedChars, Chars, CharCount);
    GlobalUnlock(CopiedCharsHandle);
    
    SetClipboardData(CF_TEXT, CopiedCharsHandle);
    
    Assert(CloseClipboard());
}

internal char *
OSPasteChars(memory_arena *Arena){
    if(!IsClipboardFormatAvailable(CF_TEXT)) return 0;
    if(!OpenClipboard(Win32MainWindow)){
        Assert(0);
        return 0;
    }
    
    HGLOBAL Handle = GetClipboardData(CF_TEXT); 
    if(!Handle) return 0;
    
    const char *Chars = (const char *)GlobalLock(Handle);
    if(!Chars) return 0;
    
    char *Result = ArenaPushCString(Arena, Chars);
    
    GlobalUnlock(Handle);
    
    Assert(CloseClipboard());
    return Result;
}

#endif
//~ Miscellaneous
internal void
OSSleepMilliseconds(u32 Milliseconds){
    usleep(1000*Milliseconds);
}

internal void
OSEndGame(){
    LinuxRunning = false;
}

internal u64
OSGetMicroseconds(){
    timespec Time = {};
    clock_gettime(CLOCK_MONOTONIC_RAW, &Time);
    u64 Result = (u64)(Time.tv_sec*1000000 + Time.tv_nsec/1000);
    
    return Result;
}

//~ Socket stuff
internal os_socket
ConnectSocket(const char *HostName, u32 Port){
    const char *PortString = ArenaPushFormatCString(&GlobalTransientMemory, "%u", Port);
    os_socket Result = {};
    
    addrinfo *OriginalInfo = 0;
    {
        int Error = getaddrinfo(HostName, PortString, 0, &OriginalInfo);
        if(Error){
            int A = 0;
            return Result;
        }
    }
    
    if(OriginalInfo && OriginalInfo->ai_addr){
        addrinfo *Info = OriginalInfo;
        
        while(Info && 
              (Info->ai_family != AF_INET)){
            Info = Info->ai_next;
        }
        
        if(!Info) return Result;
        
        int Sock = socket(Info->ai_addr->sa_family, SOCK_STREAM, IPPROTO_TCP);
        if(connect(Sock, Info->ai_addr, (int)Info->ai_addrlen) < 0){
            int A = 0;
            return Result;
        }
        Result.ID = Sock;
        Result.Valid = true;
    }
    
    return Result;
}

internal os_socket
ListenSocket(const char *HostName, u32 Port){
    const char *PortString = ArenaPushFormatCString(&GlobalTransientMemory, "%u", Port);
    os_socket Result = {};
    
    addrinfo *OriginalInfo = 0;
    {
        int Error = getaddrinfo(HostName, PortString, 0, &OriginalInfo);
        if(Error){
            int A = 0;
            return Result;
        }
    }
    
    if(OriginalInfo && OriginalInfo->ai_addr){
        addrinfo *Info = OriginalInfo;
        
        while(Info && 
              (Info->ai_family != AF_INET)){
            Info = Info->ai_next;
        }
        
        if(!Info) return Result;
        
        int Sock = socket(Info->ai_addr->sa_family, SOCK_STREAM, IPPROTO_TCP);
        if(bind(Sock, Info->ai_addr, (int)Info->ai_addrlen) < 0){
            int A = errno;
            return Result;
        }
        
        if(listen(Sock, SOMAXCONN) < 0){
            int A = 0;
            return Result;
        }
        
        fcntl(Sock, F_SETFL, O_NONBLOCK);
        
        Result.ID = Sock;
        Result.Valid = true;
    }
    
    return Result;
}

internal os_socket 
ClientSocket(os_socket Listen){
    os_socket Result = {};
    if(!Listen.Valid) return Result;
    
    int Sock = accept(Listen.ID, 0, 0);
    if(Sock < 0){
        const char *E = strerror(errno);
        return Result;
    }
    
    Result.ID = Sock;
    Result.Valid = true;
    return Result;
}

internal socket_send
SocketSendData(os_socket *Socket, const void *Data, u32 Size){
    socket_send Result = {};
    if(!Socket->Valid) return Result;
    
    int BytesSent = send(Socket->ID, Data, Size, 0);
#if 0
    if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
        fd_set FDSet;
        
        FD_ZERO(&FDSet);
        FD_SET(Socket->ID, &FDSet);
        
        select(Socket->ID+1, 0, &FDSet, 0, 0);
        if(FD_ISSET(Socket->ID, &FDSet)){
            send(Socket->ID);
        }
    }
#endif
    
    if(BytesSent < 0){
        return Result;
    }
    
    Result.BytesSent = BytesSent;
    Result.Success = true;
    if(Size == (u32)BytesSent){
        Result.SentAll = true;
    }
    
    
    return Result;
}

internal socket_recv
SocketReceive(os_socket *Socket, memory_arena *Memory, u32 BufferSize){
    socket_recv Result = {};
    if(!Socket->Valid){
        return Result;
    }
    
    Result.Buffer = ArenaPushArray(Memory, char, BufferSize);
    int BytesReceived = recv(Socket->ID, Result.Buffer, BufferSize, 0);
    if(BytesReceived < 0){
        b8 IsRepeat = (errno == EAGAIN) || (errno == EWOULDBLOCK);
        const char *E = strerror(errno);
        return Result;
    }
    
    Result.BytesReceived = BytesReceived;
    Result.Success = true;
    
    return Result;
}

internal void
SocketClose(os_socket *Socket){
    if(Socket->Valid){
        Assert(close(Socket->ID) == 0);
        Socket->Valid = false;
    }
}

internal b8
SocketPollReceive(os_socket *Socket){
    struct pollfd Poll = {};
    Poll.fd = Socket->ID;
    Poll.events = POLLIN;
    int R = poll(&Poll, 1, 0);
    Assert(R >= 0);
    return (Poll.revents == POLLIN);
}

//~ Window stuff // TODO(Tyler): As of right now these are all stubs
internal void OSCopyChars(const char *Chars, u32 Count){}
internal char *OSPasteChars(memory_arena *Arena){return 0;}
internal void OSProcessInput(os_input *Input) {}

