#ifndef __TESTCONSOLE_H
#define __TESTCONSOLE_H
#include "Terminal.h"
#include "page.h"
#include "menuitem.h"

#define MAX_PAGES 9  
#define TERMINAL_WIDTH 80
class TestConsole {
    private:
    Page* active_page;
    public:
    TestConsole(const char * Name_p);
    Terminal term;
    const char * name;   //reference to the name of the Program
    int num_pages;
    int current_page, previous_page;
    Page * page[MAX_PAGES];
    Page * add_page(const char * name_p);
    int process_cmd(char cmd);
    int tick();
    int page_change(int page);
	void status_bar(const char* format, ...);
    
    };


#endif  //fence