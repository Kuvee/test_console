#ifdef TEST   //define test to make this an active main
#include "Arduino.h"
#include "Terminal.h"
#include "testconsole.h"
#include "page.h"
#include "menuitem.h"



Terminal term(DEBUG_TX, DEBUG_RX);
Ticker heartbeat;

    
volatile bool tick = false;  // this is the primary flag that indicates that it is time to run the control loop
static int ERROR_Overrun = 0;  //this counts the number of times that the code was still running when the interrupt tried to trigger it again (ie code is running too slow)

/***********************************************************************************************
 * \brief    Interrupt for the main loop timer
 *
 **********************************************************************************************/   
void tick_int() {
    if (!tick) tick = true;  //detect if interrupt has not yet finished
    else ERROR_Overrun++ ;    
}


int main(void){

    term.baud(115200);

//create the Pages for the console
//    term.printf("\nsizeof page %d, menuitem %d\n",sizeof(Page), sizeof(MenuItem));
    TestConsole console("KV2 Test Program");

    Page &pageHome = console.add_page(Page("Home"));
    Page &page1 = console.add_page(Page("Page1"));
    Page &page2 = console.add_page(Page("Page2"));
   
 //       pageHome.add(pagePower.Name, &pagePower.set_active(),  0, menu)); 
        pageHome
            .add(page1)
            .add(page2)

//These macros are actually in the menuitem.h file but commented here below for reference
//#define MAKE_PORT(x, io) (new Menu##io(#x, io(x)))  //use this macro to create a port (it saves name as well as creatign the IO
//#define REUSE_PORT(x, io, port) (new Menu##io(#x, port))  //use this macro if you created the port externally


                page1
                    .add("DISPLAY")
                    .add("Eject Button Status", MAKE_PORT(PA_1, DigitalIn))
                    .add("Push button status", MAKE_PORT(PUSH_BUT_IN, DigitalIn))
                    .add("Battery voltage", MAKE_PORT(A1, AnalogIn))
                    .add("Dock sense", MAKE_PORT(PA_2, DigitalIn))
                    .add("CONTROL")
                    .add("MCU regulator disable", MAKE_PORT(PA_3, DigitalOut))
                    .add("Battery Read Enable", MAKE_PORT(PA_4, DigitalOut))
                    .add("Dock Read Enable", MAKE_PORT(PA_5, DigitalOut));


                page2
                    .add("DISPLAY")
                    .add("Battery voltage", MAKE_PORT(A0, AnalogIn))
                    .add("Accumulated Charge", new MenuNotImplementedAction("N/A"))
                    .add("CHRG line", MAKE_PORT(PA_0, DigitalIn))
                    .add("High/Low Charge thresholds", new MenuNotImplementedAction("N/A"))
                    .add("Status Register", new MenuNotImplementedAction("N/A"))
                    .add("Control Register", new MenuNotImplementedAction("N/A"));


    heartbeat.attach(&tick_int, 0.1); // the address of the function to be attached (tick_int) and the interval 10000uS
    
    
    wait(2);
    
            
    while(1) {
        while(tick){    //put real-time code in here
                    
               console.tick();
                
                tick=false;   //keep this as the last item in the while loop
                
                }
                
            }               
    
}

#endif