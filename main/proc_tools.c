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

#include <myscreen-stats.h> 

/*
 * ** move 'line' pointer to the next word of the line, returning it
 * */
const char *jump_separators(const char *line) {

	while (*line && is_proc_separators(*line))
		line++;
	return line;
}

/*
 * ** move 'line' pointer to the next sepatator_char of the line, returning it
 * */
const char *jump_info(const char *line) {

	while (isnot_proc_separators(*line))
		line++;
	return line;
}



/*
 * ** copy at most 'size' characters of the line in buffer.
 * ** including null byte at the end of 'buffer'
 * ** return the moved 'line' pointer, at the first separator character 
 * */
const char *copy_info(const char *line, char *buffer, size_t size) {

	size_t i;

	/* if 'line' is pointing to the end */
	IFDEBUG(if (*line == '\0')
		  fprintf(stderr, "Error : reading a proc file: missing informations\n"));


	/* copy the word-info */
	for (i = 0; i < size - 1 && isnot_proc_separators(line[i]); i++)
		buffer[i] = line[i];
	buffer[i] = '\0';
	line += i;

	/* if buffer is too small to copy the entire word */
	if (i >= size - 1 && isnot_proc_separators(line[size - i - 1])) {
	        IFDEBUG(fprintf(stderr, "Error : reading a proc file: too long word size for '%s[...]'\n", buffer));
		/* move 'line' to the end of the word */
		while (isnot_proc_separators(*line))
			line++;
	}

	return line;
}
