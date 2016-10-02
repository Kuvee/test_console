#include "page.h"

    //default constructor 
    Page::Page() :
        make_active_flag(false),
        name(NULL),
        term(NULL),
        num_menuitems(0),
        data_start_x(0),
        page_num(0)
        {}
        
Page::Page(const char * name_p, Terminal * term_p):
    make_active_flag(false),
    name(name_p),
    term(term_p),
    num_menuitems(0),
    data_start_x(0),
    max_upper_letter_cmd('A'-1),
    max_lower_letter_cmd('a'-1),
    max_number_cmd('1'-1),
    page_num(0)
{
   // term->printf("Page::Page('%s')\r\n",name);
    }
        
Page& Page::add(MenuItem const &item_p) {
    if(num_menuitems < MAX_MENUITEMS-1) {
            item[num_menuitems] = item_p;  //make a local copy of the item

            MenuItem * active_item = &item[num_menuitems];
            int item_data_start = active_item->name_len+ (LEVEL_INDENT * (active_item->level+1)) + DATA_GAP;
            
            if (item_data_start > data_start_x) data_start_x = item_data_start;   //keep track of length so we can put data after it
            
            if(active_item->type == heading) command_letter[num_menuitems]=0; //Headings don't get numbers
            
            else {  
            
            //Assign the command character for the menu item
            if(active_item->level==1)  //it gets an lower case letter
             {
             command_letter[num_menuitems] = ++max_lower_letter_cmd;
             }
            else if(active_item->level==0)  //it gets a number
             {
             command_letter[num_menuitems] = ++max_number_cmd;
             }
            else
             {
             command_letter[num_menuitems] = ++max_lower_letter_cmd;
             }
            
            }
            //term->printf("Added menu item '%s' as index %d cmd %c to page %s\r\n", active_item->name, num_menuitems, command_letter[num_menuitems], name);
            num_menuitems++;;
        }
    else term->printf("Failed to add menu item #%d, '%s'\r\n", num_menuitems, item_p.name);
    return *this;
}
    
void Page::display(void){
    term->cls();
    term->HideCursor();
    term->locate(0,0);
    term->printf("%s",name);
        int index;
    
    for (index=0; index < num_menuitems; index++){
           term->locate(LEVEL_INDENT*(item[index].level+1),index+ITEM_ROW_START);
            
                //don't print command letter for messages without them
            if(command_letter[index] != 0) term->printf("%c) ", command_letter[index]);
            
      term->printf("%s", item[index].name);
        }
        
                index++;  //add a space before the return line
        term->locate(LEVEL_INDENT,index+ITEM_ROW_START);
                term->printf("x) Back to previous page");
        update();
    }
// Update the data on the Page
void Page::update(void){
    char buf[32];
    for (int index=0; index < num_menuitems; index++){
        if(!item[index].action) continue;
        
        term->locate(data_start_x,index+ITEM_ROW_START);
        item[index].action->getString(buf, sizeof(buf));
        term->printf("%s",  buf);
    }
    
    term->HideCursor();    
}

