#ifndef __PAGE_H
#define __PAGE_H
#include "menuitem.h"
#include "Terminal.h"
//this class contains Pages of menu items

extern Terminal term;
#define MAX_MENUITEMS 15
#define LEVEL_INDENT 8
#define DATA_GAP 4  
#define ITEM_ROW_START 3
#define TERM_LOC_DEBUG 40,24   //x,y for printing debug info
#define TERM_LOC_INPUT 10,23   // X,Y for getting input
#define TERM_LOC_FEEDBACK 10,24   // X,Y for getting input
//class MenuItem;   //forward declare

class Page{
    private:
    bool make_active_flag;
    public:
    Page();
    Page(const char * name_p, Terminal * term_p);
    const char * name;   //reference to the name of the Page
    Terminal * term;
    int num_menuitems;
    int data_start_x;    //starting column for printing the data
    MenuItem item[MAX_MENUITEMS];
    char command_letter[MAX_MENUITEMS];  //the character in front of the menuitem
    char max_upper_letter_cmd;
    char max_lower_letter_cmd;
    char max_number_cmd;
    int page_num;
    bool refresh_required;
    Page& add(Page *page_p) { return add(MenuItem((*page_p))); }
    Page& add(char const *text) { return add(MenuItem(text, NULL, 0, heading)); }
    Page& add(char const *text, MenuAction *action_p) { return add(MenuItem(text, action_p, 0, ::display, -1)); }
    Page& add(MenuItem const &item_p);
    void display();
        void update();
        char * set_active() {make_active_flag = true; return NULL;}  //flag the current page to be active
        void ack_active() {make_active_flag = false;}  //call to clear the set_active flag
        bool check_active() {return make_active_flag;} // check to see if this page wants to be active
};

#endif