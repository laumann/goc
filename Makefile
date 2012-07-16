CC	:= gcc
CFLAGS	:= -Wall
RM	:= rm -rfv
OBJS += goc.o

HDRS += stone.h

goc: $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	@$(RM) $(OBJS) goc
.PHONY: clean
