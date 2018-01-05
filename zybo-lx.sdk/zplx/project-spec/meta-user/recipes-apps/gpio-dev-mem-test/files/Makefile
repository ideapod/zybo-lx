APP = gpio-dev-mem-test

# Add any other object files to this list below
APP_OBJS = gpio-dev-mem-test.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

