#include "ring.h"

static const char newline = '\r';

void ring2_init(ring2_ptr obj, uint8_t*mem, uint16_t log2) {
    if (obj == NULL)return;
    obj->buf = (uint8_t*) mem;
    obj->size = 1 << log2;
    obj->mask = (1 << log2) - 1;
    obj->in = obj->out = 0;
    obj->used = 0;
    obj->tag = 0;
}

uint16_t ring2_find(const ring2_ptr obj, char c) {
    uint16_t pos;
    uint16_t end = ring2_used(obj);
    for (pos = 0; pos < end; pos++) {
        if (ring2_at(obj, pos) == c) {
            return pos;
        }
    }
    return pos;
}

char ring2_putc(ring2_ptr obj, char c) {
    if (obj == NULL)return '\0';
    if ((obj->used + 1) < obj->size) {
        obj->buf[obj->in] = c;
        obj->in = ((obj->in + 1) & obj->mask);
        obj->used++;
        return c;
    } else {
        return '\0';
    }
}

char ring2_getc(ring2_ptr obj) {
    if (obj != NULL && obj->used > 0) {
        char c;
        c = obj->buf[obj->out];
        obj->out = ((obj->out + 1) & obj->mask);
        obj->used--;
        return c;
    } else {
        return 0;
    }
}

const uint8_t* ring2_write(ring2_ptr obj, const uint8_t* mem, size_t sz) {
    if (obj == NULL)return NULL;
    bool writable = (obj->used + sz) < obj->size;
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

const char* ring2_puts(ring2_ptr obj, const char* str) {
    const char * result = (const char*) ring2_write(obj, (const uint8_t*) str, strlen(str));
    return result;
}

const char* ring2_putl(ring2_ptr obj, const char* str) {
    const char * result = (const char*) ring2_write(obj, (const uint8_t*) str, strlen(str));
    if (result != NULL)ring2_putc(obj, newline);
    return result;
}

int ring2_putw(ring2_ptr ring, int w) {
    if (ring == NULL)return 0; //Error
    if (ring2_writable(ring, sizeof (w))) {
        uint16_t end;
        uint16_t pos = ring->in;
        unsigned int tmp = (unsigned int) w;
        //先に更新する
        ring->used += sizeof (w);
        end = ring->in = (ring->in + sizeof (w)) & ring->mask; //末尾生成
        //little endian 
        for (; pos != end; pos = (pos + 1) & ring->mask) {
            ring->buf[pos] = tmp & 0xff;
            tmp >>= 8;
        }
        return w;
    } else {
        return 0; //書き込み失敗
    }
}

uint8_t* ring2_read(ring2_ptr obj, uint8_t* mem, size_t sz) {
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

char* ring2_gets(ring2_ptr obj, char *mem, size_t sz) {
    //改行を探す
    int pos, cnt;
    for (pos = obj->out, cnt = 0; pos != obj->in; pos = (pos + 1) & obj->mask, cnt++) {
        if (obj->buf[pos] == newline)break;
    }
    int pos2;
    if (cnt+2 < sz && pos != obj->in) {
        //copy
        for (pos2 = obj->out,cnt=0; pos2 != pos; pos2 = (pos2 + 1) & obj->mask,cnt++ ){
            mem[cnt]=obj->buf[pos2];
        }
        //clip
        mem[cnt]='\0';
        //update
        obj->out=(obj->out+cnt+1)&obj->mask;
        obj->used-=cnt+1;
        return mem;
    }else{
        return NULL;
    }

}

int ring2_getw(ring2_ptr ring) {
    if (ring == NULL)return 0; //Error
    if (ring2_readable(ring, sizeof (int))) {
        uint16_t end;
        uint16_t pos = ring->in;
        unsigned int result = 0, tmp;
        //先に更新する
        ring->used -= sizeof (int);
        end = ring->out = (ring->out + sizeof (int)) & ring->mask; //末尾生成
        //little endian 
        for (; pos != end; pos = (pos + 1) & ring->mask) {
            tmp = (unsigned int) ring->buf[pos] << (sizeof (unsigned int) - 1);
            result = result >> 8 | tmp;
        }
        return (int) result;
    } else {
        return 0; //書き込み失敗
    }
}