#include "menuitem.h"
#include "page.h"


MenuItem::MenuItem() :
    name(NULL),
    level(0),
    type(display),
    action(NULL),
    name_len(0),
    data_col(0),
    target_page(-1)
{}


MenuItem::MenuItem(const char * name_p, MenuAction *action_p,  uint8_t level_p, MenuType type_p, uint8_t target_page_p):
    name(name_p),
    level(level_p),
    type(type_p),
    action(action_p),
    name_len(strlen(name_p)),
    data_col(0),
    target_page(target_page_p)
{}

MenuItem::MenuItem(Page &target_page_p):
    name(target_page_p.name),
    level(0),
    type(menu),
    action(NULL),
    name_len(strlen(name)),
    data_col(0),
    target_page(target_page_p.page_num)
{}
