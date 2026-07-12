#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STRINGVIEW_IMPLEMENTATION
#include "StringView.h"

void Enforce(bool Condition, const char* Message)
{
    if (Condition) { return; }
    printf("%s\n", Message);
    exit(EXIT_FAILURE);
}

void ParseHTTPRequest()
{
    const char* req = "GET index.html HTTP/1.1\r\n"
                      "header1: value1\r\n"
                      "header2: value2\r\n"
                      "header3: value3\r\n"
                      "header4: value4\r\n"
                      "\r\n"
                      "Lorem ipsum something something...";

    StringView svreq = SV(req);
    StringView newLine = SV("\n");

    SVTrimLeft(&svreq);
    StringView method = { 0 };
    Enforce(SVTokenize(&svreq, &method, ' '), "Failed to tokenize method");

    SVTrimLeft(&svreq);
    StringView uri = { 0 };
    Enforce(SVTokenize(&svreq, &uri, ' '), "Failed to tokenize uri");

    SVTrimLeft(&svreq);
    StringView version = { 0 };
    Enforce(SVTokenize(&svreq, &version, '\r'), "Failed to tokenize version");
    Enforce(SVConsume(&svreq, &newLine), "No new line found");

    printf("Method = "SV_FMT"\n", SV_ARGS(method));
    printf("URI = "SV_FMT"\n", SV_ARGS(uri));
    printf("Version = "SV_FMT"\n", SV_ARGS(version));
}

int main()
{
    ParseHTTPRequest();
    return 0;
}
