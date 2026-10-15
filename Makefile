catlock:
	$(CC) catlock.c -Wall -Wextra -lX11 -o catlock

clean:
	$(RM) catlock
