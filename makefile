TEST_EXEC := test_bin
BUILD_DIR := ./build

SRCS := page.c list.c hashtable.c cache.c tests/test.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

INC_DIRS=-I.
LIBDIRS=-L/ucrt64/lib
LDFLAGS=-lcheck -lm #-lsubunit
CFLAGS=-Wall -std=c11 -O2 $(INC_DIRS) $(LIBDIRS) -MMD -MP


all: $(BUILD_DIR)/$(TEST_EXEC)


$(BUILD_DIR)/$(TEST_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)


$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: all, test, check, clean
test check:
	$(BUILD_DIR)/$(TEST_EXEC)


clean:
	rm -rf $(BUILD_DIR)


# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
