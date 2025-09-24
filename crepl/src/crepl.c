#include "crepl.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void loop() {
	char *line;
	while ((line = readline("> ")) != NULL) {
		if (!line || !*line) add_history(line);

		printf("%s\n", line);

		free(line);
	}
}

int main() {
	loop();

	return 0;
}
