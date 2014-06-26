// -*-c-*-
#undef HID_ENABLED

#include <stdarg.h>
void p(char *fmt, ... );

// Wittich 3/2014
// Adapted from :
// http://forum.arduino.cc/index.php?topic=137635.5;wap2
// Arduino Due ADC->DMA->USB 1MSPS
// by stimmer
// Input: Analog in A0
// Output: Raw stream of uint16_t in range 0-4095 on Native USB Serial/ACM

// see section 28 of the SAM3X/A manual for the ADC_RPR 
// (receive pointer register)
// documentation - this is the Peripheral DMA controller 

// on linux, to stop the OS cooking your data: 
// stty -F /dev/ttyACM0 raw -iexten -echo -echoe -echok -echoctl -echoke -onlcr

volatile int bufn,obufn;
const int NBUFFER=2048;
const int NSLOT = 8; // must be power of 2
uint16_t buf[NSLOT][NBUFFER];   // 4 buffers of NBUFFER readings

#define NATIVEPORT


// This is a handler that is called for the ADC on certain interrupts;
// the name is defined in 
// ./sam/system/CMSIS/Device/ATMEL/sam3xa/source/gcc_atmel/startup_sam3xa.c
// this gets called on an interrupt defined in the ADC_IER register 
void ADC_Handler()      // move DMA pointers to next buffer
{
  // We set up the interrupt handler to move the DMA pointers
  // to the next location.
  int f=ADC->ADC_ISR;
  if (f&ADC_ISR_ENDRX){
    bufn=(bufn+1)&(NSLOT-1); // cycle through the four buffers
    ADC->ADC_RNPR=(uint32_t)buf[bufn];
    ADC->ADC_RNCR=NBUFFER;
  } 
}


// Gets called once on Arduino Due reboot.
// - Sets of the USB (either Serial or regular)
// - turns on the ADC peripheral
// - sets up the ADC 
// - turns on the DMA engine to automatically move the result of
//   digitization to main memory w/o program intervention.
// - turns on the interrupt handler; this handler updates the DMA
//   engine to the next bank of addresses once the DMA engine runs
//   out of space in the current bank
// - prints out values of some ADC registers
void setup(){
#ifdef NATIVEPORT
  SerialUSB.begin(0);
  while(!SerialUSB);
#else
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Starting ");
  Serial.print(__FILE__);
  Serial.print(" test, compiled on ");
  Serial.println( __DATE__);
#endif
  pmc_enable_periph_clk(ID_ADC);
  // change frequency in 3rd argument
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);
  // use adc_get_actual_adc_clock() for actual frequency
  // uint32_t adc_get_actual_adc_clock(const Adc *p_adc, const uint32_t ul_mck)
  Serial.print("ADC Clock is (in Hz) ");
  Serial.println(adc_get_actual_adc_clock(ADC, SystemCoreClock));
  Serial.print("SystemCoreClock is (in Hz) ");
  Serial.println(SystemCoreClock);

  ADC->ADC_MR |=ADC_MR_FREERUN_ON; // free running

  ADC->ADC_CHER=ADC_CHER_CH7;  // enable channel 7

  NVIC_EnableIRQ(ADC_IRQn);
  ADC->ADC_IDR=~(ADC_IDR_ENDRX);
  ADC->ADC_IER=ADC_IER_ENDRX; // ENDRX
  ADC->ADC_RPR=(uint32_t)buf[0];   // DMA buffer
  ADC->ADC_RCR=NBUFFER;
  ADC->ADC_RNPR=(uint32_t)buf[1]; // next DMA buffer
  ADC->ADC_RNCR=NBUFFER;
  bufn=obufn=1;
  ADC->ADC_PTCR=1;
  ADC->ADC_CR=2;

  // print out value of the MR and CR registers
  p("ADC MR = 0x%08lx\n", ADC->ADC_MR);
  p("ADC CR = 0x%08lx\n", ADC->ADC_CR);

}

// this function "loop" gets called repeatedly when the arduino is
// running. 
// It shovels the data from the main memory (where DMA put it) 
// out onto the USB for offline analysis.
void loop(){
  while(obufn==bufn); // wait for buffer to be full
#ifdef NATIVEPORT
  // send it - 512 bytes = 256 uint16_t
  SerialUSB.write((uint8_t *)buf[obufn],2*NBUFFER); 
#else
  p("buffer = %i\n", obufn);
  for ( int i = 0; i < NBUFFER; ++i ) {
    Serial.print(buf[obufn][i]);
    Serial.print(" " );
  }
  Serial.println(" " );
#endif

  obufn=(obufn+1)&3;    
}


// utility function like printf - not supplied in Arduino libraries
void p(char *fmt, ... ){
        char tmp[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(tmp, 128, fmt, args);
        va_end (args);
        Serial.print(tmp);
}
