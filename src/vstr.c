#include "vlib.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static vstr_lg vstr_make_lg(const char* data);
static vstr_lg vstr_make_lg_len(const char* data, size_t len);
static int vstr_sm_push_char(vstr_sm* sm, char c, uint8_t avail);
static int vstr_lg_push_char(vstr_lg* lg, char c);
static int vstr_realloc_lg(vstr_lg* lg, size_t len, size_t cap);

vstr vstr_new(void) {
    vstr s = {0};
    s.is_large = 0;
    s.small_avail = VSTR_MAX_SMALL_SIZE;
    return s;
}

vstr vstr_from(const char* cstr) {
    vstr s = {0};
    size_t cstr_len = strlen(cstr);
    if (cstr_len > VSTR_MAX_SMALL_SIZE) {
        s.str_data.lg = vstr_make_lg_len(cstr, cstr_len);
        s.is_large = 1;
        return s;
    }
    memcpy(s.str_data.sm.data, cstr, cstr_len);
    s.small_avail = VSTR_MAX_SMALL_SIZE - cstr_len;
    return s;
}

size_t vstr_len(vstr* s) {
    if (s->is_large) {
        return s->str_data.lg.len;
    }
    return VSTR_MAX_SMALL_SIZE - s->small_avail;
}

char* vstr_data(vstr* s) {
    if (s->is_large) {
        return s->str_data.lg.data;
    }
    return s->str_data.sm.data;
}

int vstr_cmp(vstr* a, vstr* b) {
    size_t alen = vstr_len(a);
    size_t blen = vstr_len(b);
    char* astr = vstr_data(a);
    char* bstr = vstr_data(b);
    if (alen == blen) {
        return strncmp(astr, bstr, alen);
    }
    if (alen < blen) {
        int cmp = strncmp(astr, bstr, alen);
        if (cmp == 0) {
            return 1;
        }
        return cmp;
    } else {
        int cmp = strncmp(astr, bstr, blen);
        if (cmp == 0) {
            return -1;
        }
        return cmp;
    }
}

int vstr_push_char(vstr* s, char c) {
    int push_res;
    if (s->is_large) {
        return vstr_lg_push_char(&(s->str_data.lg), c);
    }
    push_res = vstr_sm_push_char(&(s->str_data.sm), c, s->small_avail);
    if (push_res == 0) {
        s->small_avail--;
        return 0;
    }
    s->str_data.lg = vstr_make_lg(s->str_data.sm.data);
    s->is_large = 1;
    return vstr_lg_push_char(&(s->str_data.lg), c);
}

void vstr_free(vstr* s) {
    if (s->is_large) {
        free(s->str_data.lg.data);
    }
}

static vstr_lg vstr_make_lg(const char* data) {
    vstr_lg lg = {0};
    lg.len = VSTR_MAX_SMALL_SIZE;
    lg.cap = VSTR_MAX_SMALL_SIZE + 2;
    lg.data = calloc(lg.cap, sizeof(char));
    assert(lg.data != NULL);
    memcpy(lg.data, data, VSTR_MAX_SMALL_SIZE);
    return lg;
}

static vstr_lg vstr_make_lg_len(const char* data, size_t len) {
    vstr_lg lg = {0};
    assert(len < VSTR_MAX_LARGE_SIZE);
    lg.len = len;
    lg.cap = len + 1;
    lg.data = calloc(lg.cap, sizeof(char));
    assert(lg.data != NULL);
    memcpy(lg.data, data, len);
    return lg;
}

static int vstr_sm_push_char(vstr_sm* sm, char c, uint8_t avail) {
    uint8_t ins;
    if (avail <= 0) {
        return -1;
    }
    ins = VSTR_MAX_SMALL_SIZE - avail;
    sm->data[ins] = c;
    return 0;
}

static int vstr_lg_push_char(vstr_lg* lg, char c) {
    size_t len = lg->len, cap = lg->cap;
    if (len == (cap - 1)) {
        int realloc_res = vstr_realloc_lg(lg, len, cap);
        if (realloc_res == -1) {
            return -1;
        }
    }
    if ((len + 1) > VSTR_MAX_LARGE_SIZE) {
        return -1;
    }
    lg->data[len] = c;
    lg->len++;
    return 0;
}

static int vstr_realloc_lg(vstr_lg* lg, size_t len, size_t cap) {
    void* tmp;
    cap <<= 1;
    tmp = realloc(lg->data, cap);
    if (tmp == NULL) {
        return -1;
    }
    lg->data = tmp;
    memset(lg->data + len, 0, cap - len);
    lg->cap = cap;
    return 0;
}
