# RingBuf-c

![license](https://img.shields.io/github/license/t1m013y/RingBuf-c)

A ring buffer (FIFO) for C and C++ languages.

## Attention!
`RingBuf` renamed to `RingBuf_t` since v1.6

## About
Ring buffer is a data structure that uses a single, fixed-size buffer as if it were connected end-to-end. [\(Wikipedia\)](https://en.wikipedia.org/wiki/Circular_buffer)

Demonstration of a ring buffer:  
<img src="https://upload.wikimedia.org/wikipedia/commons/f/fd/Circular_Buffer_Animation.gif" alt="Ring buffer demonstration gif" width=400></img>  
[\(Wikipedia\)](https://en.wikipedia.org/wiki/Circular_buffer)

## Features
* Can store `char` or any 8-bit type
* Can be used with C and C++ languages
* Easy to use
* Custom buffer size
* Supports data peek
* Multiple buffers can be created
* Can be deinitialized to free memory
* Supports direct access to the buffer (e.g. for DMA)
* Open source MIT license

## Documentation
[English \[English\]](DOCS.md)

## Usage
First, create and initialize the buffer.
```c
RingBuf_t ring_buffer;  // Create the ring buffer
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
RingBuffer_Dequeue(&ring_buffer, &b);  // Take one element from the buffer and save to b; RingBuffer_Dequeue(pointer to buffer, pointer to variable to save)
printf("%c", b);  // Will print "a"
char c[6];
RingBuffer_DequeueArr(&ring_buffer, c, 5);  // Take 5 elements from the buffer and save to c; RingBuffer_DequeueArr(pointer to buffer, pointer to array to save, data size)
printf("%s", c);  // Will print "hello"
```

Watch detailed info about library functions in the [documentation](#Documentation).

**Important!** Don't modify or read any element of the buffer structure manually! It can break the buffer! ~~`size_t n = ring_buffer.elements_count;`~~ `size_t n = RingBuf_GetElementsCount(&ring_buffer);`
