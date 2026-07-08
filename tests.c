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

    Enforce(
        SVToCString(&a, buf1, sizeof(buf1)) == false
        && SVToCString(&b, buf1, sizeof(buf1)) == true
        && SVToCString(&c, buf3, sizeof(buf3)) == false
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

int main()
{
    TestSV();
    TestSVToCString();
    TestSVCompare();
    TestSVSubString();
    return 0;
}
