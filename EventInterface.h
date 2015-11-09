//EventInterface.h

#ifndef EVENTINTERFACE_H
#define EVENTINTERFACE_H

#define FUNCTION_TO_CALL BIND_MEM_CB
#define FUNCTION_TO_CALL_STATIC BIND_FREE_CB    //Use this instead for global/static functions

#define DEBUG

#include <vector>
#include <queue>
#include <map>

//-------------------------------------------------------------------------------------- -
#include "callback.hpp"	//Header file for delegate functionality
//
// Found online at : http://www.codeproject.com/Articles/136799/Lightweight-Generic-C-Callbacks-or-Yet-Another-Del
//
//-------------------------------------------------------------------------------------- -

using util::Callback;	//Delegate

//-------------------------------------------------------------------------------------- -
//	TypeDefs
//-------------------------------------------------------------------------------------- -
typedef char* eventData;
typedef Callback<void()> FunctionPointer;

//-------------------------------------------------------------------------------------- -
//	Event
//		-This class is completely private, only access is allowed to Senders and Listeners
//-------------------------------------------------------------------------------------- -
class Event
{
private:
	friend class EventSender;
	friend class EventListener;

	eventData m_pName;

	Event(eventData pName);

	void TriggerEvent();
};

//-------------------------------------------------------------------------------------- -
//	EventSender
//		-This class acts as the interface for Creating and Triggering events
//      -**It's responsible for cleaning up any events cleated**-
//-------------------------------------------------------------------------------------- -
class EventSender
{
private:
	std::map<eventData, Event*> m_events;

public:
	EventSender();
    ~EventSender();

	void CreateEvent(eventData eventName);
	void TriggerEvent(eventData eventName);
};

//-------------------------------------------------------------------------------------- -
//	EventListener
//		-This class acts as the interface for Receiving events
//-------------------------------------------------------------------------------------- -
class EventListener
{
private:
	friend class EventSystem;

	std::map<eventData, Event*> m_events;
	std::map<eventData, FunctionPointer> m_functionToExecute;

public:
	EventListener();
    ~EventListener();

	void ListenForEvent(eventData eventName, FunctionPointer functionToCall);

private:
	void ExecuteEvent(eventData eventName);
	bool ContainsEvent(eventData eventName);
};

#include "Singleton.h"

//-------------------------------------------------------------------------------------- -
//	EventSystem
//		-This class is completely hidden by everyone but EventSenders and Listeners
//
//      -It maintains 2 collections: 1 of EventSenders and 1 for EventListeners
//
//      -It provides an interface for Event Senders and Listeners to communicate
//       with eachother. When a sender recieves a call to trigger an event,
//       it calls this class and tells it to look for every listener with the
//       corresponding event and this class will trigger each listener's specified event.
//-------------------------------------------------------------------------------------- -
class EventSystem : public Singleton<EventSystem>
{
private:
	friend class Singleton <EventSystem>;
	friend class EventListener;
	friend class EventSender;

	std::vector<EventSender*> m_senders;
	std::vector<EventListener*> m_listeners;

	//This will set off an event by key-value
	void TriggerEvent(eventData eventName) const;

	//If registered as a listener, this will allow you to subscribe to new events
	void SubscribeToEvent(eventData eventName, EventListener* pListener);

	//**These functions must be called first to be able to handle events**//
	void RegisterSender(EventSender* pSender);
	void RegisterListener(EventListener* pListener);

#ifdef DEBUG    //Debugging functions
    std::queue<eventData> m_currentEventsThisUpdate;
    //[???] What data type would be most efficient for
    //      something like this that needs to be constantly iterated
    //      on and cleared every update?
    //      Every operation would essentially be O(n)
    //      so I guess the important factors are the constants
    //      involved with each operation

    //Logging functions
    //
    //This will log every event FIRED this update
    void LogEvent(eventData eventName);
    //This needs to be called at the end of every update
    void ClearLogs();

#endif // DEBUG

    //[???]
    //A friend gave me an idea to be able to print the callstack on certain events
    //any idea how to implement something like that?
};

#endif // !EVENTINTERFACE_H
