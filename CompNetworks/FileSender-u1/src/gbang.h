#ifndef GBANG_H_INCLUDED
#define GBANG_H_INCLUDED

// GBANG protocol info
#define GBANG_VERSION 1

#define GBANG_ERROR_NOFILE  "E_NOFILE "
#define GBANG_ERROR_SOCKS   "E_SOCKS "

#define GBANG_DATA_START    "START_DATA "
#define GBANG_DATA_SENDING  "DATA "
#define GBANG_DATA_END      "END_DATA "

#define GBANG_REQUEST_FILE      "FILE "
#define GBANG_REQUEST_DIR       "DIR "
#define GBANG_REQUEST_SYSINFO   "SYST "
#define GBANG_REQUEST_EXIT      "EXIT "
#define GBANG_REQUEST_SHUTDOWN  "SHUTDOWN "
 
#define GBANG_HEADER_SIZE   16
#define GBANG_DATA_SIZE     1472

// Protocol data structure

struct __attribute__((__packed__)) GrylBangProtoData // Set no padding for the accurate packet size.
{
    //Header
    char version;
    char commandString[ GBANG_HEADER_SIZE - 1 ];
    //Data
    char data[ GBANG_DATA_SIZE ];
};

#endif //GBANG_H_INCLUDED
