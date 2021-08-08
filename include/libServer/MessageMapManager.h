/********************************************************************

    创建日期：  2021年7月24日
    文件名称：  MessageMapManager.h
    说    明：  信息映射表管理

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

********************************************************************/
#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include "Observer.h"
#include "LogMacro.h"

namespace server
{
    template<class T>
    class MessageMapManager
    {
    public:
        // 映射对像存储类型
        typedef std::vector<T*> ObserverVector;

        // 映射表类型
        typedef std::map<uint16_t, ObserverVector> MessageMap;

    public:
        MessageMapManager();
        ~MessageMapManager();

        // 注册一条映射关系
        void regMap(uint16_t messageType, T* pObserver);

        // 注销一条映射关系
        void delMap(uint16_t messageType, T* pObserver);

        // 注销所有映射关系
        void delMap(T* pObserver);

        // 查找指定信息类型下的所有Observer
        const ObserverVector* getObservers(uint16_t messageType) const;

        // 清空指定信息类型下的所有Observer
        const void clear(uint16_t messageType);

        // 清空所有映射关系
        const void clear();

    protected:
        // 检查指定信息类型里的映射表是否存在指定的Observer
        bool checkObserver(uint16_t messageType, const T* pObserver) const;

        // 检查映射表时是否存在指定信息类型
        bool checkMessageType(uint16_t messageType) const;

    private:
        // 映射表
        MessageMap m_messageMap;
    };




    //**************************************************
    //模板实现
    //**************************************************
    template<class T>
    MessageMapManager<T>::MessageMapManager()
    {

    }

    template<class T>
    MessageMapManager<T>::~MessageMapManager()
    {

    }

    template<class T>
    void MessageMapManager<T>::regMap(uint16_t messageType, T* pObserver)
    {
        if (NULL == pObserver)
        {
            CONSOLE_WRITE("regMap: Observer is NULL.(message type:%d)", messageType);
            return;
        }

        MessageMap::iterator pos = m_messageMap.find(messageType);
        if (m_messageMap.end() != pos)
        {
            // 如果不存在则增加，否则不处理
            if (!checkObserver(messageType, pObserver))
            {
                ObserverVector& observers = pos->second;
                observers.push_back(pObserver);
                if (observers.size() > 1)
                {
                    CONSOLE_WRITE("messageType more than 1 observer:%d", messageType);
                }
            }
            else
            {
                CONSOLE_WRITE("regMap same observer:%d", messageType);
            }
        }
        else
        {
            // 临时产生一个Vector，用于插入到Map
            ObserverVector temp;
            temp.push_back(pObserver);

            m_messageMap.insert(make_pair(messageType, temp));
        }
    }

    template<class T>
    void MessageMapManager<T>::delMap(uint16_t messageType, T* pObserver)
    {
        if (NULL == pObserver)
        {
            return;
        }

        ObserverVector::iterator ovPos;
        MessageMap::iterator pos = m_messageMap.find(messageType);

        if (m_messageMap.end() != pos)
        {
            ObserverVector& observers = pos->second;

            ovPos = find(observers.begin(), observers.end(), pObserver);
            if (observers.end() != ovPos)
            {
                // 保证每一个Vector里只有一个Observer
                observers.erase(ovPos);
            }
        }
    }

    template<class T>
    void MessageMapManager<T>::delMap(T* pObserver)
    {
        MessageMap::iterator end = m_messageMap.end();
        MessageMap::iterator pos = m_messageMap.begin();

        // 遍历所有MessageType，查找Observer
        for (; pos != end; ++pos)
        {
            delMap(pos->first, pObserver);
        }
    }

    template<class T>
    typename const MessageMapManager<T>::ObserverVector* MessageMapManager<T>::getObservers(uint16_t messageType) const
    {
        const ObserverVector* pObservers = NULL;
        MessageMap::const_iterator pos = m_messageMap.find(messageType);
        if (m_messageMap.end() != pos)
        {
            pObservers = &(pos->second);
        }

        return pObservers;
    }

    template<class T>
    const void MessageMapManager<T>::clear(uint16_t messageType)
    {
        MessageMap::iterator pos = m_messageMap.find(messageType);
        if (m_messageMap.end() != pos)
        {
            pos->second.clear();
        }
    }

    template<class T>
    const void MessageMapManager<T>::clear()
    {
        m_messageMap.clear();
    }

    template<class T>
    bool MessageMapManager<T>::checkObserver(uint16_t messageType, const T* pObserver) const
    {
        bool result = false;

        MessageMap::const_iterator pos = m_messageMap.find(messageType);
        if (m_messageMap.end() != pos)
        {
            const ObserverVector& observers = pos->second;
            ObserverVector::const_iterator itor = find(observers.begin(), observers.end(), pObserver);

            result = observers.end() != itor;
        }

        return result;
    }

    template<class T>
    bool MessageMapManager<T>::checkMessageType(uint16_t messageType) const
    {
        MessageMap::const_iterator pos = m_messageMap.find(messageType);
        return m_messageMap.end() != pos;
    }

}
