#include "testconsole.h"

TestConsole::TestConsole(const char * name_p): 
    term(),
    name(name_p),
    num_pages(0),
    current_page(0)
{       
 //    term.baud(baud_rate);
 //       term.printf("TestConsole::TestConsole('%')\r\n",name);
        term.HideCursor();
       for(int i = 0; i < MAX_PAGES; i++) page[i] = NULL;   //init all pages to NULL
       active_page = page[current_page];

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
    
int TestConsole::page_change(int new_page){
            if(NULL == page[current_page]) {
                term.printf("invalid page passed to page_chage\r\n");
                return current_page;
                }
            previous_page = current_page;  //save a copy of the page so we can go back
            current_page = new_page;
            active_page = page[current_page];
            
            active_page->display();

            active_page->ack_active();

            term.locate((TERMINAL_WIDTH - strlen(name))/2, 0);
            term.printf("%s", name);

            return current_page;
    }
    
//here, using knowledge of the page, we process commands
int TestConsole::process_cmd(char cmd){
    if('x' == cmd) {
        page_change(previous_page);
        return 0;
    }   
    
    if('\r' == cmd) {   //refresh the page on enter
        active_page->set_active();
        }
        
    for(int index=0; index < active_page->num_menuitems; index++) {
        MenuItem *item = &active_page->item[index];
        if(active_page->command_letter[index] != cmd) continue;
            
        if(item->type == menu) {
            // for menuitems that goto other menus, just change the page
            page_change(item->target_page);
        } else if(item->action) {
            // otherwise call the callaback
            item->action->doAction();
        }
        return 0;
    }
    
    return 1;
}
    
int TestConsole::tick(void){
    if (Serial.available()){   //if there is a character
          if(process_cmd(Serial.read())){
                term.locate(TERM_LOC_FEEDBACK);
                term.printf("invalid command");    
            }
        }
        
        //go through the list of pages, and see if any are set to become active
        for(int index=0; index < num_pages; index++){
            if(page[index]->check_active()){
                 page_change(index);
                }
        }
        
        if(active_page != NULL)
        {
            active_page->update();
        } else 
        {
                term.printf("invalid page %p", active_page);
        }
    return 0;
    }    
	
void TestConsole::status_bar(const char* format, ...)
{
    char buffer[80];
	term.locate(0,24);
	va_list argptr;
    va_start(argptr, format);
    vsprintf(buffer, format, argptr);
    va_end(argptr);
	Serial.print(buffer);
}
