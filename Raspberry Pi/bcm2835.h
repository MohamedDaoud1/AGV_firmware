*** /opt/bcm2835-1.25/src/bcm2835.h	2013-03-22 00:49:51.000000000 +0100
--- /opt/data/development/workspace/bcm2835/include/bcm2835.h	2013-04-19 16:40:36.000000000 +0200
***************
*** 127,132 ****
--- 127,142 ----
  /// - P1-03 (SDA)
  /// - P1-05 (SLC)
  ///
+ /// \par UART Pins
+ ///
+ /// The bcm2835_uart_* functions allow you to control the BCM 2835 mini UART interface,
+ /// allowing you to send and received data like a 16550 UART.
+ ///
+ /// The Raspberry Pi V2 GPIO pins used for the mini uart are:
+ ///
+ /// - P1-08 (TXD)
+ /// - P1-10 (RXD)
+ ///
  /// \par Real Time performance constraints
  ///
  /// The bcm2835 is a library for user programs (i.e. they run in 'userland'). 
***************
*** 161,166 ****
--- 171,177 ----
  ///
  /// Some of this code has been inspired by Dom and Gert.
  /// The I2C code has been inspired by Alan Barr.
+ /// The UART code has been inspired by David Welch.
  /// 
  /// \par Revision History
  ///
***************
*** 227,232 ****
--- 238,244 ----
  /// \version 1.24 Mark Dootson p[atched a problem with his previously submitted code
  ///               under high load from other processes. 
  /// \version 1.25 Updated author and distribution location details to airspayce.com
+ /// \version 1.26 Added bcm2835_uart_* functions. Contributed by Arjan van Vught.
  /// \author  Mike McCauley (mikem@airspayce.com) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
  
  
***************
*** 252,274 ****
  
  // Physical addresses for various peripheral register sets
  /// Base Physical Address of the BCM 2835 peripheral registers
! #define BCM2835_PERI_BASE               0x20000000
  /// Base Physical Address of the System Timer registers
! #define BCM2835_ST_BASE			(BCM2835_PERI_BASE + 0x3000)
  /// Base Physical Address of the Pads registers
! #define BCM2835_GPIO_PADS               (BCM2835_PERI_BASE + 0x100000)
  /// Base Physical Address of the Clock/timer registers
! #define BCM2835_CLOCK_BASE              (BCM2835_PERI_BASE + 0x101000)
  /// Base Physical Address of the GPIO registers
! #define BCM2835_GPIO_BASE               (BCM2835_PERI_BASE + 0x200000)
  /// Base Physical Address of the SPI0 registers
! #define BCM2835_SPI0_BASE               (BCM2835_PERI_BASE + 0x204000)
  /// Base Physical Address of the BSC0 registers
! #define BCM2835_BSC0_BASE 		(BCM2835_PERI_BASE + 0x205000)
  /// Base Physical Address of the PWM registers
! #define BCM2835_GPIO_PWM                (BCM2835_PERI_BASE + 0x20C000)
!  /// Base Physical Address of the BSC1 registers
! #define BCM2835_BSC1_BASE		(BCM2835_PERI_BASE + 0x804000)
  
  
  /// Base of the ST (System Timer) registers.
--- 264,288 ----
  
  // Physical addresses for various peripheral register sets
  /// Base Physical Address of the BCM 2835 peripheral registers
! #define BCM2835_PERI_BASE              0x20000000
  /// Base Physical Address of the System Timer registers
! #define BCM2835_ST_BASE					(BCM2835_PERI_BASE + 0x3000)
  /// Base Physical Address of the Pads registers
! #define BCM2835_GPIO_PADS              (BCM2835_PERI_BASE + 0x100000)
  /// Base Physical Address of the Clock/timer registers
! #define BCM2835_CLOCK_BASE             (BCM2835_PERI_BASE + 0x101000)
  /// Base Physical Address of the GPIO registers
! #define BCM2835_GPIO_BASE              (BCM2835_PERI_BASE + 0x200000)
  /// Base Physical Address of the SPI0 registers
! #define BCM2835_SPI0_BASE              (BCM2835_PERI_BASE + 0x204000)
  /// Base Physical Address of the BSC0 registers
! #define BCM2835_BSC0_BASE 				(BCM2835_PERI_BASE + 0x205000)
  /// Base Physical Address of the PWM registers
! #define BCM2835_GPIO_PWM               (BCM2835_PERI_BASE + 0x20C000)
! /// Base Physical Address of the BSC1 registers
! #define BCM2835_BSC1_BASE				(BCM2835_PERI_BASE + 0x804000)
! /// Base Physical Address of the AUX registers
! #define BCM2835_AUX_BASE				(BCM2835_PERI_BASE + 0x215000)
  
  
  /// Base of the ST (System Timer) registers.
***************
*** 303,308 ****
--- 317,326 ----
  /// Available after bcm2835_init has been called
  extern volatile uint32_t *bcm2835_bsc1;
  
+ /// Base of the AUX registers.
+ /// Available after bcm2835_init has been called
+ extern volatile uint32_t *bcm2835_aux;
+ 
  /// Size of memory page on RPi
  #define BCM2835_PAGE_SIZE               (4*1024)
  /// Size of memory block on RPi
***************
*** 615,620 ****
--- 633,653 ----
  #define BCM2835_ST_CLO 							0x0004 ///< System Timer Counter Lower 32 bits
  #define BCM2835_ST_CHI 							0x0008 ///< System Timer Counter Upper 32 bits
  
+ // Defines for AUX
+ // GPIO register offsets from BCM2835_AUX_BASE.
+ #define BCM2835_AUX_ENABLES 					0x0004
+ #define BCM2835_AUX_MU_IO_REG 					0x0040
+ #define BCM2835_AUX_MU_IER_REG 					0x0044
+ #define BCM2835_AUX_MU_IIR_REG 					0x0048
+ #define BCM2835_AUX_MU_LCR_REG 					0x004C
+ #define BCM2835_AUX_MU_MCR_REG 					0x0050
+ #define BCM2835_AUX_MU_LSR_REG 					0x0054
+ #define BCM2835_AUX_MU_MSR_REG 					0x0058
+ #define BCM2835_AUX_MU_SCRATCH 					0x005C
+ #define BCM2835_AUX_MU_CNTL_REG 				0x0060
+ #define BCM2835_AUX_MU_STAT_REG 				0x0064
+ #define BCM2835_AUX_MU_BAUD_REG 				0x0068
+ 
  /// @}
  
  
***************
*** 1105,1110 ****
--- 1138,1169 ----
  
      /// @} 
  
+     /// \defgroup uart UART1 access
+     /// @{
+ 
+     /// Start UART1 operations.
+     /// Forces RPi UART pins P1-14 (UART1_TXD) and P1-15 (UART1_RXD)
+     /// to alternate function ALT5, which enables those pins for UART1 interface.
+     /// You should call bcm2835_uart1_end() when all UART functions are complete to return the pins to
+     /// their default functions (Alt 0)
+     /// \sa  bcm2835_uart1_end()
+     extern void bcm2835_uart_begin(void);
+ 
+     /// End UART operations.
+     /// UART1 pins P1-14 (UART1_TXD) and P1-15 (UART1_RXD)
+     /// are returned to their default UART0 behaviour.
+     extern void bcm2835_uart_end(void);
+ 
+     /// Transfers one uint32_t to the mini uart.
+     /// \param[in] uint32_t c.
+     extern void bcm2835_uart_send(uint32_t);
+ 
+     /// Receives one uint32_t from the mini uart.
+     /// \param[out] uint32_t
+     extern uint32_t bcm2835_uart_receive(void);
+ 
+     /// @}
+ 
  #ifdef __cplusplus
  }
  #endif
