////////////////////////////////////////////////////////////////////////////////
// The following FIT Protocol software provided may be used with FIT protocol
// devices only and remains the copyrighted property of Garmin Canada Inc.
// The software is being provided on an "as-is" basis and as an accommodation,
// and therefore all warranties, representations, or guarantees of any kind
// (whether express, implied or statutory) including, without limitation,
// warranties of merchantability, non-infringement, or fitness for a particular
// purpose, are specifically disclaimed.
//
// Copyright 2020 Garmin Canada Inc.
////////////////////////////////////////////////////////////////////////////////
// ****WARNING****  This file is auto-generated!  Do NOT edit this file.
// Profile Version = 21.40Release
// Tag = production/akw/21.40.00-0-g813c158
////////////////////////////////////////////////////////////////////////////////


#include "fit_mesg_with_event_broadcaster.hpp"
#include <algorithm>

namespace fit
{

#define MAX_GROUPS         256
#define DEFAULT_GROUP      255
#define BEGIN_END_GROUP    254

MesgWithEventBroadcaster::MesgWithEventBroadcaster(void)
{
    for (int i = 0; i < MAX_GROUPS; i++) {
        startedEvents.push_back(std::list<MesgWithEvent*>());
    }
}

void MesgWithEventBroadcaster::AddListener(MesgWithEventListener& mesgObserver)
{
    listeners.push_back(&mesgObserver);
}

void MesgWithEventBroadcaster::RemoveListener(MesgWithEventListener& mesgObserver)
{
    std::vector<MesgWithEventListener*>::iterator it = std::find(listeners.begin(), listeners.end(), &mesgObserver);
    if (listeners.end() != it)
        listeners.erase(it);
}

void MesgWithEventBroadcaster::OnMesg(MesgWithEvent& mesg)
{
    (void)mesg;
    /*
    MesgWithEvent broadcastMesg(mesg);
    int group = DEFAULT_GROUP;

    if (broadcastMesg.GetEventGroup() != UINT8_INVALID)
    {
       group = broadcastMesg.GetEventGroup();
    }

    if (broadcastMesg.GetEventType() == EVENT_TYPE_INVALID)
       return; // Invalid so ignore.

    // Convert depreciated events types for backwards compatibility.
    switch (broadcastMesg.GetEventType())
    {
       case EVENT_TYPE_BEGIN_DEPRECIATED:
          group = BEGIN_END_GROUP;
          broadcastMesg.SetEventType(EVENT_TYPE_START);
          break;

       case EVENT_TYPE_END_DEPRECIATED:
          group = BEGIN_END_GROUP;
          broadcastMesg.SetEventType(EVENT_TYPE_STOP);
          break;

       case EVENT_TYPE_CONSECUTIVE_DEPRECIATED:
          broadcastMesg.SetEventType(EVENT_TYPE_STOP);
          break;

       case EVENT_TYPE_END_ALL_DEPRECIATED:
          group = BEGIN_END_GROUP;
          broadcastMesg.SetEventType(EVENT_TYPE_STOP_ALL);
          break;

       default:
          break;
    }

    switch (broadcastMesg.GetEventType())
    {
       case EVENT_TYPE_START:
          for (list<int>::iterator it = startedEvents.begin(); it < startedEvents[group].end(); it++)
          {
             if (startedEvents[group][i].GetEvent() == broadcastMesg.GetEvent())
             {
                MesgWithEvent stopEvent(startedEvents[group].[i]);
                stopEvent.SetEventType(EVENT_TYPE_STOP);
                stopEvent.SetTimestamp(broadcastMesg.GetTimestamp());
                Broadcast(stopEvent);
                startedEvents[group].remove(stopEvent);
             }
          }

          startedEvents[group].add(broadcastMesg);
          break;

       case EVENT_TYPE_STOP:
       case EVENT_TYPE_STOP_DISABLE:
          for (int i = 0; i < startedEvents.get(group).size(); i++) {
             if (startedEvents.get(group).get(i).getEvent() == broadcastMesg.getEvent()) {
                startedEvents.get(group).remove(i);
             }
          }
          break;

       case EVENT_TYPE_STOP_ALL:
          for (int i = 0; i < (int)startedEvents[group].size(); i++) {
             if (startedEvents.get(group).get(i).getEvent() != broadcastMesg.getEvent()) {
                MesgWithEvent stopEvent = (MesgWithEvent)Factory.createMesg((Mesg)startedEvents.get(group).get(i));
                stopEvent.setEventType(EventType.STOP);
                stopEvent.setTimestamp(broadcastMesg.getTimestamp());
                broadcast(stopEvent);
             }
          }

          startedEvents.get(group).clear();
          broadcastMesg.setEventType(EventType.STOP);
          break;

       case EVENT_TYPE_STOP_DISABLE_ALL:
          for (int i = 0; i < startedEvents.get(group).size(); i++) {
             if (startedEvents.get(group).get(i).getEvent() != broadcastMesg.getEvent()) {
                MesgWithEvent stopEvent = (MesgWithEvent)Factory.createMesg((Mesg)startedEvents.get(group).get(i));
                stopEvent.setEventType(EVENT_TYPE_STOP_DISABLE);
                stopEvent.setTimestamp(broadcastMesg.getTimestamp());
                broadcast(stopEvent);
             }
          }

          startedEvents.get(group).clear();
          broadcastMesg.SetEventType(EVENT_TYPE_STOP_DISABLE);
          break;

       case EVENT_TYPE_MARKER:
       default:
          break;
    }

    Broadcast(broadcastMesg);

    */
}

void MesgWithEventBroadcaster::Broadcast(MesgWithEvent& mesg)
{
    for (int i = 0; i < (int)listeners.size(); i++)
    {
        listeners[i]->OnMesg(mesg);
    }
}


} // namespace fit
