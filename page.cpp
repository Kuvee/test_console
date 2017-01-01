#include "page.h"

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
    memset(&items, 0, sizeof(items));
   // term->printf("Page::Page('%s')\r\n",name);
}

MenuItem* Page::add_menu_item(MenuItem const &item_p) {
    if(num_menuitems+1 >= MAX_MENUITEMS) {
        term->printf("Failed to add menu item #%d, '%s'\r\n", num_menuitems, item_p.name);
        return NULL;
    }

    MenuItem *cur = items[num_menuitems] = new MenuItem(item_p);  // make a copy of the passed in item
    uint8_t item_data_start = cur->name_len+ (LEVEL_INDENT * (cur->level+1)) + DATA_GAP;

    if (item_data_start > data_start_x) data_start_x = item_data_start;   //keep track of length so we can put data after it

    if(cur->type == heading) {
        cur->command_letter=0; //Headings don't get numbers
    } else {
        //Assign the command character for the menu item
        if(cur->level==1) {  //it gets an lower case letter
            cur->command_letter = ++max_lower_letter_cmd;
        } else if(cur->level==0) {  //it gets a number
            cur->command_letter = ++max_number_cmd;
        } else {
            cur->command_letter = ++max_lower_letter_cmd;
        }
    }
    //term->printf("Added menu item '%s' as index %d cmd %c to page %s\r\n", cur->name, num_menuitems, cur->command_letter, name);
    ++num_menuitems;
    return cur;
}

void Page::display(void){
    term->cls();
    term->HideCursor();
    term->locate(0,0);
    term->printf("%s",name);
        uint8_t index;

    for (index=0; index < num_menuitems; index++){
           term->locate(LEVEL_INDENT*(get_menu_item(index).level+1),index+ITEM_ROW_START);

                //don't print command letter for messages without them
            if(get_command_letter(index) != 0) term->printf("%c) ", get_command_letter(index));

      term->printf("%s", get_menu_item(index).name);
        }

                index++;  //add a space before the return line
        term->locate(LEVEL_INDENT,index+ITEM_ROW_START);
                term->printf("x) Back");
        update();
    }
// Update the data on the Page
void Page::update(void){
    char buf[32];
    for (uint8_t index=0; index < num_menuitems; index++){
        if(!get_menu_item(index).action) continue;

        term->locate(data_start_x,index+ITEM_ROW_START);
        get_menu_item(index).action->getString(buf, sizeof(buf));
        term->printf("%s",  buf);
    }

    term->HideCursor();
}

