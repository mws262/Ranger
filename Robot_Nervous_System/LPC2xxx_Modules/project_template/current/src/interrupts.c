/*

	interrupts.c
	Sets up the vectored interrupts and the fast interrupt handler
	
	Nicolas Williamson - Dec 2009
	
*/

#include <includes.h>

ISR isrs[32];
unsigned int vic_nums[32];
unsigned int vect_count = 0;

void init_interrupts(void){
	// *************************************************
	// Interrupt enables
	// *************************************************
	VICIntEnable = 0;
	VICVectAddr  = 0;
	VICSoftIntClr = 0xffffffff;  	 //clear all interupts - only do this once
	

	
	// ************ PRIORITY 1 ******************  
	//ADCX
	VICVectAddr1 = (unsigned long)adcx_isr; // Pick a vectored slot for the IRQ (this one uses 1)
	VICVectCntl1 = (1<<5) | VIC_EINT1;         // (1<<5) is the interrupt enable bit, 15 is EINT1 interrupt
	VICIntEnable |= (1<<VIC_EINT1); //Enable EINT1 (bit 15)
	
/*	// ************ PRIORITY 2 ******************  
	//TIMER 1 FOR SCHEDULE
	VICVectAddr2 = (unsigned long)timer1_isr;
	VICVectCntl2 = (1<<5) | VIC_TIMER1; // (1<<5) is the interrupt enable bit, 5 is Timer1 interrupt
	VICIntEnable |= (1<<VIC_TIMER1); //Enable Timer1 (bit5)*/
	
	// ************ PRIORITY 3 ******************  
	//ADCI
	VICVectAddr3 = (unsigned long)adci_isr;
	VICVectCntl3 = (1<<5) | VIC_AD; //(1<<5) is interrupt enable bit; 18 is ADC
	VICIntEnable |= (1<<VIC_AD); //Enable ADC interrupt (bit18)
	
	// ************ PRIORITY 4 ******************
	//CAN1 TX
	VICVectAddr4 = (unsigned long)can_tx1_isr;
	VICVectCntl4 = 0x20 | VIC_CAN1TX;
	VICIntEnable = 1 << VIC_CAN1TX; 
	
	// ************ PRIORITY 5 ******************
	//CAN1 RX
	VICVectAddr5 = (unsigned long)can_rx1_isr;
	VICVectCntl5 = 0x20 | VIC_CAN1RX;
	VICIntEnable = 1 << VIC_CAN1RX;
	
	// ************ PRIORITY 6 ******************
	//CAN2 RX
	VICVectAddr6 = (unsigned long)can_rx2_isr;
	VICVectCntl6 = 0x20 | VIC_CAN2RX;
	VICIntEnable = 1 << VIC_CAN2RX;
	
	// ************ PRIORITY 7 ******************
	//CAN2 TX
	VICVectAddr7 = (unsigned long)can_tx2_isr;
	VICVectCntl7 = 0x20 | VIC_CAN2TX;
	VICIntEnable = 1 << VIC_CAN2TX;
	
	// ************ PRIORITY 8 ******************
	//CAN ERRORS
	VICVectAddr8 = (unsigned long)can_error_isr;
	VICVectCntl8 = (1<<5) | VIC_CAN_AF;
	VICIntEnable = 1<<VIC_CAN_AF;	
	
	// ************ PRIORITY 9 ******************
	//UART (uart_int)
	VICVectAddr9 = (unsigned long)uarti_isr;
	VICVectCntl9 = 0x20 | VIC_UART0; 
	VICIntEnable = 1 << VIC_UART0;   
  
  // ************ PRIORITY 10 ******************
	//Absolute Encoder
	VICVectAddr10 = (unsigned long)ae_isr;
	VICVectCntl10 = 0x20 | VIC_SPI0; 
	VICIntEnable = 1 << VIC_SPI0;   
  
  	// ************ FIQ/PRIORITY 0 ******************
	//TIMER 0 FOR QEC
	add_fiq(VIC_TIMER0, qec_isr);
}

void add_fiq(int vic_num, ISR isr){
  vic_nums[vect_count++] = vic_num;
  isrs[vic_num] = isr;
  VICIntSelect |= (1<<vic_num);
  VICIntEnable |= (1<<vic_num);
}

void FIQ_Handler(void) __irq {
  int status = VICFIQStatus;
  int i, vic_num;
  ISR isr;
  for (i = 0; i < vect_count; i++){
    vic_num = vic_nums[i];
    if (status & (1<<vic_num)){
      isr = (isrs[vic_num]);
      isr();
    }
  }
}