#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

#define END 0

struct options {
	const char    opt;
	unsigned char val;
	unsigned char shift;
};

#define L_SHIFT 0x02

// 字符码表
static struct options kval[] = {
	/*   char       code      shift? */
	{.opt = 'a',.val = 0x04,.shift = 0},
	{.opt = 'b',.val = 0x05,.shift = 0},
	{.opt = 'c',.val = 0x06,.shift = 0},
	{.opt = 'd',.val = 0x07,.shift = 0},
	{.opt = 'e',.val = 0x08,.shift = 0},
	{.opt = 'f',.val = 0x09,.shift = 0},
	{.opt = 'g',.val = 0x0a,.shift = 0},
	{.opt = 'h',.val = 0x0b,.shift = 0},
	{.opt = 'i',.val = 0x0c,.shift = 0},
	{.opt = 'j',.val = 0x0d,.shift = 0},
	{.opt = 'k',.val = 0x0e,.shift = 0},
	{.opt = 'l',.val = 0x0f,.shift = 0},
	{.opt = 'm',.val = 0x10,.shift = 0},
	{.opt = 'n',.val = 0x11,.shift = 0},
	{.opt = 'o',.val = 0x12,.shift = 0},
	{.opt = 'p',.val = 0x13,.shift = 0},
	{.opt = 'q',.val = 0x14,.shift = 0},
	{.opt = 'r',.val = 0x15,.shift = 0},
	{.opt = 's',.val = 0x16,.shift = 0},
	{.opt = 't',.val = 0x17,.shift = 0},
	{.opt = 'u',.val = 0x18,.shift = 0},
	{.opt = 'v',.val = 0x19,.shift = 0},
	{.opt = 'w',.val = 0x1a,.shift = 0},
	{.opt = 'x',.val = 0x1b,.shift = 0},
	{.opt = 'y',.val = 0x1c,.shift = 0},
	{.opt = 'z',.val = 0x1d,.shift = 0},
	
	{.opt = 'A',.val = 0x04,.shift = 1},
	{.opt = 'B',.val = 0x05,.shift = 1},
	{.opt = 'C',.val = 0x06,.shift = 1},
	{.opt = 'D',.val = 0x07,.shift = 1},
	{.opt = 'E',.val = 0x08,.shift = 1},
	{.opt = 'F',.val = 0x09,.shift = 1},
	{.opt = 'G',.val = 0x0a,.shift = 1},
	{.opt = 'H',.val = 0x0b,.shift = 1},
	{.opt = 'I',.val = 0x0c,.shift = 1},
	{.opt = 'J',.val = 0x0d,.shift = 1},
	{.opt = 'K',.val = 0x0e,.shift = 1},
	{.opt = 'L',.val = 0x0f,.shift = 1},
	{.opt = 'M',.val = 0x10,.shift = 1},
	{.opt = 'N',.val = 0x11,.shift = 1},
	{.opt = 'O',.val = 0x12,.shift = 1},
	{.opt = 'P',.val = 0x13,.shift = 1},
	{.opt = 'Q',.val = 0x14,.shift = 1},
	{.opt = 'R',.val = 0x15,.shift = 1},
	{.opt = 'S',.val = 0x16,.shift = 1},
	{.opt = 'T',.val = 0x17,.shift = 1},
	{.opt = 'U',.val = 0x18,.shift = 1},
	{.opt = 'V',.val = 0x19,.shift = 1},
	{.opt = 'W',.val = 0x1a,.shift = 1},
	{.opt = 'X',.val = 0x1b,.shift = 1},
	{.opt = 'Y',.val = 0x1c,.shift = 1},
	{.opt = 'Z',.val = 0x1d,.shift = 1},
	
	{.opt = '1',.val = 0x1e,.shift = 0},
	{.opt = '2',.val = 0x1f,.shift = 0},
	{.opt = '3',.val = 0x20,.shift = 0},
	{.opt = '4',.val = 0x21,.shift = 0},
	{.opt = '5',.val = 0x22,.shift = 0},
	{.opt = '6',.val = 0x23,.shift = 0},
	{.opt = '7',.val = 0x24,.shift = 0},
	{.opt = '8',.val = 0x25,.shift = 0},
	{.opt = '9',.val = 0x26,.shift = 0},
	{.opt = '0',.val = 0x27,.shift = 0},

	{.opt = '!',.val = 0x1e,.shift = 1},
	{.opt = '@',.val = 0x1f,.shift = 1},
	{.opt = '#',.val = 0x20,.shift = 1},
	{.opt = '$',.val = 0x21,.shift = 1},
	{.opt = '%',.val = 0x22,.shift = 1},
	{.opt = '^',.val = 0x23,.shift = 1},
	{.opt = '&',.val = 0x24,.shift = 1},
	{.opt = '*',.val = 0x25,.shift = 1},
	{.opt = '(',.val = 0x26,.shift = 1},
	{.opt = ')',.val = 0x27,.shift = 1},

	{.opt = '-',.val = 0x2d,.shift = 0},
	{.opt = '=',.val = 0x2e,.shift = 0},
	{.opt = '[',.val = 0x2f,.shift = 0},
	{.opt = ']',.val = 0x30,.shift = 0},
	{.opt = '\\',.val = 0x31,.shift = 0},
	{.opt = ';',.val = 0x33,.shift = 0},
	{.opt = '\'',.val = 0x34,.shift = 0},
	{.opt = '`',.val = 0x35,.shift = 0},
	{.opt = ',',.val = 0x36,.shift = 0},
	{.opt = '.',.val = 0x37,.shift = 0},
	{.opt = '/',.val = 0x38,.shift = 0},

	{.opt = '_',.val = 0x2d,.shift = 1},
	{.opt = '+',.val = 0x2e,.shift = 1},
	{.opt = '{',.val = 0x2f,.shift = 1},
	{.opt = '}',.val = 0x30,.shift = 1},
	{.opt = '|',.val = 0x31,.shift = 1},
	{.opt = ':',.val = 0x33,.shift = 1},
	{.opt = '"',.val = 0x34,.shift = 1},
	{.opt = '~',.val = 0x35,.shift = 1},
	{.opt = '<',.val = 0x36,.shift = 1},
	{.opt = '>',.val = 0x37,.shift = 1},
	{.opt = '?',.val = 0x38,.shift = 1},

	{.opt = '\n',.val = 0x28,.shift = 0},
	{.opt = ' ',.val = 0x2c,.shift = 0},
	{.opt = END}
};

int main(int argc, const char *argv[])
{
	const char *filename = NULL;
	int fd = 0;
	char report[8];
	int to_send = 8;
	fd_set rfds;
	int retval;
	char c;
	char i;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s devname\n",
			argv[0]);
		return 1;
	}

	filename = argv[1];

	if ((fd = open(filename, O_RDWR, 0666)) == -1) {
		perror(filename);
		return 3;
	}

	while (42) {
		FD_ZERO(&rfds);
      		FD_SET(STDIN_FILENO, &rfds);
		FD_SET(fd, &rfds);

		retval = select(fd + 1, &rfds, NULL, NULL, NULL);
		if (retval == -1 && errno == EINTR)
			continue;
		if (retval < 0) {
			perror("select()");
			return 4;
		}
		
		memset(report, 0x0, sizeof(report));
		c = getchar();
		// on keyboard
		if (c == '\n' || c == '\r' || (c >= 32 && c <= 126)) { 
			for (i = 0; kval[i].opt != END; i++) {
				if (c == kval[i].opt) {
					if (kval[i].shift == 1) {
						report[0] = L_SHIFT;
					}
					report[2] = kval[i].val;
					break;
				}
			}
			if (write(fd, report, to_send) != to_send) {
				perror(filename);
				return 5;
			}
			memset(report, 0x0, sizeof(report));
			if (write(fd, report, to_send) != to_send) {
				perror(filename);
				return 6;
			}
		}
		else {
			fprintf(stderr, "Illegal character detected, stop!\n");
			return 7;
		}
	}

	close(fd);
	return 0;
}
