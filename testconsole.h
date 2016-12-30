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
#define SZ_SB_BUF 60

class TestConsole {
    private:
    Page* active_page;
    char sb_buffer[NUM_STATUS_LINES][SZ_SB_BUF];
    uint8_t next_status_line;
    bool sb_needs_update;
    void status_bar_addnext(char const *);
    void status_bar_addnext(String const & str) {
        status_bar_addnext(str.c_str());
    }
    void increment_status_line(){
        next_status_line = (next_status_line + 1) % NUM_STATUS_LINES;
    };

    public:
    TestConsole(const char * Name_p);
    Terminal term;
    const char * name;   //reference to the name of the Program
    uint8_t num_pages;
    uint8_t current_page, previous_page;
    Page * page[MAX_PAGES];
    Page * add_page(const char * name_p);
    uint8_t process_cmd(char cmd);
    uint8_t tick();
    uint8_t page_change(int page);

    void status_bar(const char* format, ...);
    void status_bar(const __FlashStringHelper* format);
    void status_bar(String const & str);

    void update_status_bar();
    Page* get_active_page() {return active_page;};
    
    };


#endif  //fence