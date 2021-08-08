/********************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  MessageMapManager.h
    ˵    ����  ��Ϣӳ������

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

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
        // ӳ�����洢����
        typedef std::vector<T*> ObserverVector;

        // ӳ�������
        typedef std::map<uint16_t, ObserverVector> MessageMap;

    public:
        MessageMapManager();
        ~MessageMapManager();

        // ע��һ��ӳ���ϵ
        void regMap(uint16_t messageType, T* pObserver);

        // ע��һ��ӳ���ϵ
        void delMap(uint16_t messageType, T* pObserver);

        // ע������ӳ���ϵ
        void delMap(T* pObserver);

        // ����ָ����Ϣ�����µ�����Observer
        const ObserverVector* getObservers(uint16_t messageType) const;

        // ���ָ����Ϣ�����µ�����Observer
        const void clear(uint16_t messageType);

        // �������ӳ���ϵ
        const void clear();

    protected:
        // ���ָ����Ϣ�������ӳ����Ƿ����ָ����Observer
        bool checkObserver(uint16_t messageType, const T* pObserver) const;

        // ���ӳ���ʱ�Ƿ����ָ����Ϣ����
        bool checkMessageType(uint16_t messageType) const;

    private:
        // ӳ���
        MessageMap m_messageMap;
    };




    //**************************************************
    //ģ��ʵ��
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
            // ��������������ӣ����򲻴���
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
            // ��ʱ����һ��Vector�����ڲ��뵽Map
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
                // ��֤ÿһ��Vector��ֻ��һ��Observer
                observers.erase(ovPos);
            }
        }
    }

    template<class T>
    void MessageMapManager<T>::delMap(T* pObserver)
    {
        MessageMap::iterator end = m_messageMap.end();
        MessageMap::iterator pos = m_messageMap.begin();

        // ��������MessageType������Observer
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
