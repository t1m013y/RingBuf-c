/*
  RingBuf-c
  v1.0
  https://github.com/t1m013y/RingBuf-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
*/

#ifndef __RingBuf_h
#define __RingBuf_h

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdbool.h>

typedef struct RingBuf {
  bool _wInit;
  
  char* buffer;
  
  size_t buffer_size;
  
  size_t tail_index;
  size_t elements_count;
} RingBuf;

int RingBuf_Init(RingBuf* buffer_h, size_t buffer_size);
int RingBuf_Deinit(RingBuf* buffer_h);
bool RingBuf_IsInit(RingBuf* buffer_h);

int RingBuf_Clear(RingBuf* buffer_h);

int RingBuf_Queue(RingBuf* buffer_h, const char data);
size_t RingBuf_QueueArr(RingBuf* buffer_h, const char * data, size_t size);

int RingBuf_Dequeue(RingBuf* buffer_h, char* data);
size_t RingBuf_DequeueArr(RingBuf* buffer_h, char* data, size_t size);

int RingBuf_Peek(RingBuf* buffer_h, int index, char* data);

size_t RingBuf_GetElementsCount(RingBuf* buffer_h);
bool RingBuf_IsEmpty(RingBuf* buffer_h);
bool RingBuf_IsFull(RingBuf* buffer_h);


#ifdef __cplusplus
}
#endif

#endif  // __RingBuf_h
