#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define SV_FMT "%.*s"
#define SV_ARGS(sv) (int)(sv).size, (sv).data

typedef struct StringView
{
    const char* data;
    size_t size;
} StringView;


StringView SV(const char* str);
bool SVToCString(const StringView* sv, char* buf, size_t size);
StringView SVSubString(const StringView* sv, size_t pos, size_t size);
bool SVCompare(const StringView* sv1, const StringView* sv2);
bool SVSplitByDelimiter(const StringView* sv, const char delim, StringView* left, StringView* right);
void SVTrimLeft(StringView* sv);
void SVTrimRight(StringView* sv);
void SVChopLeft(StringView* sv, size_t count);
void SVChopRight(StringView* sv, size_t count);
void SVTrim(StringView* sv);
size_t SVFindFirstChar(const StringView* sv, const char c);
size_t SVFindLastChar(const StringView* sv, const char c);
bool SVTokenize(StringView* sv, StringView* token, const char delim);
bool SVStartsWith(const StringView* sv, const StringView* prefix);
bool SVEndsWith(const StringView* sv, const StringView* suffix);
bool SVConsume(StringView* sv, const StringView* prefix);
StringView SVMake(const char* data, size_t size);
size_t SVFind(const StringView* sv, const StringView* substr);

#ifdef STRINGVIEW_IMPLEMENTATION
StringView SV(const char* str)
{
    StringView sv = { 0 };
    if (str == NULL) { return sv; }

    sv.data = str;
    sv.size = strlen(str);
    return sv;
}

bool SVToCString(const StringView* sv, char* buf, size_t size)
{
    if (sv == NULL
        || sv->data == NULL
        || buf == NULL
        || size <= sv->size) { return false; }
    
    memcpy(buf, sv->data, sv->size);
    buf[sv->size] = '\0';
    return true;
}

StringView SVSubString(const StringView* sv, size_t pos, size_t size)
{
    StringView s = { 0 };
    if (sv == NULL
        || sv->data == NULL
        || pos > sv->size) { return s; }
    if (size > sv->size - pos) { size = sv->size - pos; }

    s.data = sv->data + pos;
    s.size = size;
    return s;
}

bool SVCompare(const StringView* sv1, const StringView* sv2)
{
    if (sv1 == NULL
        || sv2 == NULL
        || sv1->data == NULL
        || sv2->data == NULL
        || sv1->size != sv2->size) { return false; }
    
    return memcmp(sv1->data, sv2->data, sv1->size) == 0;
}

bool SVSplitByDelimiter(const StringView* sv, const char delim, StringView* left, StringView* right)
{
    if (sv == NULL
        || sv->data == NULL
        || left == NULL
        || right == NULL) { return false; }

    for (size_t i = 0; i < sv->size; i++)
    {
        if (sv->data[i] == delim)
        {
            left->data = sv->data;
            left->size = i;

            right->data = sv->data + i + 1;
            right->size = sv->size - i - 1;

            return true;
        }
    }
    
    left->data = sv->data;
    left->size = sv->size;

    right->data = sv->data + sv->size;
    right->size = 0;

    return false;
}

void SVTrimLeft(StringView* sv)
{
    if (sv == NULL || sv->data == NULL) { return; }

    while (sv->size > 0 && isspace((unsigned char)sv->data[0]))
    {
        sv->data += 1;
        sv->size -= 1;
    }
}

void SVTrimRight(StringView* sv)
{
    if (sv == NULL || sv->data == NULL) { return; }

    while (sv->size > 0 && isspace((unsigned char)sv->data[sv->size - 1]))
    {
        sv->size -= 1;
    }
}

void SVChopLeft(StringView* sv, size_t count)
{
    if (sv == NULL || sv->data == NULL) { return; }
    if (count > sv->size) { count = sv->size; }

    sv->data += count;
    sv->size -= count;
}

void SVChopRight(StringView* sv, size_t count)
{
    if (sv == NULL || sv->data == NULL) { return; }
    if (count > sv->size) { count = sv->size; }

    sv->size -= count;
}

void SVTrim(StringView* sv)
{
    SVTrimLeft(sv);
    SVTrimRight(sv);
}

size_t SVFindFirstChar(const StringView* sv, const char c)
{
    if (sv == NULL || sv->data == NULL) { return SIZE_MAX; }

    for (size_t i = 0; i < sv->size; i++)
    {
        if (sv->data[i] == c) { return i; }
    }

    return SIZE_MAX;
}

size_t SVFindLastChar(const StringView* sv, const char c)
{
    if (sv == NULL || sv->data == NULL) { return SIZE_MAX; }

    for (size_t i = sv->size; i-- > 0; )
    {
        if (sv->data[i] == c) { return i; }
    }

    return SIZE_MAX;
}

bool SVTokenize(StringView* sv, StringView* token, const char delim)
{
    if (sv == NULL || sv->data == NULL || token == NULL || sv->size == 0) { return false; }

    for (size_t i = 0; i < sv->size; i++)
    {
        if (sv->data[i] == delim)
        {
            token->data = sv->data;
            token->size = i;

            sv->data += i + 1;
            sv->size -= (i + 1);

            return true;
        }
    }

    token->data = sv->data;
    token->size = sv->size;

    sv->data += sv->size;
    sv->size = 0;

    return true;
}

bool SVStartsWith(const StringView* sv, const StringView* prefix)
{
    if (sv == NULL
        || sv->data == NULL
        || prefix == NULL
        || prefix->data == NULL
        || prefix->size > sv->size) { return false; }

    return memcmp(sv->data, prefix->data, prefix->size) == 0;
}

bool SVEndsWith(const StringView* sv, const StringView* suffix)
{
    if (sv == NULL
        || sv->data == NULL
        || suffix == NULL
        || suffix->data == NULL
        || suffix->size > sv->size) { return false; }

    return memcmp(sv->data + sv->size - suffix->size, suffix->data, suffix->size) == 0;
}

bool SVConsume(StringView* sv, const StringView* prefix)
{
    if (sv == NULL
        || sv->data == NULL
        || prefix == NULL
        || prefix->data == NULL
        || prefix->size > sv->size) { return false; }

    if (memcmp(sv->data, prefix->data, prefix->size) != 0) { return false; }
    else
    {
        sv->data += prefix->size;
        sv->size -= prefix->size;
        return true;
    }
}

StringView SVMake(const char* data, size_t size)
{
    StringView sv = { 0 };
    sv.data = data;
    sv.size = size;
    return sv;
}

size_t SVFind(const StringView* sv, const StringView* substr)
{
    if (sv == NULL
        || sv->data == NULL
        || substr == NULL
        || substr->data == NULL
        || sv->size < substr->size) { return SIZE_MAX; }
    if (substr->size == 0) { return 0; }

    for (size_t i = 0; i <= sv->size - substr->size; i++)
    {
        if (sv->data[i] != substr->data[0]) { continue; }
        if (memcmp(sv->data + i, substr->data, substr->size) == 0)
        {
            return i;
        }
    }

    return SIZE_MAX;
}
#endif