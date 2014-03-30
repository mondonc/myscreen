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

#ifndef _MSG_QUEUE_H
# define _MSG_QUEUE_H

# include <stdbool.h>

bool		msg_queue_is_empty(void);
bool		msg_queue_push(const char *str, const char *color);
bool		msg_queue_vpush(const char *format, ...);
const char	*msg_queue_peek(void);
bool		msg_queue_pop(void);

#endif /* !_MSG_QUEUE_H */
