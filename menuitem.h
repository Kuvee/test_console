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
    MenuItem(const char * name_p, MenuAction *action_p, uint8_t level, MenuType type_p, uint8_t target_page = -1);
    MenuItem(Page &target_page_p);  //construct a menu selection item this way
    const char *name;   //reference to the name
    MenuType type;  //are we displaying something or controlling something

    MenuAction *action; //callback for getting/setting the data

    uint8_t level;      //0 if primary 1 or greater if this is a sub-menu
    uint8_t name_len;

    uint8_t data_col;   //column where the data is shown
    uint8_t target_page;  //the page to go to if called
    char command_letter; // the character in front of menu item; set by Page

};

class MenuAction {
public:
    MenuAction(char const *name):
        m_name(name)
    {}

    char const *getName() {
        return m_name;
    }

    virtual void getString(char *buf, uint8_t bufLen) {
        if(buf && bufLen > 0) {
            buf[0] = '\0';
        }
    }
    virtual void doAction() {}
    void fmtDouble(double val, byte precision, char *buf, unsigned bufLen = 0xffff);
    unsigned fmtUnsigned(unsigned long val, char *buf, unsigned bufLen = 0xffff, byte width = 0);

private:
    char const *m_name;
};

//class to toggle a trigger variable, which would set off other events (such as run an function)
class MenuTriggerVariable: public MenuAction {
public:
    MenuTriggerVariable(char const *name, bool triggerinit=false):
        MenuAction(name)
    {trigger=triggerinit;}

    virtual void getString(char *buf, uint8_t bufLen) {
        snprintf(buf, bufLen, "%d", trigger);
    }

    virtual void doAction() {trigger = true; };
    void set() {doAction();};
    virtual bool get() {return trigger;};
    void clear() {trigger = false;};
    void set_value(bool val) {trigger = val;};
    bool test_and_clear() {bool ret = trigger; trigger=false; return ret;}

protected:
    bool trigger;
};

//class to toggle variable, which would set off other events (such as run an function)
class MenuToggleVariable: public MenuTriggerVariable {
public:
       MenuToggleVariable(char const *name, bool triggerinit=false): MenuTriggerVariable(name, triggerinit) {override_val=-1;};
       void set() {trigger = true;};
       void override(int8_t val) {override_val = val;};
       bool get() {
           if(override_val < 0) return MenuTriggerVariable::get();
           else return override_val;
       }

       virtual void doAction() {trigger = !trigger; };
private:
        int8_t override_val;
};

class MenuDigitalIn: public MenuAction {
public:
    MenuDigitalIn(char const *name, uint8_t myIO):
        MenuAction(name)
    {m_io=myIO;}

    virtual void getString(char *buf, uint8_t bufLen) {
        snprintf(buf, bufLen, "%d", digitalRead(m_io));
    }
private:
    uint8_t m_io;
};

class MenuDigitalOut: public MenuAction {
public:
    MenuDigitalOut(char const *name, uint8_t myIO):
        MenuAction(name)
    {value=0; m_io=myIO;}

    virtual void getString(char *buf, uint8_t bufLen) {
        value = 0!=(*portOutputRegister(digitalPinToPort(m_io))&digitalPinToBitMask(m_io));
        snprintf(buf, bufLen, "%d", value);
    }
    virtual void doAction() {
        value = 0!=(*portOutputRegister(digitalPinToPort(m_io))&digitalPinToBitMask(m_io));
        digitalWrite(m_io, !value);
    }
private:
    bool value;
    uint8_t m_io;
};

#if 0
class MenuAnalogIn: public MenuAction {
public:
    MenuAnalogIn(char const *name, AnalogIn const & myIO):
        MenuAction(name),
        m_io(myIO)
    {}

    virtual void getString(char *buf, uint8_t bufLen) {
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

    virtual void getString(char *buf, uint8_t bufLen) {
        snprintf(buf, bufLen, "N/A");
    }
};

class MenuTestAction: public MenuAction {
public:
    MenuTestAction(char const *name):
        MenuAction(name),
        m_value(false)
    {}

    virtual void getString(char *buf, uint8_t bufLen) {
        snprintf(buf, bufLen, "%d", int(m_value));
    }
    virtual void doAction() {
        m_value = !m_value;
    }

private:
    bool m_value;
};

class MenuCounter: public MenuAction {
  public:
    MenuCounter(char const *name): MenuAction(name){count=0;};
    virtual void doAction() {count = 0; };
    virtual void getString(char *buf, uint8_t bufLen) {
        snprintf(buf, bufLen, "%ld ", count);
    }
    uint32_t increment(){return(++count);};
    uint32_t get(){return count;};
  protected:
    uint32_t count;

};

class MenuLongInt: public MenuAction {
public:
    MenuLongInt(char const *name, uint32_t val=0):
        MenuAction(name)
    {m_val=val;}

    virtual void getString(char *buf, uint8_t bufLen) {
        snprintf(buf, bufLen, "%ld", m_val);
    }

    void set(uint32_t val){m_val = val;};
private:
    uint32_t m_val;
};

class MenuInt: public MenuAction {
public:
    MenuInt(char const *name, int& val):
        MenuAction(name), m_val(val)
    {m_val=val;}

    virtual void getString(char *buf, uint8_t bufLen) {
        snprintf(buf, bufLen, "%d", m_val);
    }

    void set(uint32_t val){m_val = val;};
private:
    int& m_val;
};

class MenuDouble: public MenuAction {
public:
    MenuDouble(char const *name, double val):
        MenuAction(name), m_val(val)
    {m_val=val;}

    virtual void getString(char *buf, uint8_t bufLen) {
        fmtDouble(m_val, 3, buf, bufLen);
    }

    void set(double val){m_val = val;};
private:
    double m_val;
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif