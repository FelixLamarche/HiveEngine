#include "EventManager.h"


hive::EventManager::EventManager()
{
}

hive::EventManager::~EventManager()
{
}

void hive::EventManager::EventPush(const Event& event, const uint16 code)
{
    p_events.push({event, code});
}

void hive::EventManager::EventFlush()
{
    while (!p_events.empty())
    {
        auto &event_data = p_events.front();
        p_events.pop();

        for (const auto &[id, callback] : p_callbacks[event_data.code])
        {
            callback(event_data.code, event_data.event);
        }
    }
}

hive::EventManager::CallbackID hive::EventManager::CallbackSubscribe(const uint16 category, const PFN_On_Event& callback)
{
    const int id = p_callbacks[category].size();
    p_callbacks[category].insert({id, callback});

    return {category, id};
}

void hive::EventManager::CallbackUnSubscribe(const CallbackID id)
{
    if (p_callbacks.contains(id.cat) && p_callbacks[id.cat].contains(id.id))
        p_callbacks[id.cat].erase(id.id);
}
