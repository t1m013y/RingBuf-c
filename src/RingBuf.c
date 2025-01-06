/*
  RingBuf-c
  v2.0
  https://github.com/t1m013y/RingBuf-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
  This code is licensed under MIT license
*/

#include "RingBuf.h"

short RingBuf_Init(RingBuf_t *ctx, size_t buffer_size)
{
  if (buffer_size == 0)
    return 0;
  
  if (ctx->_wInit)
    return 0;
  
  ctx->buffer_size = buffer_size;
  
  ctx->head_index = 0;
  ctx->elements_count = 0;
  
  ctx->buffer = (char*)malloc(ctx->buffer_size);
  if (ctx->buffer == NULL)
    return 0;
  
  ctx->_isLocked = false;
  ctx->_wInit = true;
  return 1;
}

short RingBuf_Deinit(RingBuf_t *ctx)
{
  if (!ctx->_wInit)
    return 1;
  
  if (!RingBuf__Lock(ctx))
    return 0;
  
  ctx->_wInit = false;
  
  free(ctx->buffer);
  
  return 1;
}

bool RingBuf_IsInit(RingBuf_t *ctx)
{
  return ctx->_wInit;
}

short RingBuf_Clear(RingBuf_t *ctx)
{
  if (!RingBuf__Lock(ctx))
    return 0;
  
  ctx->head_index = 0;
  ctx->elements_count = 0;
  
  RingBuf__Unlock(ctx);
  return 1;
}

short RingBuf_Queue(RingBuf_t *ctx, const char data)
{
  if (!RingBuf__Lock(ctx))
    return 0;
  
  if (ctx->elements_count == ctx->buffer_size) {
    char *write_addr = ctx->buffer + ctx->head_index;
    
    *write_addr = data;
    ++ctx->head_index;
    ctx->head_index %= ctx->buffer_size;
  } else {
    char *write_addr = ctx->buffer + (ctx->head_index + ctx->elements_count) % ctx->buffer_size;
    
    *write_addr = data;
    ++ctx->elements_count;
  }
  
  RingBuf__Unlock(ctx);
  return 1;
}

short RingBuf_Queue_NoOverwrite(RingBuf_t *ctx, const char data) {
  if (!RingBuf__Lock(ctx))
    return 0;
  
  if (ctx->elements_count == ctx->buffer_size) {
    RingBuf__Unlock(ctx);
    return 0;
  }
  
  char *write_addr = ctx->buffer + (ctx->head_index + ctx->elements_count) % ctx->buffer_size;
  
  *write_addr = data;
  ++ctx->elements_count;
  
  RingBuf__Unlock(ctx);
  return 1;
}

size_t RingBuf_QueueArr(RingBuf_t *ctx, const char *data, size_t size)
{
  if (!RingBuf__Lock(ctx))
    return 0;
  
  if (size >= ctx->buffer_size) {
    data += size - ctx->buffer_size;
    memcpy(ctx->buffer, data, ctx->buffer_size);
    ctx->head_index = 0;
    ctx->elements_count = ctx->buffer_size;
    
    RingBuf__Unlock(ctx);
    return size;
  }
  
  size_t tail_index = (ctx->head_index + ctx->elements_count) % ctx->buffer_size;
  size_t atsize = ctx->buffer_size - tail_index;
  if (atsize > size)
    atsize = size;
  memcpy(ctx->buffer + tail_index, data, atsize);
  
  size_t size2 = size - atsize;
  if (size2 != 0) {
    memcpy(ctx->buffer, data + atsize, size2);
    if (size2 > ctx->head_index) {
      ctx->elements_count -= size2 - ctx->head_index;
      ctx->head_index = size2;
    }
  }
  
  ctx->elements_count += size;
  
  RingBuf__Unlock(ctx);
  return size;
}

size_t RingBuf_QueueArr_NoOverwrite(RingBuf_t *ctx, const char *data, size_t size)
{
  if (!RingBuf__Lock(ctx))
    return 0;
  
  size_t free_size = ctx->buffer_size - ctx->elements_count;
  if (size > free_size) {
    size = free_size;
    data += size - free_size;
  }
  
  size_t tail_index = (ctx->head_index + ctx->elements_count) % ctx->buffer_size;
  size_t atsize = ctx->buffer_size - tail_index;
  if (atsize > size)
    atsize = size;
  memcpy(ctx->buffer + tail_index, data, atsize);
  
  size_t size2 = size - atsize;
  if (size2 != 0) {
    memcpy(ctx->buffer, data + atsize, size2);
  }
  
  ctx->elements_count += size;
  
  RingBuf__Unlock(ctx);
  return size;
}

short RingBuf_Dequeue(RingBuf_t *ctx, char *data)
{
  if (!RingBuf__Lock(ctx))
    return 0;
  
  if (ctx->elements_count == 0) {
    RingBuf__Unlock(ctx);
    return 0;
  }
  
  if (data != NULL) {
    char *read_addr = ctx->buffer + ctx->head_index;
    *data = *read_addr;
  }
  
  --ctx->elements_count;
  ++ctx->head_index;
  ctx->head_index %= ctx->buffer_size;
  
  RingBuf__Unlock(ctx);
  return 1;
}

size_t RingBuf_DequeueArr(RingBuf_t *ctx, char *data, size_t size)
{
  if (!RingBuf__Lock(ctx))
    return 0;
  
  if (size > ctx->elements_count)
    size = ctx->elements_count;
  
  size_t ahrsize = ctx->buffer_size - ctx->head_index;
  if (ahrsize > size)
    ahrsize = size;
  size_t size2 = size - ahrsize;
  
  if (data != NULL) {
    memcpy(data, ctx->buffer + ctx->head_index, ahrsize);
    if (size2 != 0) {
      memcpy(data + ahrsize, ctx->buffer, size2);
    }
  }
  
  ctx->head_index += size;
  ctx->head_index %= ctx->buffer_size;
  ctx->elements_count -= size;
  
  RingBuf__Unlock(ctx);
  return size;
}

short RingBuf_Peek(RingBuf_t *ctx, size_t index, char *data)
{
  if (!RingBuf__Lock(ctx))
    return 0;
  
  if (index >= ctx->elements_count) {
    RingBuf__Unlock(ctx);
    return 0;
  }
  
  if (data == NULL) {
    RingBuf__Unlock(ctx);
    return 1;
  }
  
  char *read_addr = ctx->buffer + (ctx->head_index + index) % ctx->buffer_size;
  *data = *read_addr;
  
  RingBuf__Unlock(ctx);
  return 1;
}

size_t RingBuf_PeekArr(RingBuf_t *ctx, size_t from_index, size_t size, char *data)
{
  if (!RingBuf__Lock(ctx))
    return 0;
  
  if (from_index >= ctx->elements_count) {
    RingBuf__Unlock(ctx);
    return 0;
  }
  
  size_t maxsize = ctx->elements_count - from_index;
  if (size > maxsize)
    size = maxsize;
  
  if (data == NULL){
    RingBuf__Unlock(ctx);
    return size;
  }
  
  size_t read_from = (ctx->head_index + from_index) % ctx->buffer_size;
  
  size_t afisize = ctx->buffer_size - read_from;
  if (afisize > size)
    afisize = size;
  memcpy(data, ctx->buffer + read_from, afisize);
  
  size_t size2 = size - afisize;
  if (size2 != 0) {
    memcpy(data + afisize, ctx->buffer, size2);
  }
  
  RingBuf__Unlock(ctx);
  return size;
}

size_t RingBuf_GetElementsCount(RingBuf_t *ctx)
{
  if (!ctx->_wInit)
    return 0;
  
  return ctx->elements_count;
}

bool RingBuf_IsEmpty(RingBuf_t *ctx)
{
  if (!ctx->_wInit)
    return 0;
  
  return ctx->elements_count == 0;
}

bool RingBuf_IsFull(RingBuf_t *ctx)
{
  if (!ctx->_wInit)
    return 0;
  
  return ctx->elements_count == ctx->buffer_size;
}

bool RingBuf_IsLocked(RingBuf_t *ctx)
{
  if (!ctx->_wInit)
    return 0;
  
  return ctx->_isLocked;
}

size_t RingBuf_GetBufferSize(RingBuf_t *ctx)
{
  if (!ctx->_wInit)
    return 0;
  
  return ctx->buffer_size;
}

short RingBuf__Lock(RingBuf_t *ctx)  // Auxiliary function, not for usage outside library's code
{
  if (!ctx->_wInit || ctx->_isLocked)
    return 0;
  
  ctx->_isLocked = true;
  return 1;
}

short RingBuf__Unlock(RingBuf_t *ctx)  // Auxiliary function, not for usage outside library's code
{
  if (!ctx->_wInit)
    return 0;
  
  ctx->_isLocked = false;
  return 1;
}
