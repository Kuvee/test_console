#include "Terminal.h"
#include "testconsole.h"
#include "page.h"
#include "menuitem.h"

TestConsole console("Post Fill");

void setup() {
  Serial.begin(9600);
  #if 1
  // put your setup code here, to run once:
    Page *pageHome = console.add_page("Home");
    Page *page1 = console.add_page("Page1");
    Page *page2 = console.add_page("Page2");
   
 //       pageHome.add(pagePower.Name, &pagePower.set_active(),  0, menu)); 
       (*pageHome)
            .add(page1)
            .add(page2);


//These macros are actually in the menuitem.h file but commented here below for reference
//#define MAKE_PORT(x, io) (new Menu##io(#x, io(x)))  //use this macro to create a port (it saves name as well as creatign the IO
//#define REUSE_PORT(x, io, port) (new Menu##io(#x, port))  //use this macro if you created the port externally


                (*page1)
                    .add("DISPLAY")
                    .add("Eject Button Status", new MenuNotImplementedAction("N/A"))
                    .add("Push button status", new MenuNotImplementedAction("N/A"))
                    .add("Battery voltage", new MenuNotImplementedAction("N/A"))
                    .add("Dock sense", new MenuNotImplementedAction("N/A"))
                    .add("CONTROL")
                    .add("MCU regulator disable", new MenuNotImplementedAction("N/A"))
                    .add("Battery Read Enable", new MenuNotImplementedAction("N/A"))
                    .add("Dock Read Enable", new MenuNotImplementedAction("N/A"));


                (*page2)
                    .add("DISPLAY")
                    .add("Battery voltage", new MenuNotImplementedAction("N/A"))
                    .add("Accumulated Charge", new MenuNotImplementedAction("N/A"))
                    .add("CHRG line", new MenuNotImplementedAction("N/A"))
                    .add("High/Low Charge thresholds", new MenuNotImplementedAction("N/A"))
                    .add("Status Register", new MenuNotImplementedAction("N/A"))
                    .add("Control Register", new MenuNotImplementedAction("N/A"));

#endif
}

void loop() {
  // put your main code here, to run repeatedly:
 console.tick();
// console.term.locate(0,0);
  delay(100);
}
