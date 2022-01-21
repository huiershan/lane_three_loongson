#release=DEBUG
#release=RELEASE
CROSS_COMPILE := mipsel-linux-
#CROSS_COMPILE := 
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
INCLUDE := $(COMPLIE_ROOT)/include
DEF=-D_REENTRANT
ifeq ($(release),DEBUG)
    OBJDIR= debug
    TARGET= debug/lane_app 
    DEF += -DDEBUG
    CFLAGS = -O3 -Wall -DDEBUG
else
    OBJDIR= release
    CFLAG= -O3 -Wall
    TARGET= release/lane_app 
endif
obj= main.o sington.o ini_d.o lockfile.o application.o error_use.o  sysutil_d.o tm1650.o gpio_input.o relay_output.o local_ctrl.o remote_ctrl.o modbus_server.o config.o adc7606.o serial.o
OBJECT :=$(addprefix $(OBJDIR)/,$(obj))
LIB= -lpthread -lm -L ./libmodbus -lmodbus
#-lsqlite3
#CFLAGS =  -O3 -Wall -DMEMWATCH -DM_STDIO
CFLAGS =  -O3 -Wall
all:$(TARGET)
$(TARGET):$(OBJECT)
	$(CC) -o $@ $^ $(LIB) $(CFLAGS) $(DEF)
$(OBJDIR)/%.o:%.c
	$(CC) -c -o $@ $^ $(CFLAGS) $(DEF)
.PHONY:clean
clean:
	-rm -f	 $(OBJECT) $(TARGET)
