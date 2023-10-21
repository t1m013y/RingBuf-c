/*
  RingBuf-c
  v1.2
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
  
  buffer_h->elements_count = 0;
  
  return 1;
}

int RingBuf_Queue(RingBuf* buffer_h, const char data)
{
  if (!buffer_h->_wInit)
    return 0;
  
  if (buffer_h->elements_count < buffer_h->buffer_size) {
    char* write_addr = (char*)(buffer_h->buffer + (buffer_h->tail_index + buffer_h->elements_count) % buffer_h->buffer_size);
    
    *write_addr = data;
    ++buffer_h->elements_count;
    
    return 1;
  } else if (buffer_h->elements_count == buffer_h->buffer_size) {
    char* write_addr = (char*)(buffer_h->buffer + buffer_h->tail_index);
    
    ++buffer_h->tail_index;
    buffer_h->tail_index %= buffer_h->buffer_size;
    *write_addr = data;
    
    return 1;
  } else {
    return 0;
  }
}

size_t RingBuf_QueueArr(RingBuf* buffer_h, const char * data, size_t size)
{
  if (!buffer_h->_wInit)
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
  
  return bytes_written;
}

int RingBuf_Dequeue(RingBuf* buffer_h, char* data)
{
  if (!buffer_h->_wInit)
    return 0;
  
  if (!(buffer_h->elements_count > 0))
    return 0;
  
  char* read_addr = (char*)(buffer_h->buffer + buffer_h->tail_index);
  
  char dequeued_byte = *read_addr;
  
  --buffer_h->elements_count;
  ++buffer_h->tail_index;
  buffer_h->tail_index %= buffer_h->buffer_size;
  
  if (data != NULL)
    *data = dequeued_byte;
  
  return 1;
}

size_t RingBuf_DequeueArr(RingBuf* buffer_h, char* data, size_t size)
{
  if (!buffer_h->_wInit)
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
