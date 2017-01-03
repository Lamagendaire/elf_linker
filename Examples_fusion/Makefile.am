# no code transformations to work with thumb
CFLAGS=-mno-thumb-interwork

if HAVE_ARM_COMPILER
all-am: fusion_result

fusion_result: fusion_result.o
	$(CC) -o $@ $<

fusion_result.o: file1.o file2.o
	$(LD_CROSS) -r -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
endif

clean-local:
	rm -f *.o fusion_result

EXTRA_DIST=file1.c file2.c
