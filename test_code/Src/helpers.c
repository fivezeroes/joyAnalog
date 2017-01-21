#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"
#include "shared.h"

#define INT32MAX (0x7FFFFFFF)

int32_t linear_buf_init(linear_buf *lb)
{
  if(lb == NULL)
    return 1;
  linear_buf_reset(lb);
  lb->last_recv = 0;
  return 0;
}

int32_t linear_buf_reset(linear_buf *lb)
{
  if(lb == NULL)
    return 1;
  lb->curr_index = 0;
  memset(lb->buf, 0, LB_SIZE);
  return 0;
}

int32_t linear_buf_add(linear_buf *lb, char c)
{
  if(lb == NULL)
    return 1;
  lb->buf[lb->curr_index] = c;
  if(lb->curr_index < LB_SIZE)
    lb->curr_index++;
  lb->last_recv = HAL_GetTick();
  return 0;
}

int32_t linear_buf_add_str(linear_buf *lb, char *s, uint32_t len)
{
  if(lb == NULL || s == NULL)
    return 1;
  for(uint32_t i = 0; i < len; i++)
    linear_buf_add(lb, s[i]);
  lb->buf[LB_SIZE-1] = 0;
  return 0;
}

int32_t linear_buf_line_available(linear_buf *lb)
{
  if(lb == NULL || lb->curr_index <= 0)
    return 0;
  if(lb->curr_index >= LB_SIZE)
  {
    linear_buf_reset(lb);
    return 0;
  }
  if(lb->buf[lb->curr_index - 1] == '\n')
    return 1;
  return 0;
}

int32_t linear_buf_idle(linear_buf *lb)
{
  if(lb == NULL || lb->curr_index <= 0)
    return 0;
  if(lb->curr_index >= LB_SIZE)
  {
    linear_buf_reset(lb);
    return 0;
  }
  if(HAL_GetTick() - lb->last_recv > 10)
    return 1;
  return 0;
}

int32_t get_arg(char* argstr, uint32_t index)
{
  if(argstr == NULL)
    return INT32MAX;

  char* str_end = argstr + strlen(argstr);
  char* curr = argstr;
  int32_t count = 0;

  while(curr < str_end && *curr != ' ')
    curr++;
  curr++;
  if(curr >= str_end)
    return INT32MAX - 1;
  if(index == 0)
    return atoi(curr);

  while(curr < str_end)
  {
    if(*curr == ' ')
      count++;
    if(count == index)
      break;
    curr++;
  }
  curr++;
  if(curr >= str_end)
    return INT32MAX - 2;
  return atoi(curr);
}
