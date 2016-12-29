#ifndef __TESTCONSOLE_H
#define __TESTCONSOLE_H
#include "Terminal.h"
#include "page.h"
#include "menuitem.h"
#include <string.h>


#define MAX_PAGES 9  
#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 24
#define NUM_STATUS_LINES 6
#define SZ_SB_BUF 80

class TestConsole {
    private:
    Page* active_page;
    char sb_buffer[NUM_STATUS_LINES][SZ_SB_BUF];
    int next_status_line;
    bool sb_needs_update;
    void status_bar_addnext(char *);
    void status_bar_addnext(String str);
    inline void increment_status_line(){
        next_status_line++;
        next_status_line = next_status_line%NUM_STATUS_LINES;
    };

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
    void status_bar(const __FlashStringHelper* format);
    void status_bar(String str);

    void update_status_bar();
    Page* get_active_page() {return active_page;};
    
    };


#endif  //fence