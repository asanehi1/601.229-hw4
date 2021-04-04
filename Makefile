
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu99 -fPIC 

imgproc: image.o imgproc.o tile.o mirrorh.o mirrorv.o swapbg.o pnglite.o
	gcc -o $@ image.o imgproc.o tile.o mirrorh.o mirrorv.o swapbg.o pnglite.o

imgproc.o: imgproc.c image.c pnglite.c image_plugin.h
	gcc $(CFLAGS) -c imgproc.c image.c pnglite.c

image.o: image.h image.c
	gcc $(CFLAGS) -c image.c

pnglite.o: pnglite.h pnglite.c
	gcc $(CFLAGS) -c pnglite.c

tile.o: tile.c image_plugin.h
	gcc $(CFLAGS) -c tile.c

expose.o: expose.c image_plugin.h
	gcc $(CFLAGS) -c expose.c

mirrorh.o: mirrorh.c image_plugin.h
	gcc $(CFLAGS) -c mirrorh.c

mirrorv.o: mirrorv.c image_plugin.h
	gcc $(CFLAGS) -c mirrorv.c

swapbg.o: swapbg.c image_plugin.h
	gcc $(CFLAGS) -c swapbg.c

clean:
	rm -f *.o imgproc *~