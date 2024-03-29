#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stone.h"

/* TODO Set up management of groups using (doubly) linked lists, define merging, etc... */
struct __group {
	struct __stone **st;
};

/* Two dimensional array of pointers to stones */
struct __stone ***board;

/* Global configuration */
static struct __config {
	int size;
} config = {
	.size = 13, /* Default board size */
};

void panic(const char *msg)
{
	fprintf(stderr, "Panic! %s\n", msg);
	exit(EXIT_FAILURE);
}

void exit_missing_arg(const char *arg)
{
	fprintf(stderr, "Missing argument for: %s\n", arg);
	exit(EXIT_FAILURE);
}

/* Git inspired option handling */
void parse_cmd_args(int *argc, const char **argv[])
{
	const char *arg;

	while (*argc > 0) {
		arg = (*argv)[0];

		if (!strcmp(arg, "-s") || !strcmp(arg, "--size")) {
			/* TODO Figure out why providing a different size results in a segfault */
			if (*argc < 2)
				exit_missing_arg(arg);

			(*argc)--;
			(*argv)++;
			if (!sscanf(**argv, "%d", &config.size)) {
				fprintf(stderr, "%s requires an integer argument.\n", arg);
				exit(EXIT_FAILURE);
			}
		} else {
			fprintf(stderr, "Unknown option: %s\n", arg);
		}

		(*argv)++;
		(*argc)--;
	}
}

/* Set up the required data structures, based on the configuration */
void setup()
{
	printf("Board is of size %dx%d\n", config.size, config.size);
	board = (struct __stone***)malloc(config.size * sizeof(struct __stone**));

	int i = 0;
	while (i < config.size)
		board[i++] = (struct __stone**)malloc(config.size * sizeof(struct __stone*));
}

void teardown()
{
	int i = 0;
	while (i < config.size)
		free(board[i++]);
	free(board);
}

#define init_stone(s, c) do { 					\
	s = (struct __stone*)malloc(sizeof(struct __stone*));	\
	if (!s)							\
		panic("Stone could not be initialised!");	\
	s->color = c;						\
} while(0)

#define friends(s, t)	(s->color == t->color)

int can_place(int x, int y, short color)
{
	/* step thru (x-1,y), (x,y-1), (x+1,y), (x,y+1) */
	struct __stone *s;
	
	if (x-1 >= 0) {
		s = board[x-1][y];
		if (!s || s->color == color)
			return 1;
	}
	if (x+1 < config.size) {
		s = board[x+1][y];
		if (!s || s->color == color)
			return 1;
	}
	if (y-1 >= 0) {
		s = board[x][y-1];
		if (!s || s->color == color)
			return 1;
	}
	if (y+1 < config.size) {
		s = board[x][y+1];
		if (!s || s->color == color)
			return 1;
	}
	return 0;
}

/* Return 1 for successfully placing a stone, 0 otherwise */
int place_stone(struct __stone *s, int x, int y)
{
	/* Sanity check */
	if (x < 0 || x >= config.size || y < 0 || y >= config.size) {
		fprintf(stderr, "Given coordinates out of bounds: (%d, %d)\n", x+1, y+1);
		goto fail;
	}

	if (board[x][y]) {
		fprintf(stderr, "Coordinates (%d,%d) already occupied\n", x+1, y+1);
		goto fail;
	}

	/* TODO Figure out how many liberties, friends and foes there are. */

	/* Hypothetically add the stone, compute the new setup and if the stone doesn't have any
	 * liberties afterwards, then it's an illegal move.
	 */

	/* TODO It is not as simple as this... */
	if (!can_place(x, y, s->color)) {
		fprintf(stderr, "The stone placed there will not have any liberties, which is not allowed.\n");
		goto fail;
	}

	board[x][y] = s;

	return 1;

fail:
	/* If placing a stone fails, return it to the system. */
	free(s);
	return 0;
}

void printboard()
{
	int i, j;
	struct __stone *s;

	/* Print banner */
	printf("   ");
	for (i = 0; i < config.size; i++)
		printf("%3d", (i+1));
	printf("\n");

	/* Print board */
	for (i = 0; i < config.size; i++) {
		printf("%3d", (i+1));
		for (j = 0; j < config.size; j++) {
			s = board[i][j];
			printf("%3s", s ? (s->color ? "w" : "b") : "  ·");
		}
		printf("\n");
	}
}

int main(int argc, const char *argv[])
{
	argc--;
	argv++;
	parse_cmd_args(&argc, &argv);

	printf("Board size: %d\n", config.size);

	setup();

	printf("Beginning game; please enter the coordinates on which to place the next stone like this:\n");
	printf("[color]: x y\n");
	printf("(color will be either BLACK or WHITE). Let the game begin!\n\n");

	int x, y;
	int bw = BLACK;
	char buf[32], *nl;

	for (;;) {
		/* Prompt */
		printf("[%s]: ", iswhite(bw) ? "WHITE" : "BLACK");
	
		if (!fgets(buf, 32, stdin)) {
			fprintf(stderr, "Error reading input\n");
			continue;
		}

		if (nl = strchr(buf, '\n'))
			*nl = '\0';

		if (!strlen(buf))
			continue;

		if (!strcmp(buf, "q") || !strcmp(buf, "quit"))
			break;

		if (sscanf(buf, "%d %d\n", &x, &y) != 2) {
			printf("Failed to get coordinates.\n");
			continue;
		}
		
		struct __stone *n;
		init_stone(n, bw);

		if (!place_stone(n, x-1, y-1))
			continue;

		/* Do inspection here... */
		printboard();

		/* Swapping */
		bw = iswhite(bw) ? BLACK : WHITE;
	}
	printf("Goodbye!\n");
	teardown();

	return 0;
}
