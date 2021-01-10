#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <bcm2835.h>

void mini_uart_sendstr (const char *s)
{
  char c;
  while((c = *s++)) bcm2835_uart_send(c);
}

int main(int argc, char **argv) {


	printf("bcm2835_set_debug\n");
	bcm2835_set_debug(0);

	printf("bcm2835_init\n");
	bcm2835_init();

	printf("bcm2835_uart_begin\n");
	bcm2835_uart_begin();

	printf("mini_uart_sendstr\n");
	mini_uart_sendstr("Raspberry Pi\n");

	char c = '!';

	while ((c = bcm2835_uart_receive()) != ' ')
		bcm2835_uart_send(c);

	printf("bcm2835_uart_end\n");
	bcm2835_uart_end();

	printf("bcm2835_close\n");
	bcm2835_close();

	return 0;
}
