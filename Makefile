all:Main.c Damascus
	arm-linux-gcc -g -Wall -nostartfiles -nostdlib -nodefaultlibs -o Main.bin Main.c \
		-L../Damascus_Sword_S3C2440/Objects -lDamascus \
		-L~/cross_compiler_4.4.3/lib/gcc/arm-none-linux-gnueabi/4.4.3/ -lgcc \
		-I../Damascus_Sword_S3C2440/inc/ \
		-eMain -Wl,-Ttext=0x30008000,-TMain.lds,-Map System.map
	arm-linux-objdump -h -D Main.bin > dis 
	readelf -a Main.bin > elf_message

Damascus:
	cd ../Damascus_Sword_S3C2440 && $(MAKE)

clean:
	rm -f *.o *.bin
		#-I~/cross_compiler_4.4.3/lib/gcc/arm-none-linux-gnueabi/4.4.3/include/ssp/ \,-Tdata=0x3f000000
