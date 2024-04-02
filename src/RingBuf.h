/*
  RingBuf-c
  v1.7
  https://github.com/t1m013y/RingBuf-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
*/

#ifndef INCLUDED_t1m013y_RingBuf_h
#define INCLUDED_t1m013y_RingBuf_h

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdbool.h>

typedef struct {
  bool _wInit;
  volatile bool _isLocked;
  
  char *buffer;
  
  size_t buffer_size;
  
  size_t tail_index;
  size_t elements_count;
} RingBuf_t;

int RingBuf_Init(RingBuf_t *buffer_h, size_t buffer_size);
int RingBuf_Deinit(RingBuf_t *buffer_h);

__attribute__((deprecated))
bool RingBuf_IsInit(RingBuf_t *buffer_h);

int RingBuf_Clear(RingBuf_t *buffer_h);

int RingBuf_Queue(RingBuf_t *buffer_h, const char data);
size_t RingBuf_QueueArr(RingBuf_t *buffer_h, const char  *data, size_t size);

int RingBuf_Dequeue(RingBuf_t *buffer_h, char *data);
size_t RingBuf_DequeueArr(RingBuf_t *buffer_h, char *data, size_t size);

int RingBuf_Peek(RingBuf_t *buffer_h, size_t index, char *data);

size_t RingBuf_GetElementsCount(RingBuf_t *buffer_h);
bool RingBuf_IsEmpty(RingBuf_t *buffer_h);
bool RingBuf_IsFull(RingBuf_t *buffer_h);

bool RingBuf_IsLocked(RingBuf_t *buffer_h);

size_t RingBuf_GetBufferSize(RingBuf_t *buffer_h);

char *RingBuf_OA_GetBufferPointer(RingBuf_t *buffer_h);
size_t RingBuf_OA_GetReadIndex(RingBuf_t *buffer_h);
size_t RingBuf_OA_GetWriteIndex(RingBuf_t *buffer_h);
char *RingBuf_OA_GetReadPointer(RingBuf_t *buffer_h);
char *RingBuffer_OA_GetWritePointer(RingBuf_t *buffer_h);
int RingBuf_OA_ElementQueued(RingBuf_t *buffer_h);
int RingBuf_OA_ElementDequeued(RingBuf_t *buffer_h);

int RingBuf__Queue(RingBuf_t *buffer_h, const char data, bool _ign_lock);  // Auxiliary function, not recommended to use
int RingBuf__Dequeue(RingBuf_t *buffer_h, char *data, bool _ign_lock);  // Auxiliary function, not recommended to use
int RingBuf__Lock(RingBuf_t *buffer_h);  // Auxiliary function, not recommended to use
int RingBuf__Unlock(RingBuf_t *buffer_h);  // Auxiliary function, not recommended to use


#ifdef __cplusplus
}
#endif

#endif  // INCLUDED_t1m013y_RingBuf_h
