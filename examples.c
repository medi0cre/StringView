#include <stdio.h>

#define STRINGVIEW_IMPLEMENTATION
#include "StringView.h"

int main()
{
    const char* httpRequest = "GET /index.html HTTP/1.1\r\n"
                              "header1: value1\r\n"
                              "header2: value2\r\n"
                              "header3: value3\r\n"
                              "\r\n"
                              "lorem ipsum something something";

    StringView req = SV(httpRequest);
    StringView method, uri, version;
    StringView headerName, headerValue;

    // Obtain method
    SVTrimLeft(&req);
    SVTokenize(&req, &method, ' ');

    // Obtain URI
    SVTrimLeft(&req);
    SVTokenize(&req, &uri, ' ');
   
    // Obtain version
    SVTrimLeft(&req);
    SVTokenize(&req, &version, '\r');
    SVConsume(&req, SV_ADDR("\n"));

    printf("Method = "SV_FMT", URI = "SV_FMT", Version = "SV_FMT"\n", SV_ARGS(method), SV_ARGS(uri), SV_ARGS(version));
    
    while(true)
    {
        SVTokenize(&req, &headerName, ':');
        SVTrimLeft(&req);

        SVTokenize(&req, &headerValue, '\r');
        SVConsume(&req, SV_ADDR("\n"));

        printf("Header Name = "SV_FMT", Header Value = "SV_FMT"\n", SV_ARGS(headerName), SV_ARGS(headerValue));

        if (SVConsume(&req, SV_ADDR("\r\n"))) { break; }
    }

    printf("Body = "SV_FMT"\n", SV_ARGS(req));

    return 0;
}
