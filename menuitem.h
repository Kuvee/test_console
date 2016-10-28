#ifndef __MENUITEM_H
#define __MENUITEM_H

#include "Terminal.h"
#include "Arduino.h"

#ifdef __cplusplus
 extern "C" {
#endif

extern Terminal term;

typedef enum {menu, heading, display, control} MenuType;
typedef char * (*callback_function)(bool); // type for conciseness

#define MAX_NAME_LEN (80-10-10)
#define MAKE_DIGINPORT(x) (new MenuDigitalIn(#x, x))  //use this macro to create a port (it saves name as well as creating the IO
#define MAKE_DIGOUTPORT(x) (new MenuDigitalOut(#x, x))  //use this macro to create a port (it saves name as well as creating the IO

class MenuAction;
class Page;

class MenuItem {
public:
    MenuItem();
    MenuItem(const char * name_p, MenuAction *action_p, int level, MenuType type_p, int target_page = -1);
    MenuItem(Page &target_page_p);  //construct a menu selection item this way
    const char *name;   //reference to the name 
    int level;      //0 if primary 1 or greater if this is a sub-menu
    MenuType type;  //are we displaying something or controlling something
    
    MenuAction *action; //callback for getting/setting the data
    
    int name_len;
    
    int data_col;   //column where the data is shown
    int target_page;  //the page to go to if called
};

class MenuAction {
public:
    MenuAction(char const *name):
        m_name(name)
    {}
    
    char const *getName() {
        return m_name;
    }
    
    virtual void getString(char *buf, int bufLen) {
        if(buf && bufLen > 0) {
            buf[0] = '\0';
        }
    }
    virtual void doAction() {}
    
private:
    char const *m_name;
};

//class to toggle a trigger variable, which would set off other events (such as run an function)
class MenuTriggerVariable: public MenuAction {
public:
    MenuTriggerVariable(char const *name, bool triggerinit=false):
        MenuAction(name)
    {trigger=triggerinit;}
    
    virtual void getString(char *buf, int bufLen) {
        snprintf(buf, bufLen, "%d", trigger);
    }

    virtual void doAction() {trigger = true; };
    bool set() {doAction();};
    bool get() {return trigger;};
    void clear() {trigger = false;};
    bool test_and_clear() {bool ret = trigger; trigger=false; return ret;}

protected:
    bool trigger;
};

//class to toggle variable, which would set off other events (such as run an function)
class MenuToggleVariable: public MenuTriggerVariable {
public:
       MenuToggleVariable(char const *name, bool triggerinit=false): MenuTriggerVariable(name, triggerinit) {};
       void set() {trigger = true;};

       virtual void doAction() {trigger = !trigger; };
};

class MenuDigitalIn: public MenuAction {
public:
    MenuDigitalIn(char const *name, int myIO):
        MenuAction(name)
    {m_io=myIO;}
    
    virtual void getString(char *buf, int bufLen) {
        snprintf(buf, bufLen, "%d", digitalRead(m_io));
    }
private:
    int m_io;
};

class MenuDigitalOut: public MenuAction {
public:
    MenuDigitalOut(char const *name, int myIO):
        MenuAction(name)
    {value=0; m_io=myIO;}
    
    virtual void getString(char *buf, int bufLen) {
        value = 0!=(*portOutputRegister(digitalPinToPort(m_io))&digitalPinToBitMask(m_io));
        snprintf(buf, bufLen, "%d", value);
    }
    virtual void doAction() {
        value = 0!=(*portOutputRegister(digitalPinToPort(m_io))&digitalPinToBitMask(m_io));
        digitalWrite(m_io, !value);
    }
private:
    bool value;
    int m_io;
};

#if 0 
class MenuAnalogIn: public MenuAction {
public:
    MenuAnalogIn(char const *name, AnalogIn const & myIO):
        MenuAction(name),
        m_io(myIO)
    {}
    
    virtual void getString(char *buf, int bufLen) {
        snprintf(buf, bufLen, "%f", m_io.read());
    }

private:
    AnalogIn m_io;
};
#endif

class MenuNotImplementedAction:public MenuAction {
public:
    MenuNotImplementedAction(char const *name):  MenuAction(name)
    {}
    
    virtual void getString(char *buf, int bufLen) {
        snprintf(buf, bufLen, "N/A");
    }
};

class MenuTestAction: public MenuAction {
public:
    MenuTestAction(char const *name):
        MenuAction(name),
        m_value(false)
    {}
    
    virtual void getString(char *buf, int bufLen) {
        snprintf(buf, bufLen, "%d", int(m_value));
    }
    virtual void doAction() {
        m_value = !m_value;
    }
    
private:
    bool m_value;
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif