#pragma once
#ifndef OBJECT_H
#define OBJECT_H
#define PI 3.1415926535
#include "shader.h"
#include <vector>
#include <any>
#include <algorithm>
#include <utility>
#include <functional>

struct EventListener
{
    std::string name;
    std::string key;
    unsigned int func_id;

    EventListener(const std::string& eventname, unsigned int id, const std::string& key = "") :
        name(eventname), func_id(id), key(key)
    {

    }
};

struct TimeTasker
{
    float max_t;
    float cur_t;
    std::string key;
    unsigned int func_id;

    TimeTasker(float time, unsigned int id, const std::string& key = "") :
        max_t(time), cur_t(0), func_id(id), key(key)
    {

    }
};

struct PeriodicTasker
{
    float max_t;
    float cur_t;
    float period;
    unsigned int times;
    std::string key;
    unsigned int func_id;

    PeriodicTasker(float period, unsigned int id, float max_time = INFINITY, const std::string& key = "") :
        max_t(max_time), cur_t(0), times(0), period(period), func_id(id), key(key)
    {

    }
};

typedef std::vector<std::any> DATA;

class Object
{
public:
    std::vector<std::string> tags;
    std::vector<EventListener> listeners;
    std::vector<TimeTasker> ttaskers;
    std::vector<PeriodicTasker> ptaskers;

protected:
    virtual void Draw(Shader& shader) = 0;

    virtual void TimeTask(unsigned int id)
    {

    }

    virtual void EventTask(unsigned int id, DATA data = DATA{})
    {

    }

    virtual void PeriodicTask(unsigned int id, float time)
    {

    }

public:
    virtual ~Object() {};

    bool HasTag(const std::string& tag) const
    {
        for (auto s : tags)
            if (s == tag)
                return true;
        return false;
    }

    void AddTag(const std::string& tag)
    {
        std::string t(tag);
        tags.push_back(t);
    }

    void RemoveTag(const std::string& tag)
    {
        std::vector<std::string>::iterator iter;
        for (iter = tags.begin(); iter != tags.end(); )
        {
            if (*iter == tag)
            {
                iter = tags.erase(iter);
            }
            else
                ++iter;
        }
    }

    void ListenForEvent(const std::string& eventname, unsigned int id, const std::string& key = "")
    {
        for (auto l : listeners)
        {
            if (l.name == eventname && l.func_id == id && l.key == key)
                return;
        }
        listeners.push_back(EventListener(eventname, id, key));
    }

    //非常重要，remove操作必须放到下一帧在做，不然必定炸裂
    void RemoveEventListener(const std::string& eventname, const std::string& key = "")
    {
        for (auto iter = listeners.begin(); iter != listeners.end(); )
        {
            if (key == "" && (*iter).name == eventname || key == (*iter).key && (*iter).name == eventname)
            {
                iter = listeners.erase(iter);
            }
            else
                ++iter;
        }
    }

    void PushEvent(const std::string& eventname, DATA data = DATA{})
    {
        for (auto l : listeners)
        {
            if (l.name == eventname)
                EventTask(l.func_id, data);
        }
    }

    void DoTaskInTime(float time, unsigned int id, const std::string& key = "")
    {
        for (auto t : ttaskers)
        {
            if (t.func_id == id && t.key == key)
                return;
        }
        ttaskers.push_back(TimeTasker(time, id, key));
    }
    
    void CancelTimeTask(const std::string& key)
    {
        for (auto iter = ttaskers.begin(); iter != ttaskers.end(); )
        {
            if ((*iter).key == key)
                iter = ttaskers.erase(iter);
            else
                ++iter;
        }
    }

    void DoPeriodicTask(float period, unsigned int id, float max_time = INFINITY, const std::string& key = "")
    {
        for (auto p : ptaskers)
        {
            if (p.func_id == id && p.key == key)
                return;
        }
        ptaskers.push_back(PeriodicTasker(period, id, max_time, key));
    }

    void CancelPeriodicTask(const std::string& key)
    {
        for (auto iter = ptaskers.begin(); iter != ptaskers.end(); )
        {
            if ((*iter).key == key)
                iter = ptaskers.erase(iter);
            else
                ++iter;
        }
    }

    void TimeTick(float deltaT)
    {
        for (auto iter = ttaskers.begin(); iter != ttaskers.end(); )
        {
            (*iter).cur_t += deltaT;
            if ((*iter).cur_t >= (*iter).max_t)
            {
                TimeTask((*iter).func_id);
                if (iter == ttaskers.end())
                    goto next;
                iter = ttaskers.erase(iter);
            }
            else
                ++iter;
        }
    next:;
        for (auto iter = ptaskers.begin(); iter != ptaskers.end(); )
        {
            (*iter).cur_t += deltaT;
            if ((*iter).cur_t >= (*iter).max_t)
            {
                iter = ptaskers.erase(iter);
                continue;
            }
            
            if ((*iter).cur_t >= (*iter).period * ((*iter).times + 1))
            {
                PeriodicTask((*iter).func_id, (*iter).cur_t);
                if (iter == ptaskers.end())
                    break;
                (*iter).times++;
            }
            ++iter;
        }
    }
};

#endif