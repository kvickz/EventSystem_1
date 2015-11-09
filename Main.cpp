//Michael Nevins
//GAP 255 - Design Patterns
//Module 9 - Event System Design
//Main.cpp

#include <iostream>
#include <conio.h>
#include <map>
#include <vector>
#include <string>

#include <vld.h>

#include "Singleton.h"
#include "EventInterface.h"

//-------------------------------------------------------------------------------------- -
//  Summary
//-------------------------------------------------------------------------------------- -
//	My goal with this was to create an event system that is as
//	generic and as simple to use as possible.
//
//  A system that you can easily copy and paste for any new
//  game you would be creating so that you could easily modify
//  it to tailor it for that specific game.
//
//	So, I compromised on efficiency for ease of use	and I tried to find the simplest 
//  implementation I could think of without compromising the ability to do anything 
//  that any event system would need to be able to do.
//
//  The biggest weakness currently is that events aren't passed with any additional data.
//  I tried to think of a good generic way to accomplish this but I didn't come
//  up with anything that felt satisfactory. So right now events must be defined
//  with specific names that determine what kind of reaction you want.
//
//  Another issue that I haven't compensated for are event-key collisions.
//  If you create two events of the same key-value(name) there is no defined behavior.
//
//-------------------------------------------------------------------------------------- -
//How to Create and Listen for events:
//
//Step 1. Create an EventSender
//
//Step 2. Create an event:
//	ex.		eventSender.CreateEvent("GameWon");
//
//Step 3. Create an EventListener
//
//Step 4. Subscribe to events:
//	ex.		eventListener.ListenForEvent("GameWon", FUNCTION_TO_CALL(&Class::Function, this));
//
//Step 5. Trigger the event from any EventSender
//	ex.	    eventSender.TriggerEvent("GameWon");
//
// Done!
//
//After this has happened the event system will search for any listeners registered to
//that event and call the functions linked up to them
//-------------------------------------------------------------------------------------- -

//-------------------------------------------------------------------------------------- -
//  Description of Event System Implementation
//
//      -The way I approached this was to create 2 public interfaces for
//      any object to use. EventSender and EventListener.
//      
//      The exposed functions within this interface will communicate with
//      an essentially private class, EventSystem. EventSystem will maintain
//      collections of all of the EventListeners and Senders, and handle
//      all of their communication. 
//
//      EventSystem does NOT trigger any events itself.
//
//      It simply acts as a relay station, that will say,
//      Hey, if you are a listener and have this event, it has been triggered.
//      So then any listeners containing that event will call the associated
//      functions through delegates defined by the user.
//
//      An important thing to note is that the user of the system is responsible
//      for keeping track of their events because there are no hard-coded
//      key-values that intellisense will help you with.
//
//-------------------------------------------------------------------------------------- -

//-------------------------------------------------------------------------------------- -
//	Game
//-------------------------------------------------------------------------------------- -
class Game
{
private:
	bool m_gameRunning = true;
	EventListener* m_pListener;

public:
	Game();
	~Game();

	void EndGame();
	bool IsRunning() { return m_gameRunning; }
};

Game::Game()
{
	m_pListener = new EventListener();

	m_pListener->ListenForEvent("OnPlayerDeath", FUNCTION_TO_CALL(&Game::EndGame, this));
}

void Game::EndGame() 
{ 
	m_gameRunning = false; 
}

Game::~Game()
{
	delete m_pListener;
	m_pListener = nullptr;
}

//-------------------------------------------------------------------------------------- -
//	Actor
//-------------------------------------------------------------------------------------- -
class Actor
{
private:
	unsigned int m_playerNumber;
	EventSender* m_pEventSender;

public:
	Actor(int playerNumber);
	~Actor();

	void Attack(Actor* pActor);
	void Die();
};

Actor::Actor(int playerNumber)
	:m_playerNumber(playerNumber)
{
	m_pEventSender = new EventSender();

	char* pEventName = "OnPlayerDeath";

	m_pEventSender->CreateEvent(pEventName);
}

Actor::~Actor()
{
	delete m_pEventSender;
	m_pEventSender = nullptr;
}

void Actor::Attack(Actor* pActor)
{
	std::cout << "Player " << m_playerNumber << " attacks Player " << pActor->m_playerNumber << ".\n";
	pActor->Die();
}

void Actor::Die()
{
	m_pEventSender->TriggerEvent("OnPlayerDeath");
}

//-------------------------------------------------------------------------------------- -
//	Main
//-------------------------------------------------------------------------------------- -
void main()
{
	Game game;

	Actor actor1(1);
	Actor actor2(2);

	actor1.Attack(&actor2);
    //This kills actor2

	while (game.IsRunning())
	{
		std::cout << "Game Running.\n";
		std::cout << "Press any key to continue.\n";
		_getch();

		std::cout << "\n\n";
	}

	std::cout << "Game has ended!" << std::endl;

    EventSystem::DestroySingleton();

	_getch();
}