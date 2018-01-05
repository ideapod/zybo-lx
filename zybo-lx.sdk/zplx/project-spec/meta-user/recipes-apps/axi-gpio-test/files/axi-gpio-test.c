/*
 * This test application is to read/write data directly from/to the device
 * from userspace.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define IN 0
#define OUT 1

/* offsets for AXI_GPIO registers */
#define GPIO_DATA	0x0000
#define GPIO_TRI	0x0004
#define GPIO_2_DATA	0x0008
#define	GPIO_2_TRI	0x000C


/* assume 4 bit width data */
#define DATA_4_BIT_INPUT 0xFF
#define DATA_4_BIT_OUTPUT 0x00

void usage(char *appname)
{
	printf("%s -g <GPIO_ADDRESS> -c <CHANNEL> -i|-o <VALUE>\n",appname);
	printf("    -g <GPIO_ADDR>   AXI GPIO physical base address in Hex: eg 2A3B4800\n");
	printf("    -c <CHANNEL>     AXI GPIO channel number (1 or 2)\n");
	printf("    -i               Input from GPIO\n");
	printf("    -o <VALUE>       Output to GPIO\n");
	printf("%s -g 41200000 -c 1 -i");
	return;
}

int main(int argc, char *argv[])
{
	int c;
	int fd;
	int direction=IN;
	unsigned gpio_addr = 0;
	unsigned gpio_chan = 1;
	unsigned data_offset = 0;
	unsigned ctrl_offset = 0;
	int value = 0;


	unsigned page_addr =0;
	void *ptr;
	unsigned page_offset;
	unsigned page_size=sysconf(_SC_PAGESIZE);

	printf("AXI GPIO access through /dev/mem. page size: %d\n", page_size);

	/* Parse command line arguements */
	while((c = getopt(argc, argv, "g:c:io:h")) != -1) {
		printf("options: token is: %c\n",c);
		switch(c) {
		case 'g':
			gpio_addr=strtoul(optarg,NULL, 16);
			break;
		case 'c':
			gpio_chan=atoi(optarg);
			break;
		case 'i':
			direction=IN;
			break;
		case 'o':
			direction=OUT;
			value=atoi(optarg);
			break;
		case 'h':
			usage(argv[0]);
			return 0;
		default:
			printf("invalid option: %c\n", (char)c);
			usage(argv[0]);
			return -1;
		}

	}

	printf("options: gpio: 0x%08x, channel: %d, direction: %d\n", gpio_addr, gpio_chan, direction);
	
	if (gpio_addr == 0) {
		printf("AXI GPIO physical address is required.\n");
		usage(argv[0]);
		return -1;
	}

	if (gpio_chan == 0) {
		printf("AXI GPIO channel address is required.\n");
		usage(argv[0]);
		return -1;
	}

	
	
	/* Open /dev/mem file */
	fd = open ("/dev/mem", O_RDWR);
	if (fd < 1) {
		perror(argv[0]);
		return -1;
	}

	printf("memory opened ok, page size=%d\n",page_size);


	if (gpio_chan == 1) {
		data_offset = GPIO_DATA;
		ctrl_offset = GPIO_TRI;
	} else {
		data_offset = GPIO_2_DATA;
		ctrl_offset = GPIO_2_TRI;
	}

	printf("channel: %d, data: 0x%08x, ctrl: 0x%08x\n", gpio_chan, data_offset, ctrl_offset);



	/* mmap the device into memory */
	page_addr = (gpio_addr & (~(page_size-1)));
	page_offset = gpio_addr - page_addr;

	ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, page_addr);

	/* set the direction for the channel to in */
	char *register_ptr = ((unsigned *)ptr) + page_offset;
	printf("page addr: 0x%08x, page offset: 0x%08x, ptr: 0x%08x, register pointer: 0x%08x\n", page_addr, page_offset, ptr, register_ptr);
	
	char *ctrl_ptr = register_ptr + ctrl_offset;
	char *data_ptr = register_ptr + data_offset; 

	printf("control pointer: 0x%08x, data pointer 0x%08x\n",ctrl_ptr, data_ptr);
	
	if (direction == IN) {
		printf("Current control register: 0x%08x\n", *ctrl_ptr);
		*ctrl_ptr = DATA_4_BIT_INPUT;
		printf("set to 4 bit input, Current control register: 0x%08x\n", *ctrl_ptr);
		/* Read value from the device register */
		value = *data_ptr;
		printf("axi gpio dev-mem test:  input: 0x%08x\n",value);
	} else {
	/* Write value to the device register */
		/* set the direction for the channel to output */
		printf("Current control register: 0x%08x", *ctrl_ptr);
		*ctrl_ptr = DATA_4_BIT_OUTPUT;
		printf("set to 4 bit OUTPUT, Current control register: 0x%08x\n", *ctrl_ptr);
		printf("writing value: 0x%08x", value);
		/* write value */
		*data_ptr = value;
	}

	printf("complete, unmapping memory\n");
	munmap(ptr, page_size);

	return 0;
}



