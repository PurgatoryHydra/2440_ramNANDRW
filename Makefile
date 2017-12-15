SOURCE_DIR=src
OBJECT_DIR=Objects
CC=arm-linux-gcc
CFLAGS=-g -Iinc/ -I../Damascus_Sword_S3C2440/inc

all:Main.c Damascus LocalSrc
	$(CC) -g -Wall -nostartfiles -nostdlib -nodefaultlibs -o Main.bin Main.c \
		-LObjects \
		-L../Damascus_Sword_S3C2440/Objects -lDamascus \
		-L~/cross_compiler_4.4.3/lib/gcc/arm-none-linux-gnueabi/4.4.3/ \
		-lLocalSrc  -lDamascus -lgcc\
		-I../Damascus_Sword_S3C2440/inc/ \
		-Iinc/ \
		-eMain -Wl,-Ttext=0x30008000,-TMain.lds,-Map System.map
	arm-linux-objdump -h -D Main.bin > dis 
	readelf -a Main.bin > elf_message

LocalSrc: modules
	ar crv $(OBJECT_DIR)/libLocalSrc.a $(OBJECT_DIR)/*.o

modules:
	for name in $(wildcard $(SOURCE_DIR)/*.c); \
	do \
		echo "$$name" |\
		awk -F '.' '{print $$1}' |\
	   	awk -F '/' '{print sprintf("%s.o", $$2)}' |\
	   	xargs -I {} $(CC) $(CFLAGS) -c -o $(OBJECT_DIR)/{} $$name; \
	done

Damascus:
	cd ../Damascus_Sword_S3C2440 && $(MAKE)

clean:
	rm -f *.o *.bin
		#-I~/cross_compiler_4.4.3/lib/gcc/arm-none-linux-gnueabi/4.4.3/include/ssp/ \,-Tdata=0x3f000000
