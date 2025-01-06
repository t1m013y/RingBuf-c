# RingBuf-c v2.0 documentation

A ring buffer (FIFO) for C and C++ languages.

Ring buffer is a data structure that uses a single, fixed-size buffer as if it were connected end-to-end. [\(Wikipedia\)](https://en.wikipedia.org/wiki/Circular_buffer)

## Index

* [Compiling](#compiling)
* [Adding to your project](#adding-to-your-project)
* [Proper usage](#proper-usage)
* [Locking mechanism](#locking-mechanism)
* [Basic usage example](#basic-usage-example)
* [Recent changes in the library interface](#recent-changes-in-the-library-interface)
* [Library interface documentation](#library-interface-documentation)

## Compiling

Requires C99 or later, GCC/G++ compiler is recommended but should work with most other compilers.

## Adding to your project

To add this library to your project, just include the `src/RingBuf.h` file and add `src/RingBuf.c` to sources.

**Example:**  
`main.c`:  
```c
#include "RingBuf.h"

int main() {
    /* Your code here */
}
```
Compile command: `gcc main.c RingBuf.c -o main.exe`

## Proper usage

- When declaring `RingBuf_t` variable, initialize it with `{0}`: `RingBuf_t ring_buffer = {0};` (`{}` can be used instead of `{0}` since C23).  
When allocating it on the heap, fill it with zeros using `memset`: `memset(ringbuf_ptr, 0, sizeof(RingBuf_t))` (`ringbuf_ptr` is pointer to the `RingBuf_t` allocated on the heap)  
If it is not initialized with zero value, it **may result in undefined behavior and memory leaks!**
- Don't modify or read any element of the buffer structure manually. It can break the buffer and cause **undefined behavior**! ~~`size_t n = ring_buffer.elements_count;`~~ `size_t n = RingBuf_GetElementsCount(&ring_buffer);`

## Locking mechanism

The ring buffer has a locking mechanism, which allows it to be used in multi-threaded systems and in interrupts on microcontrollers. When the buffer is in the process of being modified (for example, adding data), lock flag is set and other threads/interrupts cannot modify the buffer. It made to avoid conflicts and undefined behavior when several processes modify the same buffer at the same time. When buffer modification process ends, the locked flag is cleared and other processes can modify the buffer. The lock state can be checked with `RingBuf_IsLocked()` function.

Most of the functions will return `0` if they are called when the buffer is locked by another therad/interrupt.

## Basic usage example

First, create and initialize the buffer.
```c
RingBuf_t ring_buffer = {0};  // Create the buffer and initialize it with zero
RingBuf_Init(&ring_buffer, 128);  // Initialize the buffer; RingBuf_Init(pointer to buffer, buffer size)
```

Use `RingBuf_Queue()` and `RingBuf_QueueArr()` to add data to the buffer.
```c
RingBuf_Queue(&ring_buffer, 'a');  // Add 'a' to the buffer; RingBuf_Queue(pointer to buffer, data)
char a[] = "hello";
RingBuf_QueueArr(&ring_buffer, a, 5);  // Add "hello" to the buffer; RingBuf_QueueArr(pointer to buffer, pointer to data array, data size)
```

Use `RingBuf_Dequeue()` and `RingBuf_DequeueArr()` to take data from the buffer.
```c
char b;
RingBuf_Dequeue(&ring_buffer, &b);  // Take one element from the buffer and save to b; RingBuf_Dequeue(pointer to buffer, pointer to variable to save)
printf("%c", b);  // Will print "a"
char c[6];
memset(c, 0, 6);  // Fill c with zeros
RingBuf_DequeueArr(&ring_buffer, c, 5);  // Take 5 elements from the buffer and save to c; RingBuf_DequeueArr(pointer to buffer, pointer to array to save, data size)
printf("%s", c);  // Will print "hello"
```

## Recent changes in the library interface

V2.0:
- **`RingBuf_OA_*` functions removed**  
because usage of these functions caused bugs and they can't work normally with recent optimizations.
- **`RingBuf_IsInit` is no more deprecated**  
- **Return type of some functions changed from `int` to `short`**
- **`RingBuf_QueueArr()` doesn't add zero elements to the buffer if `data` is a null pointer**  
It will cause undefined behavior insted. It made for optimization purposes.

V1.6:
- **`RingBuf` renamed to `RingBuf_t` since v1.6**

## Library interface documentation

### RingBuf_t
`typedef struct { ... } RingBuf_t`  
The ring buffer typedef. Used to create the buffer and passed as the first argument to library functions. Stores data about the buffer.

**Example:** see [RinBuf_Init()](#ringbuf_init)

**Note:** Don't forget to initialize the buffer using `RingBuf_Init()` before usage.

**Important!** Follow the points specified in [Proper usage](#proper-usage) to avoid problems and undefined behavior cases when using the buffer.

### RingBuf_Init()
`short RingBuf_Init(RingBuf_t *ctx, size_t buffer_size)`  
`ctx` – Pointer to the `RingBuf_t`  
`buffer_size` – Size of the buffer (must be greater than `0`)  
Return value – `1` if initialization is successful, `0` otherwise

Initializes the ring buffer. Returns `0` if the buffer is already initialized.

**Example**:  
```c
RingBuf_t ring_buffer = {0};  // Create the buffer and initialize it with zero

while (!RingBuf_Init(&ring_buffer, 128)) {}  // Better way to initialize the buffer, will retry until initialization is successful, but make sure that the buffer was not already initialized, otherwise the loop will be infinite.
```

### RingBuf_Deinit()
`short RingBuf_Deinit(RingBuf_t *ctx)`  
`ctx` – Pointer to the `RingBuf_t`  
Return value – returns `1` if the buffer was deinitialized, `0` if the buffer is currently locked

Deinitializes the ring buffer. Use this function to free memory if you don't need the buffer anymore. If the buffer is already deinitialized, returns `1`. After deinitialization, the buffer can be initialized and used again.  
**Ignores lock flag**

**Example:**  
```c
RingBuf_Deinit(&ring_buffer);  // Deinitialize the buffer
```

**Warning!** Be sure not to use the buffer after deinitialization.

### RingBuf_IsInit() 
`bool RingBuf_IsInit(RingBuf_t *ctx)`  
`ctx` – Pointer to the `RingBuf_t`  
Return value – `true` if the buffer is initialized, `false` if the buffer is not initialized

Checks if the buffer is initialized.

**Example:**  
```c
if (!RingBuf_IsInit(&ring_buffer)) {  // If the buffer is not initialized
    RingBuf_Init(&ring_buffer, buffer_size);  // Initialize the buffer
}
```

### RingBuf_Clear()
`short RingBuf_Clear(RingBuf_t *ctx)`  
`ctx` – Pointer to the `RingBuf_t`  
Return value – `1` if successful, `0` otherwise

Clears the ring buffer.

**Example:**  
```c
RingBuf_Clear(&ring_buffer);  // Clear the buffer
```

### RingBuf_Queue()
`short RingBuf_Queue(RingBuf_t *ctx, const char data)`  
`ctx` – Pointer to the `RingBuf_t`  
`data` – Data to add to the buffer  
Return value – `1` if successful, `0` otherwise

Adds one element to buffer. Overwrites elements of the buffer if there is no free space in the buffer.

**Example:**  
```c
RingBuf_Queue(&ring_buffer, 'a');  // Add 'a' to the buffer
```

### RingBuf_Queue_NoOverwrite()
`short RingBuf_Queue_NoOverwrite(RingBuf_t *ctx, const char data);`  
`ctx` – Pointer to the `RingBuf_t`  
`data` – Data to add to the buffer  
Return value – `1` if successful, `0` otherwise

Adds one element to buffer. Does not overwrite elements of the buffer and return `0` if there is no free space in the buffer.

**Example:**  
```c
RingBuf_Queue_NoOverwrite(&ring_buffer, 'a');  // Add 'a' to the buffer but don't overwrite if no free space
```

### RingBuf_QueueArr()
`size_t RingBuf_QueueArr(RingBuf_t *ctx, const char *data, size_t size)`  
`ctx` – Pointer to the `RingBuf_t`  
`data` – Pointer to data array to add to the buffer  
`size` – Size of a data array  
Return value – Number of bytes have been queued

Adds `size` elements from `data` to the buffer. Overwrites elements of the buffer if there is no enough free space in the buffer.

**Example:**  
```c
char a[] = "hello";
RingBuf_QueueArr(&ring_buffer, a, 5);  // Add "hello" to the buffer
```

### RingBuf_QueueArr_NoOverwrite()
`size_t RingBuf_QueueArr_NoOverwrite(RingBuf_t *ctx, const char *data, size_t size)`  
`ctx` – Pointer to the `RingBuf_t`  
`data` – Pointer to data array to add to the buffer  
`size` – Size of a data array  
Return value – Number of bytes have been queued

Adds `size` elements from `data` to the buffer. Does not overwrite elements of the buffer if there is no enough free space in the buffer.

**Example:**  
```c
char a[] = "hello";
RingBuf_QueueArr_NoOverwrite(&ring_buffer, a, 5);  // Add "hello" to the buffer but don't overwrite if not enough free space
```

### RingBuf_Dequeue()
`short RingBuf_Dequeue(RingBuf_t *ctx, char *data)`  
`ctx` – Pointer to the `RingBuf_t`  
`data` – Pointer to a variable to save read value  
Return value – `1` if successful, `0` otherwise

Reads one (oldest) element from the buffer and removes this element. If `data` is a null pointer, the element from the buffer will be removed but it will not be saved. Returns `0` and doesn't modify `data` if there is no elements in the buffer.

**Example:**  
```c
char b;
RingBuffer_Dequeue(&ring_buffer, &b);  // Take one element from the buffer and save to b
printf("%c", b);
```

### RingBuf_DequeueArr()
`size_t RingBuf_DequeueArr(RingBuf_t *ctx, char *data, size_t size)`  
`ctx` – Pointer to the `RingBuf_t`  
`data` – Pointer to an array to save data  
`size` – Size of the data to read  
Return value – Number of bytes have been read

Reads `size` (oldest) elements from the buffer, saves them to the `data` array and removes them from the buffer. If `data` is a null pointer, bytes will be removed but they will not be saved. If number of elements in the buffer is less than `size`, reads as mush elements as can, other elements of `data` array are unmodified.

**Warning!** Make sure that `data` length is at least `size` elements.

**Example:**  
```c
char c[6];
RingBuffer_DequeueArr(&ring_buffer, c, 5);  // Take 5 elements from the buffer and save to c
printf("%s", c);
```

### RingBuf_Peek()
`short RingBuf_Peek(RingBuf_t *ctx, size_t index, char *data)`  
`ctx` – Pointer to the `RingBuf_t`  
`index` – Index of the element to read  
`data` – Pointer to a variable to save read value  
Return value – `1` if successful, `0` otherwise

Reads the element with the given index (counted from head of the buffer), saves it to `data` but does not delete it. If `data` is a null pointer, data will not be saved. Returns `0` and doesn't modify `data` if index is out of range.

**Example:**  
```c
char r;
RingBuf_Peek(&ring_buffer, 3, &r);  // Read the element with index 3 and save it to r
printf("%c", r);
```

### RingBuf_PeekArr()
`size_t RingBuf_PeekArr(RingBuf_t *ctx, size_t from_index, size_t size, char *data)`  
`ctx` – Pointer to the `RingBuf_t`  
`from_index` – Index of the first element to read  
`size` – How many elements to read  
`data` – Pointer to an array to save read elements  
Return value – Number of elements saved to `data`

Reads `size` elements from the buffer starting from `from_index` (counted from head of the buffer), saves them to `data` but does not delete them. If `data` is a null pointer, data will not be saved. Returns `0` if `from_index` is out of range. If number of elements in the buffer is less than `size`, reads as mush elements as can, other elements of `data` array are unmodified.

**Warning!** Make sure that `data` length is at least `size` elements.

### RingBuf_GetElementsCount()
`size_t RingBuf_GetElementsCount(RingBuf_t *ctx)`  
`ctx` – Pointer to the `RingBuf_t`  
Return value – Number of elements in the buffer

Checks number of elements in the buffer. If the buffer is not initialized, it returns `0`.

**Example:**  
```c
char r[4];
if (RingBuf_GetElementsCount(&ring_buffer) >= 3) {  // If there are at least 3 elements in the buffer
    RingBuf_DequeueArr(&ring_buffer, r, 3);  // Read 3 elements from the buffer
    printf("%s", r);
}
```

### RingBuf_IsEmpty()
`bool RingBuf_IsEmpty(RingBuf_t *ctx)`  
`ctx` – Pointer to the `RingBuf_t`  
Return value – `true` if the buffer is empty, `false` otherwise

Checks if the buffer is empty. If the buffer is not initialized, returns `false`.

**Example:**  
```c
while (!RingBuf_IsEmpty(&ring_buffer)) {  // If the buffer is not empty
    char r;
    RingBuf_Dequeue(&ring_buffer, &r);  // Read one element from the buffer
    printf("%c", r);
}
```

### RingBuf_IsFull()
`bool RingBuf_IsFull(RingBuf_t *ctx)`  
`ctx` – Pointer to the `RingBuf_t`  
Return value – `true` if the buffer is full, `false` otherwise

Checks if the buffer is full. If the buffer is not initialized, it returns `false`.

**Example:**  
```c
if (!RingBuf_IsFull(&ring_buffer)) {  // If the buffer is not full
    RingBuf_Queue(&ring_buffer, 'a');  // Add 'a' to the buffer
}
```

### RingBuf_IsLocked()
`bool RingBuf_IsLocked(RingBuf_t *ctx)`  
`ctx` – Pointer to the `RingBuf_t`  
Return value – `true` if the buffer is locked, `false` otherwise

Checks if the buffer's [lock flag](#locking-mechanism) is set.

### RingBuf_GetBufferSize()
`size_t RingBuf_GetBufferSize(RingBuf_t *ctx)`  
`ctx` – Pointer to the `RingBuf_t` structure  
Return value – Ring buffer size

Returns size of the ring buffer (including free space).

**Example:**  
```c
size_t buffer_size = RingBuf_GetBufferSize(&ring_buffer);  // Get ring buffer size
```
