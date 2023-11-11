/*
  RingBuf-c
  v1.4
  https://github.com/t1m013y/RingBuf-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
*/

#include "RingBuf.h"

int RingBuf_Init(RingBuf* buffer_h, size_t buffer_size)
{
  if (!(buffer_size > 0))
    return 0;
  
  if (!buffer_h->_wInit) {
    buffer_h->buffer_size = buffer_size;
    
    buffer_h->tail_index = 0;
    buffer_h->elements_count = 0;
    
    buffer_h->buffer = (char*)malloc(buffer_h->buffer_size);
    if (buffer_h->buffer == NULL)
      return 0;
    
    buffer_h->_isLocked = false;
    buffer_h->_wInit = true;
    return 1;
  } else {
    return 1;
  }
}

int RingBuf_Deinit(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 1;
  
  buffer_h->_wInit = false;
  
  free(buffer_h->buffer);
  
  return 1;
}

bool RingBuf_IsInit(RingBuf* buffer_h)
{
  return buffer_h->_wInit;
}

int RingBuf_Clear(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  if (!RingBuf__Lock(buffer_h))
    return 0;
  
  buffer_h->elements_count = 0;
  
  RingBuf__Unlock(buffer_h);
  return 1;
}

int RingBuf_Queue(RingBuf* buffer_h, const char data)
{
  if (!buffer_h->_wInit)
    return 0;
  if (!RingBuf__Lock(buffer_h))
    return 0;
  
  if (buffer_h->elements_count < buffer_h->buffer_size) {
    char* write_addr = (char*)(buffer_h->buffer + (buffer_h->tail_index + buffer_h->elements_count) % buffer_h->buffer_size);
    
    *write_addr = data;
    ++buffer_h->elements_count;
    
    RingBuf__Unlock(buffer_h);
    return 1;
  } else if (buffer_h->elements_count == buffer_h->buffer_size) {
    char* write_addr = (char*)(buffer_h->buffer + buffer_h->tail_index);
    
    ++buffer_h->tail_index;
    buffer_h->tail_index %= buffer_h->buffer_size;
    *write_addr = data;
    
    RingBuf__Unlock(buffer_h);
    return 1;
  } else {
    RingBuf__Unlock(buffer_h);
    return 0;
  }
}

size_t RingBuf_QueueArr(RingBuf* buffer_h, const char * data, size_t size)
{
  if (!buffer_h->_wInit)
    return 0;
  if (!RingBuf__Lock(buffer_h))
    return 0;
  
  size_t bytes_written = 0;
  for (size_t i = 0; i < size; i++) {
    int r;
    
    if (data != NULL)
      r = RingBuf_Queue(buffer_h, data[i]);
    else
      r = RingBuf_Queue(buffer_h, 0);
    
    if (r == 1)
      bytes_written++;
    else
      break;
  }
  
  RingBuf__Unlock(buffer_h);
  return bytes_written;
}

int RingBuf_Dequeue(RingBuf* buffer_h, char* data)
{
  if (!buffer_h->_wInit)
    return 0;
  if (!RingBuf__Lock(buffer_h))
    return 0;
  
  if (!(buffer_h->elements_count > 0)) {
    RingBuf__Unlock(buffer_h);
    return 0;
  }
  
  char* read_addr = (char*)(buffer_h->buffer + buffer_h->tail_index);
  
  char dequeued_byte = *read_addr;
  
  --buffer_h->elements_count;
  ++buffer_h->tail_index;
  buffer_h->tail_index %= buffer_h->buffer_size;
  
  if (data != NULL)
    *data = dequeued_byte;
  
  RingBuf__Unlock(buffer_h);
  return 1;
}

size_t RingBuf_DequeueArr(RingBuf* buffer_h, char* data, size_t size)
{
  if (!buffer_h->_wInit)
    return 0;
  if (!RingBuf__Lock(buffer_h))
    return 0;
  
  size_t bytes_read = 0;
  for (size_t i = 0; i < size; i++) {
    char read;
    int r = RingBuf_Dequeue(buffer_h, &read);
    
    if (r == 1) {
      if (data != NULL)
        data[i] = read;
      bytes_read++;
    } else {
      break;
    }
  }
  
  RingBuf__Unlock(buffer_h);
  return bytes_read;
}

int RingBuf_Peek(RingBuf* buffer_h, size_t index, char* data)
{
  if (!buffer_h->_wInit)
    return 0;
  
  if (!(index < buffer_h->elements_count))
    return 0;
  
  char* read_addr = (char*)(buffer_h->buffer + (buffer_h->tail_index + index) % buffer_h->buffer_size);
  if (data != NULL)
    *data = *read_addr;
  
  return 1;
}

size_t RingBuf_GetElementsCount(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  
  return buffer_h->elements_count;
}

bool RingBuf_IsEmpty(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return (bool)0;
  
  return !(buffer_h->elements_count > 0);
}

bool RingBuf_IsFull(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return (bool)0;
  
  return !(buffer_h->elements_count < buffer_h->buffer_size);
}

bool RingBuf_IsLocked(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  
  return buffer_h->_isLocked;
}

size_t RingBuf_GetBufferSize(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  
  return buffer_h->buffer_size;
}

char* RingBuf_OA_GetBufferPointer(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return (char*)NULL;
  
  return buffer_h->buffer;
}

size_t RingBuf_OA_GetReadIndex(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  
  return buffer_h->tail_index;
}

size_t RingBuf_OA_GetWriteIndex(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  
  if (buffer_h->elements_count < buffer_h->buffer_size)
    return (buffer_h->tail_index + buffer_h->elements_count) % buffer_h->buffer_size;
  else
    return buffer_h->tail_index;
}

char* RingBuf_OA_GetReadPointer(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  
  return (char*)(buffer_h->buffer + buffer_h->tail_index);
}

char* RingBuffer_OA_GetWritePointer(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  
  if (buffer_h->elements_count < buffer_h->buffer_size)
    return (char*)(buffer_h->buffer + (buffer_h->tail_index + buffer_h->elements_count) % buffer_h->buffer_size);
  else
    return (char*)(buffer_h->buffer + buffer_h->tail_index);
}

int RingBuf_OA_ElementQueued(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  if (!RingBuf__Lock(buffer_h))
    return 0;
  
  if (buffer_h->elements_count < buffer_h->buffer_size) {
    ++buffer_h->elements_count;
    RingBuf__Unlock(buffer_h);
    return 1;
  } else if (buffer_h->elements_count == buffer_h->buffer_size) {
    ++buffer_h->tail_index;
    buffer_h->tail_index %= buffer_h->buffer_size;
    RingBuf__Unlock(buffer_h);
    return 1;
  } else {
    RingBuf__Unlock(buffer_h);
    return 0;
  }
}

int RingBuf_OA_ElementDequeued(RingBuf* buffer_h)
{
  if (!buffer_h->_wInit)
    return 0;
  if (!RingBuf__Lock(buffer_h))
    return 0;
  
  if (!(buffer_h->elements_count > 0))
    return 0;
  
  --buffer_h->elements_count;
  ++buffer_h->tail_index;
  buffer_h->tail_index %= buffer_h->buffer_size;
  
  RingBuf__Unlock(buffer_h);
  return 1;
}

int RingBuf__Lock(RingBuf* buffer_h)  // Auxiliary function, not recommended to use
{
  if (!buffer_h->_wInit)
    return 0;
  if (buffer_h->_isLocked)
    return 0;
  
  buffer_h->_isLocked = true;
  return 1;
}

int RingBuf__Unlock(RingBuf* buffer_h)  // Auxiliary function, not recommended to use
{
  if (!buffer_h->_wInit)
    return 0;
  
  buffer_h->_isLocked = false;
  return 1;
}
