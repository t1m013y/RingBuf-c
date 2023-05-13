# RingBuf-c v1.1 documentation

A ring buffer (FIFO) for C and C++ languages. 

Ring buffer is a data structure that uses a single, fixed-size buffer as if it were connected end-to-end. [\(Wikipedia\)](https://en.wikipedia.org/wiki/Circular_buffer)

## RingBuf
`typedef struct RingBuf {...} RingBuf`  
The ring buffer structure and typedef. Used to create the buffer and by library functions. Stores data about the buffer. 

**Example:**   
```c
RingBuf ring_buffer;  // Create the ring buffer structure
```

**Important!** Don't forget to init the buffer with `RingBuf_Init()` before usage! 

**Note:** Don't modify or read any element of the buffer structure manually! It can break the buffer! ~~`size_t n = ring_buffer.elements_count;`~~ `size_t n = RingBuf_GetElementsCount(&ring_buffer);`

## RingBuf_Init()
`int RingBuf_Init(RingBuf* buffer_h, size_t buffer_size)`  
`buffer_h` – Pointer to the `RingBuf` structure  
`buffer_size` – Size of the buffer  
Return value – `1` if init is successful, `0` otherwise

Initializes the ring buffer before usage. 

**Example**:   
```c
RingBuf_Init(&ring_buffer, 128);  // Initialize the buffer
```

**Note:** Don't initialize the buffer twice. If the buffer is already initialized, you can initialize it again only after deinitialization with `RingBuf_Deinit()`. 

## RingBuf_Deinit()
`int RingBuf_Deinit(RingBuf* buffer_h)`
`buffer_h` – Pointer to the `RingBuf` structure  
Return value – `1` if initialization is successful, `0` otherwise

Deinitialize the ring buffer. Use this function to free memory if you don't need the buffer anymore. If the buffer is already deinitialized, returns `0`. After deinitialization, the buffer can be initialized and used again. 

**Example:**   
```c
RingBuf_Deinit(&ring_buffer);  // Deinitialize the buffer
```

**Note:** After deinitialization, all data stored in the buffer will be lost! 

## RingBuf_IsInit()
`bool RingBuf_IsInit(RingBuf* buffer_h)`  
`buffer_h` – Pointer to the `RingBuf` structure  
Return value – `true` if the buffer is initialized, `false` if the buffer is not initialized

Checks if the buffer structure is initialized. 

**Example:**   
```c
if (!RingBuf_IsInit(&ring_buffer)) {  // If the buffer is not initialized
    RingBuf_Init(&ring_buffer, buffer_size);  // Initialize the buffer
}
```

## RingBuf_Clear()
`int RingBuf_Clear(RingBuf* buffer_h)`  
`buffer_h` – Pointer to the `RingBuf` structure  
Return value – `1` if successful, `0` otherwise

Clears the ring buffer. Deletes all elements from it. 

**Example:**   
```c
RingBuf_Clear(&ring_buffer);  // Clear the buffer
```

## RingBuf_Queue()
`int RingBuf_Queue(RingBuf* buffer_h, const char data)`  
`buffer_h` – Pointer to the `RingBuf` structure  
`data` – Data to add to the buffer  
Return value – `1` if successful, `0` otherwise

Adds one element to buffer. If buffer is already full, it will overwrite the oldest element of the buffer. 

**Example:**   
```c
RingBuf_Queue(&ring_buffer, 'a');  // Add 'a' to the buffer
```

## RingBuf_QueueArr()
`size_t RingBuf_QueueArr(RingBuf* buffer_h, const char * data, size_t size)`  
`buffer_h` – Pointer to the `RingBuf` structure  
`data` – Pointer to data array to add to the buffer  
`size` – Size of a data array  
Return value – Number of bytes have been queued

Adds `size` elements from `data` to the buffer. If `data` is a null pointer, the function will add `size` elements with `0` value. 

**Example:**   
```c
char a[] = "hello";
RingBuf_QueueArr(&ring_buffer, a, 5);  // Add "hello" to the buffer
```

## RingBuf_Dequeue()
`int RingBuf_Dequeue(RingBuf* buffer_h, char* data)`  
`buffer_h` – Pointer to the `RingBuf` structure  
`data` – Pointer to a variable to save read value  
Return value – `1` if successful, `0` otherwise

Reads one (oldest) element from the buffer and removes this element. If `data` is a null pointer, the function will remove the element from the buffer but it will not be saved. 

**Example:**   
```c
char b;
RingBuffer_Dequeue(&ring_buffer, &b);  // Take one element from the buffer and save to b
printf("%c", b);
```

## RingBuf_DequeueArr()
`size_t RingBuf_DequeueArr(RingBuf* buffer_h, char* data, size_t size)`  
`buffer_h` – Pointer to the `RingBuf` structure  
`data` – Pointer to an array to save data  
`size` – Size of the data to read  
Return value – Number of bytes have been read

Reads `size` (oldest) elements from the buffer, saves them to the `data` array and removes them from the buffer. If `data` is a null pointer, bytes will be removed but they will not be saved. 

**Example:**   
```c
char c[6];
RingBuffer_DequeueArr(&ring_buffer, c, 5);  // Take 5 elements from the buffer and save to c
printf("%s", c);
```

## RingBuf_Peek()
`int RingBuf_Peek(RingBuf* buffer_h, size_t index, char* data)`  
`buffer_h` – Pointer to the `RingBuf` structure  
`index` – Index of the element to read  
`data` – Pointer to a variable to save read value  
Return value – `1` if successful, `0` otherwise

Reads th element with the given index from the buffer, saves it to `data` but do not deletes it. The index is relative to the oldest written element. If `data` is a null pointer, data will not be saved. Returns `0` if index is out of range. 

**Example:**   
```c
char r;
RingBuf_Peek(&ring_buffer, 3, &r);  // Read the element with index 3 and save it to r
printf("%c", r);
```

## RingBuf_GetElementsCount()
`size_t RingBuf_GetElementsCount(RingBuf* buffer_h)`  
`buffer_h` – Pointer to the `RingBuf` structure  
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

## RingBuf_IsEmpty()
`bool RingBuf_IsEmpty(RingBuf* buffer_h)`  
`buffer_h` – Pointer to the `RingBuf` structure  
Return value – `true` if the buffer is empty, `false` otherwise

Checks if the buffer is empty. If the buffer is not initialized, it returns `false`. 

**Example:**   
```c
while (!RingBuf_IsEmpty(&ring_buffer)) {  // If the buffer is not empty
    char r;
	RingBuf_Dequeue(&ring_buffer, &r);  // Read one element from the buffer
	printf("%c", r);
}
```

## RingBuf_IsFull()
`bool RingBuf_IsFull(RingBuf* buffer_h)`  
`buffer_h` – Pointer to the `RingBuf` structure  
Return value – `true` if the buffer is full, `false` otherwise

Checks if the buffer is full. If the buffer is not initialized, it returns `false`. 

**Example:**   
```c
if (!RingBuf_IsFull(&ring_buffer)) {  // If the buffer is not full
    RingBuf_Queue(&ring_buffer, 'a');  // Add 'a' to the buffer
}
```
