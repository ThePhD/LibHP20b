PATH := usr/local/bin:opt/local/bin:$(PATH)

#OPENOCDPATH = /opt/local/bin/
#CROSSPATH = /usr/local/bin/
#OPENOCDPATH = /opt/local/bin/
#CROSSPATH = /usr/bin/

OPENOCD = $(OPENOCDPATH)openocd
CROSS = $(CROSSPATH)arm-linux-gnueabi-

CC = gcc

CFLAGS = -Wall -Dat91sam7l128 -mcpu=arm7tdmi -marm

USE_PRIVATE_LIBGCC = yes

LDFLAGS = -nostdlib -Wl,--gc-sections -static
# -Wl,--print-map
#-Wl,--verbose 

LDSCRIPT = flash.lds

TARGET = libhp20b

TARGETPROPER = LibHP20b

TARGETEXT = .bin

SRC = main.c Number.c Decimal.c StandardDef.c Standard.c Core.c HP20b.c HP20b_LCD.c HP20b_Keyboard.c

HEADERS = Architecture.h AT91SAM7L128.h StandardDef.h Number.h Decimal.h Standard.h Core.h HP20b_LCD.h HP20b_Keyboard.h HP20b.h openocd.cfg\
	board/hp-20b-calculator.cfg target/at91sam7l128.cfg

OBJS = crt0.o $(SRC:.c=.o)

TARFILES = Makefile NMakefile $(LDSCRIPT) crt0.sx $(SRC) $(HEADERS) HP20bRemix.xcodeproj LibHP20bTest.xcodeproj HP20bRemix.sln LibHP20b.vcxproj LibHP20b.vcxproj.user LibHP20b.vcxproj.filters LibHP20bTest.vcxproj LibHP20bTest.vcxproj.user LibHP20bTest.vcxproj.filters .project .cproject

LIBS = -lgcc

DELCOM = rm -rf
COMPRESSCOM = cd .. && tar zcf $(TARGETPROPER)/$@ $(TARFILES:%=$(TARGETPROPER)/%)
COMPRESSEXT = .tar.gz

all : $(TARGET)$(TARGETEXT)

.PHONY : compress
compress : $(TARGETPROPER)$(COMPRESSEXT)

$(TARGET)$(TARGETEXT) : $(SRC) $(HEADERS)

.PHONY : flash
flash : $(TARGET)$(TARGETEXT)
	$(OPENOCD) -f openocd.cfg \
	-c "init" \
	-c "write_flash $(TARGET)$(TARGETEXT)" \
	-c "reset" \
	-c "resume" \
	-c "shutdown"

$(TARGET).elf : $(OBJS) $(LDSCRIPT)
	$(CROSS)$(CC) $(LDFLAGS) -T$(LDSCRIPT) -o $(TARGET).elf $(OBJS) $(LIBS)

%$(TARGETEXT) : %.elf
	$(CROSS)objcopy -O ihex $< $@

%.o : %.c
	$(CROSS)$(CC) $(CFLAGS) -c $<

%.o : %.sx
	$(CROSS)$(CC) $(CFLAGS) -c -x assembler-with-cpp $<


.PHONY : $(TARGETPROPER)$(COMPRESSEXT)
$(TARGETPROPER)$(COMPRESSEXT) : $(TARFILES)
	$(COMPRESSCOM)

.PHONY : clean
clean :
	$(DELCOM) *.o *.elf *$(TARGETEXT) *$(COMPRESSEXT) Compress/ Debug/ Flash/
