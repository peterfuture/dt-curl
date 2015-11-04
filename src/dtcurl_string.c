/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_string.c
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015��10��30�� 09ʱ41��28��
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s
 *    Email      :  peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "dtcurl_utils.h"

int dtcurl_strstart(const char *str, const char *pfx, const char **ptr)
{
    while (*pfx && *pfx == *str) {
        pfx++;
        str++;
    }
    if (!*pfx && ptr) {
        *ptr = str;
    }
    return !*pfx;
}

int dtcurl_stristart(const char *str, const char *pfx, const char **ptr)
{
    while (*pfx && toupper((unsigned)*pfx) == toupper((unsigned)*str)) {
        pfx++;
        str++;
    }
    if (!*pfx && ptr) {
        *ptr = str;
    }
    return !*pfx;
}

char *dtcurl_stristr(const char *s1, const char *s2)
{
    if (!*s2) {
        return (char*)(intptr_t)s1;
    }

    do
        if (dtcurl_stristart(s1, s2, NULL)) {
            return (char*)(intptr_t)s1;
        }
    while (*s1++);

    return NULL;
}

char *dtcurl_strnstr(const char *haystack, const char *needle, size_t hay_length)
{
    size_t needle_len = strlen(needle);
    if (!needle_len) {
        return (char*)haystack;
    }
    while (hay_length >= needle_len) {
        hay_length--;
        if (!memcmp(haystack, needle, needle_len)) {
            return (char*)haystack;
        }
        haystack++;
    }
    return NULL;
}

size_t dtcurl_strlcpy(char *dst, const char *src, size_t size)
{
    size_t len = 0;
    while (++len < size && *src) {
        *dst++ = *src++;
    }
    if (len <= size) {
        *dst = 0;
    }
    return len + strlen(src) - 1;
}

size_t dtcurl_strlcat(char *dst, const char *src, size_t size)
{
    size_t len = strlen(dst);
    if (size <= len + 1) {
        return len + strlen(src);
    }
    return len + dtcurl_strlcpy(dst + len, src, size - len);
}

size_t dtcurl_strlcatf(char *dst, size_t size, const char *fmt, ...)
{
    size_t len = strlen(dst);
    va_list vl;

    va_start(vl, fmt);
    len += vsnprintf(dst + len, size > len ? size - len : 0, fmt, vl);
    va_end(vl);

    return len;
}

char *dtcurl_asprintf(const char *fmt, ...)
{
    char *p = NULL;
    va_list va;
    int len;

    va_start(va, fmt);
    len = vsnprintf(NULL, 0, fmt, va);
    va_end(va);
    if (len < 0) {
        goto end;
    }

    p = dtcurl_malloc(len + 1);
    if (!p) {
        goto end;
    }

    va_start(va, fmt);
    len = vsnprintf(p, len + 1, fmt, va);
    va_end(va);
    if (len < 0) {
        free(p);
    }

end:
    return p;
}

char *dtcurl_d2str(double d)
{
    char *str = dtcurl_malloc(16);
    if (str) {
        snprintf(str, 16, "%f", d);
    }
    return str;
}

#define WHITESPACES " \n\t"

char *dtcurl_get_token(const char **buf, const char *term)
{
    char *out     = dtcurl_malloc(strlen(*buf) + 1);
    char *ret     = out, *end = out;
    const char *p = *buf;
    if (!out) {
        return NULL;
    }
    p += strspn(p, WHITESPACES);

    while (*p && !strspn(p, term)) {
        char c = *p++;
        if (c == '\\' && *p) {
            *out++ = *p++;
            end    = out;
        } else if (c == '\'') {
            while (*p && *p != '\'') {
                *out++ = *p++;
            }
            if (*p) {
                p++;
                end = out;
            }
        } else {
            *out++ = c;
        }
    }

    do {
        *out-- = 0;
    } while (out >= end && strspn(out, WHITESPACES));

    *buf = p;

    return ret;
}

char *dtcurl_strtok(char *s, const char *delim, char **saveptr)
{
    char *tok;

    if (!s && !(s = *saveptr)) {
        return NULL;
    }

    /* skip leading delimiters */
    s += strspn(s, delim);

    /* s now points to the first non delimiter char, or to the end of the string */
    if (!*s) {
        *saveptr = NULL;
        return NULL;
    }
    tok = s++;

    /* skip non delimiters */
    s += strcspn(s, delim);
    if (*s) {
        *s = 0;
        *saveptr = s + 1;
    } else {
        *saveptr = NULL;
    }

    return tok;
}

int dtcurl_strcasecmp(const char *a, const char *b)
{
    uint8_t c1, c2;
    do {
        c1 = tolower(*a++);
        c2 = tolower(*b++);
    } while (c1 && c1 == c2);
    return c1 - c2;
}

int dtcurl_strncasecmp(const char *a, const char *b, size_t n)
{
    const char *end = a + n;
    uint8_t c1, c2;
    do {
        c1 = tolower(*a++);
        c2 = tolower(*b++);
    } while (a < end && c1 && c1 == c2);
    return c1 - c2;
}
