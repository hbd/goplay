#include <stdlib.h> /* size_t, malloc */
#include <strings.h> /* memset */

/*
 * Slice.
 */

// Define an underlying slice struct.
typedef   struct {
  void**  data; // Generic array.
  size_t  len; // size_t is an unsigned int >= 16 bits
  size_t  cap;
}         s_slice;

// Define a slice, a pointer to a s_slice struct.
typedef   s_slice* slice;

/*
 * !Slice.
 */


/*
 * Interface.
 */

typedef   struct {
  char**  name; // Name of the function.
}         function;

// itable (interface table) defines the lists the methods used to satisfy an interface.
typedef     struct {
  /* void*  type?    */   // type is ...?
  /* void** function */   // functions
}           itable;

// Define an underlying slice struct.
typedef  struct {
  void*  type;  // Pointer to type in itable (interface table).
  void*  data;  // Pointer to data.
}        s_interface;

// Define an interface, a pointer to an s_interface struct.
typedef  s_interface* interface;

/*
 * !Interface.
 */

/*
 * Helpers.
 */

// Returns a zeroed out piece of memory.
// i.e. malloc + memset
void* mallocset(size_t size) {
  void* ret;

  // Carve out memory.
  ret = malloc(size);
  // Fill the malloc'd space with 0.
  memset(ret, 0, size);

  return ret;
}

/*
 * !Helpers.
 */

/*
 * Builtin Functions.
 */

// Define `make`.
slice make(void* dataType, size_t len, size_t cap) {
  // Carve out mem for s_slice.
  slice s = mallocset(sizeof(s_slice));

  // Check the capacity.
  if (cap == 0) {
    cap = len;
  }

  // Set the values.
  s->data = mallocset(sizeof(*dataType) * cap);
  s->len = len;
  s->cap = cap;

  return s;
}

/*
 * !Builtin Functions.
 */

#define byte()(char*)0
#define int_t()(char*)0

int main() {
  slice s = make(byte(), 10, 0);
}
