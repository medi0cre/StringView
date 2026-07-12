#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STRINGVIEW_IMPLEMENTATION
#include "StringView.h"

StringView a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;

void Enforce(bool Condition, const char* Message)
{
    if (Condition) { return; }
    printf("%s\n", Message);
    exit(EXIT_FAILURE);
}

void TestSV()
{
    a = SV(NULL);
    b = SV("");
    c = SV("abc");

    Enforce(
        a.data == NULL
        && a.size == 0
        && b.data != NULL
        && b.size == 0
        && c.data != NULL
        && c.size == 3,
        "TestSV() failed"
    );

    printf("TestSV() succeeded\n");
}

void TestSVToCString()
{
    a = SV(NULL);
    b = SV("");
    c = SV("abc");

    char buf1[1];
    char buf3[3];
    char buf4[4];
	char b4[4];
	
	SVToCString(&c, b4, sizeof(b4));
    
	Enforce(
        SVToCString(&a, buf1, sizeof(buf1)) == false
        && SVToCString(&b, buf1, sizeof(buf1)) == true
		&& SVToCString(&c, NULL, 4) == false
		&& SVToCString(NULL, buf4, sizeof(buf4)) == false
        && SVToCString(&c, buf3, sizeof(buf3)) == false
		&& strcmp(b4, "abc") == 0
        && SVToCString(&c, buf4, sizeof(buf4)) == true,
        "TestSVToCString() failed"
    );

    printf("TestSVToCString() succeeded\n");
}

void TestSVCompare()
{
    a = SV(NULL);
    b = SV("");
    c = SV("abcdefghij");
    d = SV("abcdefghij");
    e = SV("abcdeFghij");
    f = SV("abcdefghi");

    Enforce(
        !SVCompare(&a, &a)
        && !SVCompare(&a, &b)
        && !SVCompare(&a, &c)
        && SVCompare(&b, &b)
        && !SVCompare(&b, &c)
        && SVCompare(&c, &c)
        && SVCompare(&c, &d)
        && !SVCompare(&c, &e)
        && !SVCompare(NULL, &b)
        && !SVCompare(&b, NULL)
        && !SVCompare(NULL, NULL)
        && !SVCompare(&c, &f),
        "TestSVCompare() failed"
    );

    printf("TestSVCompare() succeeded\n");
}

void TestSVSubString()
{
    a = SV(NULL);
    b = SV("");
    c = SV("abcdefghij");
    d = SVSubString(&a, 0, 0);
    e = SVSubString(&b, 0, 0);
    f = SVSubString(&b, 0, 5);
    g = SVSubString(&c, 11, 0);
    h = SVSubString(&c, 6, 11);
    i = SVSubString(&c, 2, 3);
    j = SVSubString(&c, 0, 10);
    k = SV("ghij");
    l = SV("cde");
    m = SVSubString(&c, 10, 0);
    n = SVSubString(&c, 10, 10);
    o = SVSubString(&c, 4, 0);
    p = SVSubString(&c, 9, 1);
    q = SV("j");
    r = SVSubString(&c, 6, 4);
    s = SVSubString(NULL, 0, 0);

    char buf[] = { 'a','b','c','d','e' };
    t = SVMake(buf, 5);
    u = SVSubString(&t, 1, 3);
    v = SV("bcd");

    Enforce(
        d.data == NULL && d.size == 0
        && e.data == b.data && e.size == 0
        && f.data == b.data && f.size == 0
        && g.data == NULL && g.size == 0
        && SVCompare(&h, &k)
        && SVCompare(&i, &l)
        && SVCompare(&j, &c)
        && m.data == c.data + 10 && m.size == 0
        && n.data == c.data + 10 && n.size == 0
        && SVCompare(&p, &q)
        && SVCompare(&r, &k)
        && r.data == c.data + 6 && r.size == 4
        && i.data == c.data + 2 && i.size == 3
        && s.data == NULL && s.size == 0
        && h.data == c.data + 6 && h.size == 4
        && p.data == c.data + 9 && p.size == 1
        && j.data == c.data && j.size == c.size
        && SVCompare(&v, &u)
        && u.data == buf + 1 && u.size == 3 && SVCompare(&u, &v)
        && o.data == c.data + 4 && o.size == 0,
        "TestSVSubString() failed"
    );

    printf("TestSVSubString() succeeded\n");
}

void TestSVTrimLeft()
{
    a = SV(NULL);
    b = SV("");
    c = SV("     ");
    d = SV("   abc   def   ");
    e = SV("");
    f = SV("abc   def   ");
    g = SV("abcdef");
    h = SV("abcdef");
    i = SV("\r\n\t abc\r\n\t def\r\n\t ");
    j = SV("abc\r\n\t def\r\n\t ");

    SVTrimLeft(&a);
    SVTrimLeft(&b);
    SVTrimLeft(&c);
    SVTrimLeft(&d);
    SVTrimLeft(&g);
    SVTrimLeft(&i);

    Enforce(
        a.data == NULL && a.size == 0
        && SVCompare(&b, &e)
        && SVCompare(&c, &e)
        && SVCompare(&g, &h)
        && SVCompare(&i, &j)
        && SVCompare(&d, &f),
        "TestSVTrimLeft failed"
    );

    printf("TestSVTrimLeft() succeeded\n");
}

void TestSVTrimRight()
{
    a = SV(NULL);
    b = SV("");
    c = SV("     ");
    d = SV("   abc   def   ");
    e = SV("");
    f = SV("   abc   def");
    g = SV("abcdef");
    h = SV("abcdef");
    i = SV("\r\n\t abc\r\n\t def\r\n\t ");
    j = SV("\r\n\t abc\r\n\t def");

    SVTrimRight(&a);
    SVTrimRight(&b);
    SVTrimRight(&c);
    SVTrimRight(&d);
    SVTrimRight(&g);
    SVTrimRight(&i);

    Enforce(
        a.data == NULL && a.size == 0
        && SVCompare(&b, &e)
        && SVCompare(&c, &e)
        && SVCompare(&g, &h)
        && SVCompare(&i, &j)
        && SVCompare(&d, &f),
        "TestSVTrimRight failed"
    );

    printf("TestSVTrimRight() succeeded\n");
}

void TestSVTrim()
{
    a = SV(NULL);
    b = SV("");
    c = SV("     ");
    d = SV("   abc   def   ");
    e = SV("");
    f = SV("abc   def");
    g = SV("abcdef");
    h = SV("abcdef");
    i = SV("\r\n\t abc\r\n\t def\r\n\t ");
    j = SV("abc\r\n\t def");

    SVTrim(&a);
    SVTrim(&b);
    SVTrim(&c);
    SVTrim(&d);
    SVTrim(&g);
    SVTrim(&i);

    Enforce(
        a.data == NULL && a.size == 0
        && SVCompare(&b, &e)
        && SVCompare(&c, &e)
        && SVCompare(&g, &h)
        && SVCompare(&i, &j)
        && SVCompare(&d, &f),
        "TestSVTrim failed"
    );

    printf("TestSVTrim() succeeded\n");
}

void TestSVChopLeft()
{
    a = SV(NULL);
    b = SV("");
    c = SV("abcdef");
    d = SV("abcdef");
    e = SV("abcdef");
    f = SV("");
    g = SV("abcdef");
    h = SV("def");
    i = SV("abcdef");
    
    SVChopLeft(&a, 0);
    SVChopLeft(&b, 5);
    SVChopLeft(&c, 0);
    SVChopLeft(&d, 3);
    SVChopLeft(&e, 6);
    SVChopLeft(&i, 7);
    SVChopLeft(NULL, 5);

    Enforce(
        a.data == NULL && a.size == 0
        && SVCompare(&b, &f)
        && SVCompare(&c, &g)
        && SVCompare(&d, &h)
        && SVCompare(&e, &f)
        && SVCompare(&i, &f),
        "TestSVChopLeft() failed"
    );

    printf("TestSVChopLeft() succeeded\n");
}

void TestSVChopRight()
{
    a = SV(NULL);
    b = SV("");
    c = SV("abcdef");
    d = SV("abcdef");
    e = SV("abcdef");
    f = SV("");
    g = SV("abcdef");
    h = SV("abc");
    i = SV("abcdef");
    
    SVChopRight(&a, 0);
    SVChopRight(&b, 5);
    SVChopRight(&c, 0);
    SVChopRight(&d, 3);
    SVChopRight(&e, 6);
    SVChopRight(&i, 7);
    SVChopRight(NULL, 5);

    Enforce(
        a.data == NULL && a.size == 0
        && SVCompare(&b, &f)
        && SVCompare(&c, &g)
        && SVCompare(&d, &h)
        && SVCompare(&e, &f)
        && SVCompare(&i, &f),
        "TestSVChopRight() failed"
    );

    printf("TestSVChopRight() succeeded\n");
}

void TestSVFindFirstChar()
{
    a = SV(NULL);
    b = SV("");
    c = SV("  ab cd\ref\ng\thijklm \r z    ");

    Enforce(
        SVFindFirstChar(&a, 'a') == SIZE_MAX
        && SVFindFirstChar(&a, ' ') == SIZE_MAX
        && SVFindFirstChar(&b, ' ') == SIZE_MAX
        && SVFindFirstChar(&b, 'z') == SIZE_MAX
        && SVFindFirstChar(&c, 'q') == SIZE_MAX
        && SVFindFirstChar(&c, ' ') == 0
        && SVFindFirstChar(&c, 'a') == 2
        && SVFindFirstChar(&c, '\r') == 7
        && SVFindFirstChar(&c, '\t') == 12
        && SVFindFirstChar(&c, 'z') == 22,
        "TestSVFindFirstChar() failed"
    );

    printf("TestSVFindFirstChar() succeeded\n");
}

void TestSVFindLastChar()
{
    a = SV(NULL);
    b = SV("");
    c = SV("  ab cd\ref\ng\thijklm \r z    ");

    Enforce(
        SVFindLastChar(&a, 'a') == SIZE_MAX
        && SVFindLastChar(&a, ' ') == SIZE_MAX
        && SVFindLastChar(&b, ' ') == SIZE_MAX
        && SVFindLastChar(&b, 'z') == SIZE_MAX
        && SVFindLastChar(&c, 'q') == SIZE_MAX
        && SVFindLastChar(&c, ' ') == 26
        && SVFindLastChar(&c, 'z') == 22
        && SVFindLastChar(&c, '\r') == 20
        && SVFindLastChar(&c, '\t') == 12
        && SVFindLastChar(&c, 'a') == 2,
        "TestSVFindLastChar() failed"
    );

    printf("TestSVFindLastChar() succeeded\n");
}

void TestSVMake()
{
    char aa[] = { 0 };
    char bb[] = { '\0' };
    char cc[] = { 'a', 'b', 'c' };
    char dd[] = { '\0', 'a', 'b', 'c' };
    char ee[] = { 'a', 'b', '\0', 'c' };
    char ff[] = { 'a', 'b', 'c', '\0' };
    char gg[] = { 'a', 'b', 'c', 'd', 'e' }; 

    a = SVMake(aa, sizeof(aa));
    b = SVMake(bb, sizeof(bb));
    c = SVMake(cc, sizeof(cc));
    d = SVMake(dd, sizeof(dd));
    e = SVMake(ee, sizeof(ee));
    f = SVMake(ff, sizeof(ff));
    g = SVMake(NULL, 0);
    h = SVMake(NULL, 1);
    i = SVMake(cc, 0);
    j = SVMake(gg + 2, 2);

    Enforce(
        a.data != NULL && a.size == 1
        && b.data != NULL && b.size == 1
        && memcmp(c.data, "abc", 3) == 0 && c.size == 3
        && memcmp(d.data, "\0abc", 4) == 0 && d.size == 4
        && memcmp(e.data, "ab\0c", 4) == 0 && e.size == 4
        && g.data == NULL && g.size == 0
        && h.data == NULL && h.size == 0
        && i.data == cc && i.size == 0
        && j.data == gg + 2 && j.size == 2 && memcmp(j.data, "cd", 2) == 0
        && memcmp(f.data, "abc\0", 4) == 0 && f.size == 4,
        "TestSVMake() failed"
    );

    printf("TestSVMake() succeeded\n");
}

void TestSVStartsWith()
{
    a = SV(NULL);
    b = SV("");
    c = SV("abcde");
    d = SV("a");
    e = SV("abc");
    f = SV("abcde");

    Enforce(
        !SVStartsWith(&a, &a)
        && !SVStartsWith(&a, &b)
        && !SVStartsWith(&a, &d)
        && !SVStartsWith(&b, &a)
        && SVStartsWith(&b, &b)
        && !SVStartsWith(&b, &d)
        && !SVStartsWith(&c, &a)
        && SVStartsWith(&c, &b)
        && SVStartsWith(&c, &d)
        && SVStartsWith(&c, &e)
        && SVStartsWith(&c, &f)
        , "TestSVStartsWith() failed"
    );

    printf("TestSVStartsWith() succeeded\n");
}

void TestSVEndsWith()
{
    a = SV(NULL);
    b = SV("");
    c = SV("abcde");
    d = SV("e");
    e = SV("cde");
    f = SV("abcde");

    Enforce(
        !SVEndsWith(&a, &a)
        && !SVEndsWith(&a, &b)
        && !SVEndsWith(&a, &d)
        && !SVEndsWith(&b, &a)
        && SVEndsWith(&b, &b)
        && !SVEndsWith(&b, &d)
        && !SVEndsWith(&c, &a)
        && SVEndsWith(&c, &b)
        && SVEndsWith(&c, &d)
        && SVEndsWith(&c, &e)
        && SVEndsWith(&c, &f)
        , "TestSVEndsWith() failed"
    );

    printf("TestSVEndsWith() succeeded\n");
}

void TestSVFind()
{
    a = SV(NULL);
    b = SV("");
    c = SV("abcde");
    d = SV("a");
    e = SV("abc");
    f = SV("abcde");
    g = SV("bc");
    h = SV("bcd");
    i = SV("bcde");
    j = SV("c");
    k = SV("cd");
    l = SV("cde");
    m = SV("z");
    n = SV("def");
    o = SV("aaaaa");
    p = SV("aa");
    s = SV("e");

    char buf[] = { 'a', '\0', 'b', '\0', 'c' };
    q = SVMake(buf, sizeof(buf));
    r = SVMake(buf + 1, 2);

    Enforce(
        SVFind(&a, &a) == SIZE_MAX
        && SVFind(&a, &b) == SIZE_MAX
        && SVFind(&a, &c) == SIZE_MAX
        && SVFind(&b, &a) == SIZE_MAX
        && SVFind(&b, &b) == 0
        && SVFind(&b, &c) == SIZE_MAX
        && SVFind(&c, &a) == SIZE_MAX
        && SVFind(&c, &b) == 0
        && SVFind(&c, &d) == 0 && SVFind(&c, &e) == 0 && SVFind(&c, &f) == 0
        && SVFind(&c, &g) == 1 && SVFind(&c, &h) == 1 && SVFind(&c, &i) == 1
        && SVFind(&c, &j) == 2 && SVFind(&c, &k) == 2 && SVFind(&c, &l) == 2
        && SVFind(&c, &m) == SIZE_MAX
        && SVFind(&c, &n) == SIZE_MAX
        && SVFind(&o, &p) == 0
        && SVFind(&q, &r) == 1
        && SVFind(&c, &s) == 4
        , "TestSVFind() failed"
    );

    printf("TestSVFind() succeeded\n");
}

int main()
{
    TestSV();
    TestSVToCString();
    TestSVCompare();
    TestSVSubString();
    TestSVTrimLeft();
    TestSVTrimRight();
    TestSVTrim();
    TestSVChopLeft();
    TestSVChopRight();
    TestSVFindFirstChar();
    TestSVFindLastChar();
    TestSVMake();
    TestSVStartsWith();
    TestSVEndsWith();
    TestSVFind();
    return 0;
}
