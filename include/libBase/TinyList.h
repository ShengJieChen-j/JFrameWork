/****************************************************************
	创建日期：  2021/7/15
	文件名称：  TinyList.h
	说    明：  简易的侵入式双链表

	当前版本：  1.00
	作    者：  shengjiechen
	概    述：

******************************************************************/
#pragma once

namespace base
{
	template<class T>
	struct STinyListNode
	{
		STinyListNode* m_pPre;
		STinyListNode* m_pNext;
		T              m_Val;

		STinyListNode();
		~STinyListNode();

		void remove();
		bool isHang() const;
	};

	template<class NodeType>
	class CTinyList
	{
	private:
		NodeType m_Head;
		NodeType m_Tail;

	public:
		CTinyList();
		~CTinyList();

		bool      pushBack(NodeType* pNode);
		NodeType* getBack();

		bool      pushFront(NodeType* pNode);
		NodeType* getFront();

		bool      isEmpty() const;
	};

	template<class T>
	STinyListNode<T>::STinyListNode()
	{
		std::cout << "This is STinyListNode" << std::endl;
		this->m_Val = T();
		this->m_pNext = NULL;
		this->m_pPre = NULL;
	};

	template<class T>
	STinyListNode<T>::~STinyListNode()
	{
		std::cout << "This is ~STinyListNode" << std::endl;
		this->remove();
	};

	template<class T>
	void STinyListNode<T>::remove()
	{
		if (this->m_pNext && this->m_pPre)
		{
			this->m_pPre->m_pNext = this->m_pNext;
			this->m_pNext->m_pPre = this->m_pPre;
		}
		this->m_pPre = nullptr;
		this->m_pNext = nullptr;
	};

	template<class T>
	bool STinyListNode<T>::isHang() const
	{
		return (m_pNext) && (m_pPre);
	};

	template <class NodeType>
	CTinyList<NodeType>::CTinyList()
	{
		std::cout << "this is CTinyList" << std::endl;
		this->m_Head.m_pPre = nullptr;
		this->m_Head.m_pNext = &this->m_Tail;
		this->m_Tail.m_pPre = &this->m_Head;
		this->m_Tail.m_pNext = nullptr;
	};

	template <class NodeType>
	CTinyList<NodeType>::~CTinyList()
	{

	}

	template <class NodeType>
	bool CTinyList<NodeType>::pushBack(NodeType* pNode)
	{
		std::cout << "this is pushBack" << std::endl;
		if (!pNode)
		{
			return false;
		}

		if (pNode->isHang())
		{
			return false;
		}
		pNode->m_pNext = &this->m_Tail;
		pNode->m_pPre = this->m_Tail.m_pPre;
		this->m_Tail.m_pPre->m_pNext = pNode;
		this->m_Tail.m_pPre = pNode;

		return true;
	};

	template <class NodeType>
	NodeType* CTinyList<NodeType>::getBack()
	{
		std::cout << "this is getBack" << std::endl;
		if (this->isEmpty())
		{
			return nullptr;
		}
		return this->m_Tail.m_pPre;
	}

	template <class NodeType>
	bool CTinyList<NodeType>::pushFront(NodeType* pNode)
	{
		std::cout << "this is pushBack" << std::endl;

		if (!pNode)
		{
			return false;
		}

		if (pNode->isHang())
		{
			return false;
		}


		pNode->m_pPre = &this->m_Head;
		pNode->m_pNext = this->m_Head.m_pNext;
		this->m_Head.m_pNext->m_pPre = pNode;
		this->m_Head.m_pNext = pNode;
		return true;
	}

	template <class NodeType>
	NodeType* CTinyList<NodeType>::getFront()
	{
		std::cout << "this is getBack" << std::endl;
		if (this->isEmpty())
		{
			return nullptr;
		}

		return this->m_Head.m_pNext;
	}

	template <class NodeType>
	bool CTinyList<NodeType>::isEmpty() const
	{
		return this->m_Head.m_pNext == &this->m_Tail;
	}

}