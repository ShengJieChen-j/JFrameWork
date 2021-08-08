/****************************************************************
    创建日期：  2021年7月24日
    文件名称：  MsgAdapter.h
    说    明：  消息适配

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include "MsgInterface.h"
#include "libBase/TypeDefine.h"

namespace server
{
    class MsgAdapter : public IMsg
    {
    public:
        MsgAdapter();
        ~MsgAdapter();

        // 绑定信息
        void bind(const char*, uint32_t unSocketID, int32_t nType, int64_t n64Id, int32_t netType);

        virtual const char* GetBuf() const;
        virtual uint16_t GetSize() const;
        virtual uint16_t GetType() const;
        virtual uint16_t GetHeadSize() const;
        virtual bool IsValid() const;
        virtual uint32_t GetSocketID() const;
        virtual int32_t GetConnectType() const;
        virtual int64_t	GetID() const;
        virtual int32_t getNetType() const;

    private:
        const char* m_pBuffer;		    // 消息地址
        uint32_t m_SocketID;	        // SocketID
        int32_t m_ConnectType;			// 连接类型
        int64_t m_n64ID;				// ID(可以服务器ID 也可以是玩家DBID)
        int32_t m_NetType;				// 网络类型
    };

}
