/*
  RingBuf-c
  v2.0
  https://github.com/t1m013y/RingBuf-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
  This code is licensed under MIT license
*/

#ifndef INCLUDED_t1m013y_RingBuf_h
#define INCLUDED_t1m013y_RingBuf_h

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
#include <stdbool.h>  // Include stdbool.h if not c++
#endif

typedef struct {
  bool _wInit;
  volatile bool _isLocked;
  
  char *buffer;
  
  size_t buffer_size;
  
  size_t head_index;
  size_t elements_count;
} RingBuf_t;

short RingBuf_Init(RingBuf_t *ctx, size_t buffer_size);
short RingBuf_Deinit(RingBuf_t *ctx);

bool RingBuf_IsInit(RingBuf_t *ctx);

short RingBuf_Clear(RingBuf_t *ctx);

short RingBuf_Queue(RingBuf_t *ctx, const char data);
short RingBuf_Queue_NoOverwrite(RingBuf_t *ctx, const char data);
size_t RingBuf_QueueArr(RingBuf_t *ctx, const char *data, size_t size);
size_t RingBuf_QueueArr_NoOverwrite(RingBuf_t *ctx, const char *data, size_t size);

short RingBuf_Dequeue(RingBuf_t *ctx, char *data);
size_t RingBuf_DequeueArr(RingBuf_t *ctx, char *data, size_t size);

short RingBuf_Peek(RingBuf_t *ctx, size_t index, char *data);
size_t RingBuf_PeekArr(RingBuf_t *ctx, size_t from_index, size_t size, char *data);

size_t RingBuf_GetElementsCount(RingBuf_t *ctx);
bool RingBuf_IsEmpty(RingBuf_t *ctx);
bool RingBuf_IsFull(RingBuf_t *ctx);

bool RingBuf_IsLocked(RingBuf_t *ctx);

size_t RingBuf_GetBufferSize(RingBuf_t *ctx);

short RingBuf__Lock(RingBuf_t *ctx);  // Auxiliary function, not for usage outside library's code
short RingBuf__Unlock(RingBuf_t *ctx);  // Auxiliary function, not for usage outside library's code


#ifdef __cplusplus
}
#endif

#endif  // INCLUDED_t1m013y_RingBuf_h
