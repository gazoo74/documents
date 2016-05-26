#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>

#define __write(fd, buf) write(fd, buf, sizeof(buf))

int setup_tc(int fd)
{
	struct termios termios;

	if (tcgetattr(fd, &termios) < 0) {
		perror("tcgetattr");
		return -1;
	}

#ifdef SYSTEMD
        /* We only reset the stuff that matters to the software. How
         * hardware is set up we don't touch assuming that somebody
         * else will do that for us */

        termios.c_iflag &= ~(IGNBRK | BRKINT | ISTRIP | INLCR | IGNCR | IUCLC);
        termios.c_iflag |= ICRNL | IMAXBEL | IUTF8;
        termios.c_oflag |= ONLCR;
        termios.c_cflag |= CREAD;
        termios.c_lflag = ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOPRT | ECHOKE;

        termios.c_cc[VINTR]    =   03;  /* ^C */
        termios.c_cc[VQUIT]    =  034;  /* ^\ */
        termios.c_cc[VERASE]   = 0177;
        termios.c_cc[VKILL]    =  025;  /* ^X */
        termios.c_cc[VEOF]     =   04;  /* ^D */
        termios.c_cc[VSTART]   =  021;  /* ^Q */
        termios.c_cc[VSTOP]    =  023;  /* ^S */
        termios.c_cc[VSUSP]    =  032;  /* ^Z */
        termios.c_cc[VLNEXT]   =  026;  /* ^V */
        termios.c_cc[VWERASE]  =  027;  /* ^W */
        termios.c_cc[VREPRINT] =  022;  /* ^R */
        termios.c_cc[VEOL]     =    0;
        termios.c_cc[VEOL2]    =    0;

        termios.c_cc[VTIME]  = 0;
        termios.c_cc[VMIN]   = 1;
#else
#ifdef BUSYBOX
 	/* set control chars */
	termios.c_cc[VINTR] = 3;	/* C-c */
	termios.c_cc[VQUIT] = 28;	/* C-\ */
	termios.c_cc[VERASE] = 127;	/* C-? */
	termios.c_cc[VKILL] = 21;	/* C-u */
	termios.c_cc[VEOF] = 4;	/* C-d */
	termios.c_cc[VSTART] = 17;	/* C-q */
	termios.c_cc[VSTOP] = 19;	/* C-s */
	termios.c_cc[VSUSP] = 26;	/* C-z */

#ifdef __linux__
	/* use line discipline 0 */
	termios.c_line = 0;
#endif

	/* Make it be sane */
#ifndef CRTSCTS
# define CRTSCTS 0
#endif
	/* added CRTSCTS to fix Debian bug 528560 */
	termios.c_cflag &= CBAUD | CBAUDEX | CSIZE | CSTOPB | PARENB | PARODD | CRTSCTS;
	termios.c_cflag |= CREAD | HUPCL | CLOCAL;

	/* input modes */
	termios.c_iflag = ICRNL | IXON | IXOFF;

	/* output modes */
	termios.c_oflag = OPOST | ONLCR;

	/* local modes */
	termios.c_lflag = ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE | IEXTEN;

//	tcsetattr_stdin_TCSANOW(&termios);
#else
#ifdef UPSTART
	termios.c_cflag &= (CBAUD | CBAUDEX | CSIZE | CSTOPB
			| PARENB | PARODD);
	termios.c_cflag |= (HUPCL | CLOCAL | CREAD);

	/* Set up usual keys */
	termios.c_cc[VINTR]  = 3;   /* ^C */
	termios.c_cc[VQUIT]  = 28;  /* ^\ */
	termios.c_cc[VERASE] = 127;
	termios.c_cc[VKILL]  = 24;  /* ^X */
	termios.c_cc[VEOF]   = 4;   /* ^D */
	termios.c_cc[VTIME]  = 0;
	termios.c_cc[VMIN]   = 1;
	termios.c_cc[VSTART] = 17;  /* ^Q */
	termios.c_cc[VSTOP]  = 19;  /* ^S */
	termios.c_cc[VSUSP]  = 26;  /* ^Z */

	/* Pre and post processing */
	termios.c_iflag = (IGNPAR | ICRNL | IXON | IXANY);
	termios.c_oflag = (OPOST | ONLCR);
	termios.c_lflag = (ISIG | ICANON | ECHO | ECHOCTL
			| ECHOPRT | ECHOKE);
#else
	termios.c_cflag &= (CBAUD | CBAUDEX | CSIZE | CSTOPB
			| PARENB | PARODD);
	termios.c_cflag |= (HUPCL | CLOCAL | CREAD);

	/* Set up usual keys */
	termios.c_cc[VINTR]  = 3;   /* ^C */
	termios.c_cc[VQUIT]  = 28;  /* ^\ */
	termios.c_cc[VERASE] = 127;
	termios.c_cc[VKILL]  = 24;  /* ^X */
	termios.c_cc[VEOF]   = 4;   /* ^D */
	termios.c_cc[VTIME]  = 0;
	termios.c_cc[VMIN]   = 1;
	termios.c_cc[VSTART] = 17;  /* ^Q */
	termios.c_cc[VSTOP]  = 19;  /* ^S */
	termios.c_cc[VSUSP]  = 26;  /* ^Z */

	/* Pre and post processing */
	termios.c_iflag = (IGNPAR | ICRNL | IXON | IXANY);
	termios.c_oflag = (OPOST | ONLCR);
	termios.c_lflag = (ISIG | ICANON | ECHO | ECHOCTL
			| ECHOPRT | ECHOKE);
#endif
#endif
#endif

        if (tcsetattr(fd, TCSANOW, &termios) < 0) {
		perror("tcsetattr");
		return -1;
	}

        if (tcflush(fd, TCIOFLUSH) < 0) {
		perror("tcflush");
		return -1;
	}

	return 0;
}

int main(int argc, char * const argv[])
{
	int fd = open("/dev/console", O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd == -1)
		return EXIT_FAILURE;

	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);

	if (fd > STDERR_FILENO)
		close(fd);

	ioctl(fd, TIOCSCTTY, 1);

	__write(STDOUT_FILENO, "Hello, world!\n");
	__write(STDERR_FILENO, "Hello, world!\n");
	
	for (;;)
		printf("Hello, world!!\n");

	return EXIT_SUCCESS+0x100;
}
