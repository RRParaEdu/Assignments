C = gcc
WRAP_FLAGS = -fno-builtin -Wl,--wrap=pthread_mutex_lock -Wl,--wrap=pthread_mutex_unlock -Wl,--wrap=pthread_cond_wait -Wl,--wrap=pthread_cond_signal -Wl,--wrap=pthread_cond_broadcast -Wl,--wrap=printf -Wl,--wrap=pthread_create
C_FLAGS = -g -Wall -pthread $(WRAP_FLAGS)
LD_FLAGS = -L../Pthread_Hooks/
LIBS = -lm
SOURCES = cond_var_bug1.c
OBJECTS = $(SOURCES:%.c=%.o)
BINS = $(OBJECTS:%.o=%.bin)
PHOOKS_SOURCES = ../Pthread_Hooks/pthread_hooks.c
PHOOKS_OBJECTS = $(PHOOKS_SOURCES:%.c=%.o)
DEPENDS = ../Pthread_Hooks/pthread_hooks.h

all: $(SOURCES) $(OBJECTS) $(PHOOKS_OBJECTS) $(BINS) 

%.o: %.c $(DEPENDS) 
	$(CC) -c -o $@ $< $(C_FLAGS)

%.bin : %.o $(PHOOKS_OBJECTS)
	$(CC) $(C_FLAGS) $(LD_FLAGS) -o $@ $^ $(LIBS)

clean:
	rm $(BINS) $(OBJECTS) $(PHOOKS_OBJECTS)
