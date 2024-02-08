Simple LRU cache implementation.

Cached type:
```c
struct page_t {
  char* key;
  char* data;
};
```

Build and run tests:
```
make
make check
```
