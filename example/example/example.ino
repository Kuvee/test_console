#include "testconsole.h"

TestConsole console("Post Fill");

void setup() {
  // put your setup code here, to run once:
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


}

void loop() {
  // put your main code here, to run repeatedly:
  console.tick();
  delay(100);
}
