# Target library
lib := libuthread.a

# Don't print the commands unless explicitly requested with `make V=1`
ifneq ($(V),1)
Q = @
V = 0
endif

# Define static library compile commands
AR = ar
RCS = rcs

# Define compilation toolchain
CC	= gcc

# General gcc options
CFLAGS	:= -Wall -Wextra -Werror
## Debug flag
ifneq ($(D),1)
CFLAGS	+= -O2
else
CFLAGS	+= -g
endif
## Dependency generation
CFLAGS	+= -MMD

# Objects to compile
objs := queue.o uthread.o context.o preempt.o

# Include dependencies
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

all: $(lib)

# Build libuthread.a from object files
$(lib): $(objs)
	@echo "CC	$@"
	$(Q)$(AR) $(RCS) $@ $^

# Generic rule for compiling objects
%.o: %.c
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

# Cleaning rule
clean: 
	@echo "CLEAN"
	$(Q)rm -rf $(lib) $(objs) $(deps)

