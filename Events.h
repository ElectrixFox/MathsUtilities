#ifndef EVENTS_H
#define EVENTS_H

#include "Utility.h"
#include "qt_pch.h"
/* 
class CustomEvent : public QEvent 
    {
    public:
    CustomEvent() : QEvent(QEvent::Type(QEvent::User + 1)) { std::cout << "\nEVENTING"; }
    };

enum EVENT_TYPE
{
    NODE_CREATED
};

class EventsManager : public QObject
    {
    public:
    static EventsManager& instance()
    {
    static EventsManager ev;
    return ev;
    }
    
    void callEvent(EVENT_TYPE type) { evLst.push_back(type); };

    EVENT_TYPE getEvents() { for(EVENT_TYPE e : evLst)  { return e; }; };

    protected:
        bool event(QEvent* event) override;

    private:
    std::vector<EVENT_TYPE> evLst;
    EventsManager() { };

    EventsManager(const EventsManager&) = delete;
    EventsManager& operator=(const EventsManager&) = delete;
    };
 */
#endif