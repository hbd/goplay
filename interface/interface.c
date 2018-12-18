#include <unistd.h> /* write(2), read(2) */
#include <stdlib.h> /* malloc(3) */
#include <stdio.h> /* perror(3) */
#include <strings.h> /* memset(3) */

typedef         struct {
    const char* data;
    size_t      len;
}               s_string;

typedef s_string* string;

typedef     struct {
    void**  data;
    size_t  len;
    size_t  cap;
}           s_slice;

typedef s_slice* slice;

string      mkString(const char* in) {
    string  str = malloc(sizeof(s_string));

    str->data = in;
    for (str->len = 0; *(in + str->len); ++str->len);
    return str;
}

size_t  len(void* in) {
    return ((slice)in)->len;
}

size_t  cap(void* in) {
    return ((slice)in)->cap;
}

ssize_t printString(string str) {
    return write(1, str->data, len(str));
}

/* Wraps malloc + memset. */
void*       mallocset(size_t size) {
    void*   ret;

    ret = malloc(size);
    memset(ret, 0, size);
    return ret;
}

/* TOOD: Handle vargs for optional len/cap. */
slice       make(void* dataType, size_t len, size_t cap) {
    // Carve out a `slice` of memory (includes pointer to
    // data array, len, and capacity).
    slice   ret = mallocset(sizeof(s_slice));

    // If we are not given a capacity, initialize the capacity as the given length.
    // This means we will need to increase the capacity if the length increases.
    if (cap == 0) cap = len;

    // Carve out the data array.
    ret->data = mallocset(sizeof(*dataType) * cap);
    ret->len = len;
    ret->cap = cap;

    return ret;
}

/* TOOD: Handle vargs for optional end. */
slice       subSlice(slice in, int begin, int end) {
    slice   ret = mallocset(sizeof(s_slice));;

    ret->data = in->data + begin;

    /* TODO: Do this properly when cap != len. */
    ret->len = end - begin;
    ret->cap = in->cap - begin;
    return ret;
}

void*   sliceIdx(slice in, int i) {
    return *(in->data + i);
}

slice       append(slice in, void* elem) {
    /* Empty slice. */
    if (in->len == 0) {
        in = make(in->data, 1, 1);
        *in->data = elem;
        return in;
    }

    /* We have enough space. */
    if (in->len < in->cap) {
        *(in->data + in->len++) = elem;
        return in;
    }

    /* Need realloc. */
    slice ret = mallocset(sizeof(s_slice));

    ret->data = mallocset(sizeof(*in->data) * cap(in) * 2);
    /* TODO: Use memcpy or memmove. */
    for (size_t i = 0; i < len(in); ++i) {
        ret->data[i] = in->data[i];
    }
    ret->cap = cap(in) * 2;
    ret->len = len(in) + 1;
    *(ret->data + len(in)) = elem;
    return ret;
}

string      String(slice in) {
    string  ret = mkString("");

    ret->data = (const char*)in->data;
    ret->len  = in->len;
    return ret;
}

/* func (r fd) Read(buf []byte) (n int, err error) */
ssize_t  Read(int fd, slice buf) {
    return read(fd, buf, len(buf));
}

#define byte()(char*)0
#define int_t()(int*)0

#define range(_idx, _val, _slice, _body)                         \
    size_t  _idx;                                                \
    for (_idx = 0; _idx < len(_slice); _idx++) {                 \
        void* _val = sliceIdx(_slice, _idx);                     \
        _body                                                    \
    }

int         main() {
    /* str := "hello world\n" */
    string  str = mkString("Hello world!\n");
    printString(str);

    /* buf := make([]byte, 50) */
    slice   buf = make(byte(), 50, 0);
    /* n, err := os.Stdin.Read(buf) */
    int     n = Read(0, buf);

    /* if err != nil */
    if (n == -1) {
        /* log.Printf("%s\n", err) */
        perror("read");
        return -1;
    }

    /* buf = buf[:n] */
    buf = subSlice(buf, 0, n);

    /* fmt.Printf("%s\n", string(buf)) */
    printString(String(buf));

    /* nums := []int{} */
    slice   nums = make(int_t(), 0, 0);

    /* nums = append(nums, 11) */
    nums = append(nums, 11);
    nums = append(nums, 21);
    nums = append(nums, 31);
    nums = append(nums, 41);

    /*
       for i, elem := range nums {
         fmt.Printf("[%d] %d\n", i, elem);
       )
    */

    range(i, elem, nums, {
            printf("[%d] %d\n", i, elem);
        })

    return 0;
}

