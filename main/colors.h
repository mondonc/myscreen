/*Copyright 2013 Barthelemy Delemotte <d.bartouz@gmail.com>

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

#ifndef _COLORS_H
# define _COLORS_H

# if defined(NO_ESCAPE_CODE)
#  define BLACK		""
#  define RED		""
#  define GREEN		""
#  define YELLOW	""
#  define BLUE		""
#  define MAGENTA	""
#  define CYAN		""
#  define WHITE		""
#  define DEFAULT_COLOR ""
#  define UNCHANGED_COLOR ""
#  define BOLD		""
#  define COLOR_END ""

# elif defined(SCREEN_ESCAPE_CODE)
#  define BLACK		"\005{= k}"
#  define RED		"\005{= r}"
#  define GREEN		"\005{= g}"
#  define YELLOW	"\005{= y}"
#  define BLUE		"\005{= b}"
#  define MAGENTA	"\005{= m}"
#  define CYAN		"\005{= c}"
#  define WHITE		"\005{= w}"
#  define DEFAULT_COLOR "\005{= dd}"
#  define UNCHANGED_COLOR "\005{= .}"
#  define BOLD		"\005{+b}"
#  define COLOR_END "\005{-}"

# elif defined(ANSI_ESCAPE_CODE)
#  define BLACK		"\x1b[30m"
#  define RED		"\x1b[31m"
#  define GREEN		"\x1b[32m"
#  define YELLOW	"\x1b[33m"
#  define BLUE		"\x1b[34m"
#  define MAGENTA	"\x1b[35m"
#  define CYAN		"\x1b[36m"
#  define WHITE		"\x1b[37m"
#  define DEFAULT_COLOR	"\x1b[0m"
#  define UNCHANGED_COLOR	""
#  define BOLD		"\x1b[1m"
#  define COLOR_END	DEFAULT_COLOR

# else
#  error "unsupported escape code format"
# endif

#endif /* !_COLORS_H */
