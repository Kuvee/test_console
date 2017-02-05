#include "testconsole.h"

TestConsole::TestConsole(const char * name_p):
    next_status_line(0),
    sb_needs_update(true),
    term(),
    name(name_p),
    num_pages(0),
    current_page(0)
{
 //     term.baud(baud_rate);
 //     term.printf("TestConsole::TestConsole('%')\r\n",name);
    term.HideCursor();
    for(int i = 0; i < MAX_PAGES; i++) {
        page[i] = NULL;   //init all pages to NULL
        previous_page[i] = 0;  //if never changed, return from each page will go home
    }
    active_page = page[current_page];

    for(int i = 0; i < NUM_STATUS_LINES; i++) {
         //fill the buffer will null terminated spaces
        memset(sb_buffer[i], ' ',SZ_SB_BUF);
        sb_buffer[i][SZ_SB_BUF-1] = '\0';  //terminate string
    }
}

//Page& TestConsole::add_page(Page const &page_p){
Page* TestConsole::add_page(const char * name_p){
    if(num_pages >=  MAX_PAGES)
    {
        term.printf("MAX_PAGES Exceeded adding'%s'\r\n", name_p);
        return NULL;  //return valid reference to home page on

    }

        page[num_pages] = new Page(name_p, &term);
        Page* this_page = page[num_pages];
        this_page->page_num = num_pages;   //let the page know what number it is to help with lookups

                if(num_pages == 0) {   //if this is the first page, set it active
                    this_page->set_active();
                }
        //term.printf("Added page '%s'\r\n", this_page->name);
        num_pages++;
        return this_page;

    }

uint8_t TestConsole::page_change(uint8_t new_page){
            if(NULL == page[current_page]) {
                term.printf("invalid page passed to page_chage\r\n");
                return current_page;
                }

            //save a copy of the page so we can go back, but only up the tree
            if((new_page != current_page) &&
               (previous_page[current_page] != new_page)) {
                   previous_page[new_page] = current_page;
               }
            current_page = new_page;
            active_page = page[current_page];

            active_page->display();

            active_page->ack_active();

            term.locate((TERMINAL_WIDTH - strlen(name))/2, 0);
            term.printf("%s", name);

            return current_page;
    }

//here, using knowledge of the page, we process commands
uint8_t TestConsole::process_cmd(char cmd){
    if('x' == cmd) {
        page_change(previous_page[current_page]);
        return 0;
    }

    if('q' == cmd) {
        page_change(0);
        return 0;
    }

    if('\r' == cmd) {   //refresh the page on enter
        active_page->set_active();
        }

    for(int index=0; index < active_page->num_menuitems; index++) {
        MenuItem const & item = active_page->get_menu_item(index);
        if(active_page->get_command_letter(index) != cmd) continue;

        if(item.type == menu) {
            // for menuitems that goto other menus, just change the page
            page_change(item.target_page);
        } else if(item.action) {
            // otherwise call the callaback
            item.action->doAction();
        }
        return 0;
    }

    return 1;
}

uint8_t TestConsole::tick(void){
    static unsigned long last_display = millis();
    if (Serial.available()){   //if there is a character
          if(process_cmd(Serial.read())){
                term.locate(TERM_LOC_FEEDBACK);
                term.printf("invalid command");
            }
        }

        //go through the list of pages, and see if any are set to become active
        for(uint8_t index=0; index < num_pages; index++){
            if(page[index]->check_active()){
                 page_change(index);
                 sb_needs_update = true;
                }
        }

        //Every three seconds, update the display in case there is a new connection
        if(millis() - last_display > 3000) {
            active_page->display();
            last_display = millis();
        }

        if(active_page != NULL)
        {
            active_page->update();
        } else
        {
                term.printf("invalid page %p", active_page);
        }

        if(sb_needs_update) update_status_bar();
    return 0;
    }

void TestConsole::update_status_bar(){
    for(uint8_t i = 0; i < NUM_STATUS_LINES; i++){
    term.locate(0,TERMINAL_HEIGHT-NUM_STATUS_LINES+i);
    Serial.print(sb_buffer[(next_status_line+1+i)%NUM_STATUS_LINES]);
    term.clear_to_eol();
    }
}
void TestConsole::status_bar(const char* format, ...)
{
    char buffer[SZ_SB_BUF];

    va_list argptr;
    va_start(argptr, format);
    vsnprintf(buffer, sizeof(buffer), format, argptr);
    va_end(argptr);

    status_bar_addnext(buffer);
}

void TestConsole::status_bar(const __FlashStringHelper* format)
{
    String str(format);
    status_bar_addnext(str.c_str());
}

void TestConsole::status_bar(String const & str)
{
    status_bar_addnext(str);
}

void TestConsole::status_bar_addnext(char const * tmpbuf)
{
    //only buffer and increment if the message is different from the last
    if(0 != strncmp(tmpbuf, sb_buffer[next_status_line], sizeof(sb_buffer[0]))) {
        increment_status_line();
        strncpy(sb_buffer[next_status_line], tmpbuf, sizeof(sb_buffer[0]));
        sb_needs_update = true;
    }
}
