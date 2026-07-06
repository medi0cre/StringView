#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define SV_FMT "%.*s"
#define SV_ARGS(sv) (int)(sv).size, (sv).data

typedef struct StringView
{
    const char* data;
    size_t size;
} StringView;

void Enforce(bool Condition, const char* Message)
{
    if (Condition) { return; }
    printf("%s\n", Message);
    exit(EXIT_FAILURE);
}

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

int main()
{
    // Tests
    StringView TestSV = SV("The quick brown fox jumps over the lazy dog");
    StringView NullSV = SV(NULL);
    StringView BlankSV = SV("");

    Enforce(TestSV.size == 43 && strncmp(TestSV.data, "The quick brown fox jumps over the lazy dog", TestSV.size) == 0, "Test 1 failed");
    Enforce(NullSV.data == NULL && NullSV.size == 0, "Test 2 failed");
    Enforce(BlankSV.size == 0 && strncmp(BlankSV.data, "", BlankSV.size) == 0, "Test 3 failed");
    
    char SmallBuffer[8];
    char BigBuffer[64];

    Enforce(SVToCString(&TestSV, SmallBuffer, 8) == false, "Test 4 failed");
    Enforce(SVToCString(&TestSV, BigBuffer, 64) == true, "Test 5 failed");
    Enforce(SVToCString(&NullSV, SmallBuffer, 8) == false, "Test 6 failed");
    Enforce(SVToCString(&NullSV, BigBuffer, 64) == false, "Test 7 failed");
    Enforce(SVToCString(&BlankSV, SmallBuffer, 8) == true, "Test 8 failed");
    Enforce(SVToCString(&BlankSV, BigBuffer, 64) == true, "Test 9 failed");

    Enforce(SVSubString(&TestSV, 4, 39).size == 39 && strncmp(SVSubString(&TestSV, 4, 39).data, "quick brown fox jumps over the lazy dog", 39) == 0,"Test 10 failed");
    Enforce(SVSubString(&TestSV, 4, 40000).size == 39
            && strncmp(SVSubString(&TestSV, 4, 40000).data, "quick brown fox jumps over the lazy dog", 39) == 0,
            "Test 11 failed");
 
    char ExactBuffer[43];
    char MinBuffer[44];
    Enforce(SVToCString(&TestSV, MinBuffer, sizeof(MinBuffer)) == true
        && strcmp(MinBuffer, "The quick brown fox jumps over the lazy dog") == 0,
        "Test 12 failed");
    Enforce(SVToCString(&TestSV, ExactBuffer, sizeof(ExactBuffer)) == false, "Test 13 failed");   
    Enforce(SVToCString(NULL, BigBuffer, sizeof(BigBuffer)) == false,
        "Test 14 failed");
    Enforce(SVToCString(&TestSV, NULL, 64) == false,
        "Test 15 failed");
    Enforce(SVToCString(&TestSV, BigBuffer, 0) == false,
        "Test 16 failed");
    
    BigBuffer[0] = 'X';
    Enforce(SVToCString(&BlankSV, BigBuffer, sizeof(BigBuffer))
        && BigBuffer[0] == '\0',
        "Test 17 failed");
    
    StringView s = SVSubString(&TestSV, 0, 3);
    Enforce(s.size == 3 && strncmp(s.data, "The", 3) == 0,
        "Test 18 failed");
    
    StringView s2 = SVSubString(&TestSV, 40, 3);
    Enforce(s2.size == 3 && strncmp(s2.data, "dog", 3) == 0,
        "Test 19 failed");
    
    StringView s3 = SVSubString(&TestSV, TestSV.size, 5);
    Enforce(s3.size == 0 && s3.data == TestSV.data + TestSV.size,
        "Test 20 failed");
    
    StringView s4 = SVSubString(&TestSV, TestSV.size + 1, 5);
    Enforce(s4.size == 0 && s4.data == NULL,
        "Test 21 failed");

    StringView s5 = SVSubString(&TestSV, 10, 0);
    Enforce(s5.size == 0 && s5.data == TestSV.data + 10,
        "Test 22 failed");

    StringView s6 = SVSubString(NULL, 0, 1);
    Enforce(s6.data == NULL && s6.size == 0,
        "Test 23 failed");

    StringView s7 = SVSubString(&BlankSV, 0, 10);
    Enforce(s7.size == 0 && s7.data == BlankSV.data,
        "Test 24 failed");

    char text[] = "hello";
    StringView s8 = SV(text);
    text[0] = 'H';

    Enforce(strncmp(s8.data, "Hello", 5) == 0,
        "Test 25 failed");

    StringView A = SV("Hello");
    StringView AA = SV("Hello");
    StringView B = SV("Hello ");
    StringView C = SV("HellO");

    Enforce(!SVCompare(&A, &B), "Test 26 failed");
    Enforce(!SVCompare(&A, &C), "Test 27 failed");
    Enforce(SVCompare(&A, &AA), "Test 28 failed");
    
    StringView Full = SV(":abcdefghijklmnop");
    StringView Left = { 0 };
    StringView Right = { 0 };
    StringView LeftExpected = SV("");
    StringView RightExpected = SV("abcdefghijklmnop");
    Enforce(SVSplitByDelimiter(&Full, ':', &Left, &Right) == true, "Test 29 failed");
    Enforce(SVCompare(&Right, &RightExpected) == true, "Test 30 failed");
    Enforce(SVCompare(&Left, &LeftExpected) == true, "Test 31 failed");

    StringView Spaced = SV("    Anirban Mistry    ");
    StringView Expected = SV("Anirban Mistry    ");
    SVTrimLeft(&Spaced);
    Enforce(SVCompare(&Spaced, &Expected), "Test 32 failed");
    SVTrimRight(&Spaced);
    Expected = SV("Anirban Mistry");
    Enforce(SVCompare(&Spaced, &Expected), "Test 33 failed");
    
    StringView a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r;

    a = SV("Anirban Mistry");
    b = SV("Anirban Mis");
    SVChopRight(&a, 3);
    Enforce(SVCompare(&a, &b), "Test failed");

    a = SV("Anirban Mistry");
    Enforce(SVFindFirstChar(&a, 'b') == 4, "Test failed");
    Enforce(SVFindFirstChar(&a, 'o') == SIZE_MAX, "Test failed");

    a = SV("Anirban Liam Zaid Sami Patrick");
    c = SV("Anirban");
    d = SV("Liam Zaid Sami Patrick");
    Enforce(SVTokenize(&a, &b, ' ') == true, "Test failed");
    Enforce(SVCompare(&b, &c), "Test failed");
    Enforce(SVCompare(&a, &d), "Test failed");
    Enforce(SVTokenize(&a, &b, ' ') == true, "Test failed");
    c = SV("Liam");
    d = SV("Zaid Sami Patrick");
    Enforce(SVCompare(&b, &c), "Test failed");
    Enforce(SVCompare(&a, &d), "Test failed");
    Enforce(SVTokenize(&a, &b, ' ') == true, "Test failed");
    c = SV("Zaid");
    d = SV("Sami Patrick");
    Enforce(SVCompare(&b, &c), "Test failed");
    Enforce(SVCompare(&a, &d), "Test failed");
    Enforce(SVTokenize(&a, &b, ' ') == true, "Test failed");
    c = SV("Sami");
    d = SV("Patrick");
    Enforce(SVCompare(&b, &c), "Test failed");
    Enforce(SVCompare(&a, &d), "Test failed");
    Enforce(SVTokenize(&a, &b, ' ') == true, "Test 1failed");
    c = SV("Patrick");
    d = SV("");
    Enforce(SVCompare(&b, &c), "Test 2failed");
    Enforce(SVCompare(&a, &d), "Test 3failed");
    Enforce(SVTokenize(&a, &b, ' ') == false, "Test 1failed");
    c = SV("");
    d = SV("");
    Enforce(SVCompare(&a, &d), "Test 3failed");

    a = SV("Anirban Mistry");
    b = SV("Ani");
    c = SV("Anirban Mistryaaa");
    d = SV("anirban Mistry");
    Enforce(SVStartsWith(&a, &b), "Test failed");
    Enforce(!SVStartsWith(&a, &c), "Test failed");
    Enforce(!SVStartsWith(&d, &b), "Test failed");

    a = SV("Anirban Mistry");
    b = SV("try");
    c = SV("rrrAnirban Mistry");
    d = SV("Anirban MistRy");
    Enforce(SVEndsWith(&a, &b), "Test 1 failed");
    Enforce(!SVEndsWith(&a, &c), "Test 2 failed");
    Enforce(!SVEndsWith(&d, &b), "Test 3 failed");

    a = SV("abcdefghij");
    b = SV("cdef");
    c = SV("abcdefghijklmn");
    d = SV("abcd");
    e = SV("efghij");
    Enforce(!SVConsume(&a, &b), "Test failed");
    Enforce(!SVConsume(&a, &c), "Test failed");
    Enforce(SVConsume(&a, &d), "Test failed");
    Enforce(SVCompare(&a, &e), "Test failed");

    a = SV("www.google.com");
    b = SV("wwwgooglecom");
    c = SV("");
    d = SV(NULL);
    Enforce(SVFindLastChar(&a, '.') == 10, "Test failed");
    Enforce(SVFindLastChar(&b, '.') == SIZE_MAX, "Test failed");
    Enforce(SVFindLastChar(&c, '.') == SIZE_MAX, "Test failed");
    Enforce(SVFindLastChar(&d, '.') == SIZE_MAX, "Test failed");

    a = SV("Anirban Mistry");
    b = SV("ist");
    c = SV("try");
    d = SV("IST");
    e = SV("Ani");
    f = SV("");
    Enforce(SVFind(&a, &b) == 9, "Test 1 failed");
    Enforce(SVFind(&a, &c) == 11, "Test 2 failed");
    Enforce(SVFind(&a, &d) == SIZE_MAX, "Test 3 failed");
    Enforce(SVFind(&a, &e) == 0, "Test 4 failed");
    Enforce(SVFind(&a, &f) == 0, "Test 4 failed");

    a = SV("       ");
    SVTrim(&a);
    Enforce(SVCompare(&a, &(StringView){ "", 0 }), "Test failed");

    a = SV("a:b:c:d:e");
    printf("This should print a, b, c, d, e\n\n");
    while(SVTokenize(&a, &b, ':'))
    {
        printf("The token is "SV_FMT"\n", SV_ARGS(b));
    }

    a = SV("a::b");
    printf("\n\nThis should print a, {blank line}, b\n\n");
    while(SVTokenize(&a, &b, ':'))
    {
        printf("The token is "SV_FMT"\n", SV_ARGS(b));
    }

    return 0;
}
