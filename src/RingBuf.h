/*
  RingBuf-c
  v1.4
  https://github.com/t1m013y/RingBuf-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
*/

#ifndef incl_RingBuf_h
#define incl_RingBuf_h

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdbool.h>

typedef struct RingBuf {
  bool _wInit;
  volatile bool _isLocked;
  
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

int RingBuf_Peek(RingBuf* buffer_h, size_t index, char* data);

size_t RingBuf_GetElementsCount(RingBuf* buffer_h);
bool RingBuf_IsEmpty(RingBuf* buffer_h);
bool RingBuf_IsFull(RingBuf* buffer_h);

bool RingBuf_IsLocked(RingBuf* buffer_h);

size_t RingBuf_GetBufferSize(RingBuf* buffer_h);

char* RingBuf_OA_GetBufferPointer(RingBuf* buffer_h);
size_t RingBuf_OA_GetReadIndex(RingBuf* buffer_h);
size_t RingBuf_OA_GetWriteIndex(RingBuf* buffer_h);
char* RingBuf_OA_GetReadPointer(RingBuf* buffer_h);
char* RingBuffer_OA_GetWritePointer(RingBuf* buffer_h);
int RingBuf_OA_ElementQueued(RingBuf* buffer_h);
int RingBuf_OA_ElementDequeued(RingBuf* buffer_h);

int RingBuf__Lock(RingBuf* buffer_h);  // Auxiliary function, not recommended to use
int RingBuf__Unlock(RingBuf* buffer_h);  // Auxiliary function, not recommended to use


#ifdef __cplusplus
}
#endif

#endif  // incl_RingBuf_h
