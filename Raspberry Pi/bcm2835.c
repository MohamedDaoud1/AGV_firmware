*** /opt/bcm2835-1.25/src/bcm2835.c	2013-03-22 00:47:13.000000000 +0100
--- /opt/data/development/workspace/bcm2835/src/bcm2835.c	2013-04-19 16:17:49.000000000 +0200
***************
*** 32,39 ****
  volatile uint32_t *bcm2835_spi0 = MAP_FAILED;
  volatile uint32_t *bcm2835_bsc0 = MAP_FAILED;
  volatile uint32_t *bcm2835_bsc1 = MAP_FAILED;
! volatile uint32_t *bcm2835_st	= MAP_FAILED;
! 
  
  // This variable allows us to test on hardware other than RPi.
  // It prevents access to the kernel memory, and does not do any peripheral access
--- 32,39 ----
  volatile uint32_t *bcm2835_spi0 = MAP_FAILED;
  volatile uint32_t *bcm2835_bsc0 = MAP_FAILED;
  volatile uint32_t *bcm2835_bsc1 = MAP_FAILED;
! volatile uint32_t *bcm2835_st	 = MAP_FAILED;
! volatile uint32_t *bcm2835_aux  = MAP_FAILED;
  
  // This variable allows us to test on hardware other than RPi.
  // It prevents access to the kernel memory, and does not do any peripheral access
***************
*** 896,901 ****
--- 896,956 ----
  	;
  }
  
+ void bcm2835_uart_begin(void) {
+     volatile uint32_t* enables = bcm2835_aux + BCM2835_AUX_ENABLES/4;
+     volatile uint32_t* ier     = bcm2835_aux + BCM2835_AUX_MU_IER_REG/4;
+     volatile uint32_t* cntl    = bcm2835_aux + BCM2835_AUX_MU_CNTL_REG/4;
+     volatile uint32_t* lcr     = bcm2835_aux + BCM2835_AUX_MU_LCR_REG/4;
+     volatile uint32_t* mcr     = bcm2835_aux + BCM2835_AUX_MU_MCR_REG/4;
+     volatile uint32_t* iir     = bcm2835_aux + BCM2835_AUX_MU_IIR_REG/4;
+     volatile uint32_t* baud    = bcm2835_aux + BCM2835_AUX_MU_BAUD_REG/4;
+ 
+     bcm2835_peri_write_nb(enables, 1);
+     bcm2835_peri_write_nb(ier, 0);
+     bcm2835_peri_write_nb(cntl, 0);
+     bcm2835_peri_write_nb(lcr, 0x03);
+     bcm2835_peri_write_nb(mcr, 0);
+     bcm2835_peri_write_nb(ier, 0);
+     bcm2835_peri_write_nb(iir, 0xC6);
+     bcm2835_peri_write_nb(baud, 270);
+ 
+     // Set the GPI0 pins to the Alt 5 function to enable UART1 access on them
+     bcm2835_gpio_fsel(RPI_V2_GPIO_P1_08, BCM2835_GPIO_FSEL_ALT5); // UART1_TXD
+     bcm2835_gpio_fsel(RPI_V2_GPIO_P1_10, BCM2835_GPIO_FSEL_ALT5); // UART1_RXD
+ 
+     // Disable pull-up/down
+     bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_08, BCM2835_GPIO_PUD_OFF);
+     bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_10, BCM2835_GPIO_PUD_OFF);
+ 
+     bcm2835_peri_write_nb(cntl, 0x03);
+ }
+ 
+ void bcm2835_uart_end(void) {
+     // Set the GPI0 pins to the Alt 0 function to enable UART0 access on them (default)
+     bcm2835_gpio_fsel(RPI_V2_GPIO_P1_08, BCM2835_GPIO_FSEL_ALT0); // UART0_TXD
+     bcm2835_gpio_fsel(RPI_V2_GPIO_P1_10, BCM2835_GPIO_FSEL_ALT0); // UART0_RXD
+ }
+ 
+ void bcm2835_uart_send(uint32_t c) {
+ 	volatile uint32_t* lsr = bcm2835_aux + BCM2835_AUX_MU_LSR_REG / 4;
+ 	volatile uint32_t* io = bcm2835_aux + BCM2835_AUX_MU_IO_REG / 4;
+ 
+ 	while ((bcm2835_peri_read(lsr) & 0x20) == 0)
+ 		;
+ 
+ 	bcm2835_peri_write_nb(io, c);
+ }
+ 
+ uint32_t bcm2835_uart_receive(void) {
+ 	volatile uint32_t* lsr = bcm2835_aux + BCM2835_AUX_MU_LSR_REG / 4;
+ 	volatile uint32_t* io = bcm2835_aux + BCM2835_AUX_MU_IO_REG / 4;
+ 
+ 	while ((bcm2835_peri_read(lsr) & 0x01) == 0)
+ 		;
+ 
+ 	return (bcm2835_peri_read_nb(io) & 0xFF);
+ }
+ 
  // Allocate page-aligned memory.
  void *malloc_aligned(size_t size)
  {
***************
*** 935,940 ****
--- 990,996 ----
  	bcm2835_bsc0 = (uint32_t*)BCM2835_BSC0_BASE;
  	bcm2835_bsc1 = (uint32_t*)BCM2835_BSC1_BASE;
  	bcm2835_st   = (uint32_t*)BCM2835_ST_BASE;
+ 	bcm2835_aux   = (uint32_t*)BCM2835_AUX_BASE;
  	return 1; // Success
      }
      int memfd = -1;
***************
*** 976,981 ****
--- 1032,1041 ----
      bcm2835_st = mapmem("st", BCM2835_BLOCK_SIZE, memfd, BCM2835_ST_BASE);
      if (bcm2835_st == MAP_FAILED) goto exit;
  
+     // AUX
+     bcm2835_aux = mapmem("aux", BCM2835_BLOCK_SIZE, memfd, BCM2835_AUX_BASE);
+     if (bcm2835_aux == MAP_FAILED) goto exit;
+ 
      ok = 1;
  
  exit:
***************
*** 999,1004 ****
--- 1059,1065 ----
      unmapmem((void**) &bcm2835_bsc0, BCM2835_BLOCK_SIZE);
      unmapmem((void**) &bcm2835_bsc1, BCM2835_BLOCK_SIZE);
      unmapmem((void**) &bcm2835_st,   BCM2835_BLOCK_SIZE);
+     unmapmem((void**) &bcm2835_aux,   BCM2835_BLOCK_SIZE);
      return 1; // Success
  }    
  
***************
*** 1078,1081 ****
  
      return 0;
  }
! #endif
\ No newline at end of file
--- 1139,1142 ----
  
      return 0;
  }
! #endif
