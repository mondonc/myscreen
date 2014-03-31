/*Copyright 2014 Barthélémy Delemotte <barthelemy.delemotte@gmail.com>

	This file is part of project myscreen.

	myscreen is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	myscreen is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with myscreen.  If not, see http://www.gnu.org/licenses/.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>

#include "msg_queue.h"
#include "myscreen-stats.h"

/*
** PRIVATES SYMBOLS :
*/

#define MAX_MSG_LENGTH 256 /* the length of a message */
#define MAX_MSG_NBR 16 /* capacity of the queue */

/* a message */
typedef		struct
{
  char		buffer[MAX_MSG_LENGTH + 1];
}		msg_t;

/* the queue structure */
typedef		struct
{
  msg_t		queue[MAX_MSG_NBR];
  size_t	push_idx;
  size_t	pop_idx;
  size_t	counter;
}		msg_queue_t;

static bool init = false; /* indicates if the queue has already been used */
static msg_queue_t self; /* the queue itself */

/*
** Set all queue members to zero.
*/
static void _init_msg_queue(void)
{
  IFDEBUG_PRINT("initialize the messages queue");
  memset(&self, 0, sizeof(self));
  init = true;
}

/*
** If the queue is not full, return a pointer to the next available element,
** increasing counter and push_idx.
** Return NULL otherwise.
** Initialize the queue if necessary.
*/
static msg_t *_push()
{
  msg_t *msg = NULL;

  if (!init)
    _init_msg_queue();

  assert(self.counter <= MAX_MSG_NBR);
  assert(self.push_idx < MAX_MSG_NBR);

  if (self.counter < MAX_MSG_NBR)
    {
      msg = &self.queue[self.push_idx];
      self.push_idx = (self.push_idx + 1) % MAX_MSG_NBR;
      self.counter++;
    }
  return msg;
}

/*
** If the queue is not empty, decrease counter, increase pop_idx,
** and return true. Return false otherwise.
** Initialize the queue if necessary.
*/
static bool _pop()
{
  if (!init)
    _init_msg_queue();

  assert(self.counter <= MAX_MSG_NBR);
  assert(self.pop_idx < MAX_MSG_NBR);

  if (self.counter == 0)
    return false;
  self.pop_idx = (self.pop_idx + 1) % MAX_MSG_NBR;
  self.counter--;
  return true;
}

/*
** If the queue is not empty, return a pointer to the next element to print,
** WITHOUT decreasing counter and increasing pop_idx.
** Return NULL otherwise.
** Initialize the queue if necessary.
*/
static msg_t *_peek()
{
  if (!init)
    _init_msg_queue();

  assert(self.counter <= MAX_MSG_NBR);
  assert(self.pop_idx < MAX_MSG_NBR);

  return  self.counter > 0 ? &self.queue[self.pop_idx] : NULL;
}


/*
** VISIBLES SYMBOLS (INTERFACE) :
*/

/*
** Return true if the queue is empty.
*/
bool msg_queue_is_empty(void)
{
  return !init || self.counter == 0;
}

/*
** Push a message on the queue copying the content of the parameters.
** A color equals to null is ignored.
*/
bool msg_queue_push(const char *str, const char *color)
{
  msg_t *msg = _push();
  char * buffer;

  assert(str);
  assert(strlen(str) <= MAX_MSG_LENGTH);

  if (!msg)
    return false;
  buffer = msg->buffer;
  if (color) {
    (void)strncpy(msg->buffer, color, MAX_MSG_LENGTH);
    msg->buffer[MAX_MSG_LENGTH] = '\0';
  }
  (void)strncat(msg->buffer, str, MAX_MSG_LENGTH - (msg->buffer - buffer));
  return true;
}

/*
** Push a message on the queue filling the message content with a snprintf.
*/
bool msg_queue_vpush(const char *format, ...)
{
  msg_t *msg = _push();
  va_list ap;

  assert(format);

  if (!msg)
    return false;
  va_start(ap, format);
  (void)vsnprintf(msg->buffer, MAX_MSG_LENGTH, format, ap);
  va_end(ap);
  return (true);
}

/*
** Return a pointer to the next message to print without remove it, or NULL
** if the queue is empty.
*/
const char *msg_queue_peek(void)
{
  msg_t *msg = _peek();

  return msg ? msg->buffer : NULL;
}

/*
** Remove the older message of the queue.
** Return false if the stack is empty, true otherwise.
*/
bool msg_queue_pop(void)
{
  return _pop();
}
