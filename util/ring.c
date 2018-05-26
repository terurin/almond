#include "ring.h"

static const char newline = '\r';

void ring2_init(ring2_t* obj, uint8_t*mem, uint16_t log2) {
    if (obj == NULL)return;
    obj->buf = (uint8_t*) mem;
    obj->size = 1 << log2;
    obj->mask = (1 << log2) - 1;
    ring2_clean(obj);
    obj->tag = 0;
}

uint16_t ring2_find(const ring2_t *obj, char c) {
    uint16_t pos;
    uint16_t end = ring2_used(obj);
    for (pos = 0; pos < end; pos++) {
        if (ring2_at(obj, pos) == c) {
            return pos;
        }
    }
    return pos;
}

char ring2_putc(ring2_t* obj, char c) {
    if (obj == NULL)return '\0';
    if (obj->used + 1 < obj->size) {
        obj->buf[obj->in] = c;
        obj->in = ((obj->in + 1) & obj->mask);
        obj->used++;
        return c;
    } else {
        return '\0';
    }
}

char ring2_getc(ring2_t* obj) {
    if (obj == NULL)return 0;
    if (obj->used > 0) {
        char c;
        c = obj->buf[obj->out];
        obj->out = ((obj->out + 1) & obj->mask);
        obj->used--;
        return c;
    } else {
        return 0;
    }
}

const uint8_t* ring2_write(ring2_t* obj, const uint8_t* mem, size_t sz) {
    if (obj == NULL)return NULL;
    bool writable = ring2_free(obj) - sz > 0;
    if (writable) {
        uint16_t pos = obj->in;
        const uint8_t* it = mem;
        //先に更新する
        obj->in = (obj->in + sz) & obj->mask;
        obj->used += sz;
        //コピー
        for (; sz > 0; sz--, it++) {
            obj->buf[pos] = *it;
            pos = (pos + 1) & obj->mask;
        }
        return mem;
    } else {
        return NULL;
    }
}

const char* ring2_puts(ring2_t* obj, const char* str) {
    const char * result = (const char*) ring2_write(obj, (const uint8_t*) str, strlen(str));
    return result;
}

const char* ring2_putl(ring2_t* obj, const char* str) {
    const char * result = (const char*) ring2_write(obj, (const uint8_t*) str, strlen(str));
    ring2_putc(obj,newline);
    return result;
}

uint8_t* ring2_read(ring2_t* obj, uint8_t* mem, size_t sz) {
    if (obj == NULL)return NULL;
    bool readable = ring2_used(obj) - sz > 0;
    if (readable) {
        uint16_t pos = obj->out;
        uint8_t* it = mem;
        //先に更新する
        obj->out = (obj->out + sz) & obj->mask;
        obj->used -= sz;
        //コピー
        for (; sz > 0; sz--, it++) {
            *it = obj->buf[pos];
            pos = (pos + 1) & obj->mask;
        }
        return mem;
    } else {
        return NULL;
    }
}

char* ring2_gets(ring2_t* obj, char *mem, size_t sz) {
    uint16_t pos = ring2_find(obj, newline);
    if (pos < ring2_used(obj) && pos + 2 < sz) {
        ring2_read(obj, (uint8_t*) mem, pos + 1);
        mem[pos + 1] = '\0';
        return mem;
    }
    return NULL;
}