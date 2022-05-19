#ifndef __ATS_STRING_H__
#define __ATS_STRING_H__

struct string {
    const usize size;
    const char* data;
};

#define STR_FMT "%.*s"
#define STR_ARG(s) (int)(s).size, (s).data

#define string(text) (struct string) { sizeof (text) - 1, text }
#define string_const(text) { sizeof (text) - 1, text }

#endif // __ATS_STRING_H__

#ifdef ATS_IMPL

//

#endif

