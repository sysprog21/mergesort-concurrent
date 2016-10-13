CC = gcc
CFLAGS = -std=gnu11 -Wall -g -pthread
OBJS = list.o threadpool.o merge_sort.o main.o

.PHONY: all clean test

GIT_HOOKS := .git/hooks/pre-commit

all: $(GIT_HOOKS) sort

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

deps := $(OBJS:%.o=.%.o.d)
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -MMD -MF .$@.d -c $<

sort: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -rdynamic

genData:
	uniq test_data/words.txt | sort -R > test_data/input.txt

clean:
	rm -f $(OBJS) sort
	@rm -rf $(deps)

-include $(deps)
