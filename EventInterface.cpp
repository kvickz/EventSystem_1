//EventSender.cpp

#include "EventInterface.h"

#include <iostream>

//-------------------------------------------------------------------------------------- -
//	Event - Implementation
//-------------------------------------------------------------------------------------- -
Event::Event(eventData pName)
	:m_pName(pName)
{ }

void Event::TriggerEvent()
{
	std::cout << "Event Triggered" << std::endl;
}

//-------------------------------------------------------------------------------------- -
//	EventSender Implementation
//-------------------------------------------------------------------------------------- -
EventSender::EventSender()
{
	EventSystem::Get()->RegisterSender(this);
}

EventSender::~EventSender()
{
    //Deleting all events
    auto iterator = m_events.begin();

    while (iterator != m_events.end())
    {
        delete iterator->second;
        iterator->second = nullptr;

        ++iterator;
    }

    m_events.clear();
}

void EventSender::CreateEvent(eventData eventName)
{
	Event* m_pEvent = new Event(eventName);
	m_events.emplace(eventName, m_pEvent);

	std::cout << "Event Created: " << eventName << std::endl;
}

void EventSender::TriggerEvent(eventData eventName)
{
	std::cout << "Event Triggered: " << eventName << std::endl << std::endl;
	EventSystem::Get()->TriggerEvent(eventName);

	
}

//-------------------------------------------------------------------------------------- -
//	EventListener Implementation
//-------------------------------------------------------------------------------------- -
//
//Public
//
EventListener::EventListener()
{
	//RegisterListener
	EventSystem::Get()->RegisterListener(this);
}

EventListener::~EventListener()
{
    //Deleting all events
    auto iterator = m_events.begin();

    while (iterator != m_events.end())
    {
        delete iterator->second;
        iterator->second = nullptr;

        ++iterator;
    }

    m_events.clear();
}

void EventListener::ListenForEvent(eventData eventName, FunctionPointer functionToCall)
{
	Event* pTempEvent = new Event(eventName);

	m_events.emplace(eventName, pTempEvent);
	m_functionToExecute.emplace(eventName, functionToCall);
}
//
//Private Functions
//
void EventListener::ExecuteEvent(eventData eventName)
{
	m_functionToExecute[eventName]();

    EventSystem::Get()->LogEvent(eventName);
}

bool EventListener::ContainsEvent(eventData eventName)
{
	//Look to see if this listener contains the event
	auto iterator = m_events.find(eventName);

	if (iterator != m_events.end())
	{
		return true;	//Found the event
	}
	else
	{
		return false;	//Did not find the event
	}
}

//-------------------------------------------------------------------------------------- -
//	EventSystem - Implementation
//-------------------------------------------------------------------------------------- -

void EventSystem::TriggerEvent(eventData eventName) const
{
	//Loop through all Listeners looking to see if they're listening for this event
	//If they are, execute their OnEvent function

	auto iteratorSender = m_listeners.begin();

	while (iteratorSender != m_listeners.end())
	{
		if ((*iteratorSender)->ContainsEvent(eventName))
			(*iteratorSender)->ExecuteEvent(eventName);

		++iteratorSender;
	}
}

void EventSystem::SubscribeToEvent(eventData eventName, EventListener* pListener)
{
	//pListener->ListenForEvent(eventName);

	m_listeners.push_back(pListener);
}

//Private functions
void EventSystem::RegisterSender(EventSender* pSender)
{
	m_senders.push_back(pSender);
}

void EventSystem::RegisterListener(EventListener* pListener)
{
	m_listeners.push_back(pListener);
}

//  Log Event Function
//      -This function is designed to keep track of the order of events every update
//       and clear each update as well.
void EventSystem::LogEvent(eventData eventName)
{
    m_currentEventsThisUpdate.push(eventName);

    std::cout << "Event: " << eventName << " executed.\n";
}

//  Clear Logs Function
//      -This function needs to be called by the game manager at the end of every update!
void EventSystem::ClearLogs()
{
    while (!m_currentEventsThisUpdate.empty())
    {
        m_currentEventsThisUpdate.pop();
    }
}
