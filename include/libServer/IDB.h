/****************************************************************

	创建日期：  2021年7月24日
	文件名称：  IDB.h
	说    明：  DB interface

	当前版本：  1.00
	作    者：  shengjiechen
	概    述：

******************************************************************/

#pragma once

#ifndef MYSQL_PORT
#define MYSQL_PORT 3306
#endif

namespace dbtool
{
	const int	_MAX_SQL = 60000;

	class IData
	{
	public:
		virtual					~IData() {}
		virtual	unsigned int	Release(void) = 0;

		// conversion function
		virtual					operator bool() const = 0;

		virtual					operator char() const = 0;
		virtual					operator unsigned char() const = 0;

		virtual					operator short() const = 0;
		virtual					operator unsigned short() const = 0;

		virtual					operator long() const = 0;
		virtual					operator unsigned long() const = 0;

		virtual					operator int() const = 0;
		virtual					operator unsigned int() const = 0;
#if defined WIN32
		virtual					operator __int64() const = 0;
		virtual					operator unsigned __int64() const = 0;
#elif defined __LINUX__
		virtual					operator long long() const = 0;
		virtual					operator unsigned long long() const = 0;
#endif
		virtual					operator float() const = 0;
		virtual					operator double() const = 0;

		virtual					operator char* () const = 0;
		virtual					operator const char* () const = 0;

		// overload assignment operator
		virtual IData& operator = (bool bOp) = 0;

		virtual IData& operator = (char chOp) = 0;
		virtual IData& operator = (unsigned char chOp) = 0;

		virtual IData& operator = (short nOp) = 0;
		virtual IData& operator = (unsigned short unOp) = 0;

		virtual IData& operator = (long lOp) = 0;
		virtual IData& operator = (unsigned long lOp) = 0;

		virtual IData& operator = (int iOp) = 0;
		virtual IData& operator = (unsigned int uOp) = 0;
#if defined WIN32
		virtual IData& operator = (__int64 uOp) = 0;
		virtual IData& operator = (unsigned __int64 uOp) = 0;
#elif defined __LINUX__
		virtual IData& operator = (long long uOp) = 0;
		virtual IData& operator = (unsigned long long uOp) = 0;
#endif
		virtual IData& operator = (float fltOp) = 0;
		virtual IData& operator = (double dblOp) = 0;

		virtual IData& operator = (const char* szVal) = 0;
	};

	//////////////////////////////////////////////////////////////////////
	class IRecord
	{
	public:
		IRecord() {}
		virtual ~IRecord() {}
		virtual	unsigned int	Release(void) = 0;

		// return field data by field index
		virtual IData& Field(unsigned int idx) = 0;
		virtual IData& Field(unsigned int idx) const = 0;

		// return field data by field name
		virtual IData& Field(const char* pszName) = 0;
		virtual IData& Field(const char* pszName) const = 0;

		// return the key data of this record
		virtual IData& Key(void) = 0;

		// 通过 UPDATE 语句来更新 修改过 的数据到数据库，
		// 缺省使用同步方式，调用者要等待数据库更新完成后返回。
		// 也可指定异步方式，调用者不需要等待数据库更新完成，直接返回true
		virtual bool			Update(bool bSync = true) = 0;
		virtual bool			Delete(bool bArchive = false) = 0;
		virtual bool			Insert(void) = 0;

		// clear the edit flag of all field in this record, so update operation will ignored.
		virtual void			ClsEditFlag(void) = 0;

		// 新扩展的接口 用来获取字段的详细信息
		virtual const char* getValue(unsigned int unIndex) = 0;
		virtual unsigned int	getType(unsigned int unIndex) = 0;
		virtual unsigned int	getAttr(unsigned int unIndex) = 0;
		virtual unsigned int	getFiledSize() = 0;
	};

	//////////////////////////////////////////////////////////////////////
	class IRecordset
	{
	public:
		IRecordset() {}
		virtual ~IRecordset() {}
		virtual	unsigned int	Release(void) = 0;

		// get record from record set 
		virtual IRecord* GetRecord(void) const = 0;

		// count record in set.	 return -1: error
		virtual unsigned int	CountRecord(void) const = 0;

		// move to specified record
		virtual void			Move(unsigned int index) = 0;

		// update this record by "update, delete or insert" sql
		// bFlag = Sync or Archive
		virtual bool			Update(bool bFlag = true) = 0;

		// clear the edit flag of all field in this record, so update operation will ignored.
		virtual void			ClsEditFlag(void) = 0;

		// make a new default record, it doesn`t exist in dbtool
		virtual IRecord* MakeDefRecord(void) = 0;

		virtual int				FieldsCount(void) const = 0;
	};


	//Result
	typedef struct sql_result
	{
		void   			(*callback)(sql_result*);
		void* pPlayer;
		bool   			Falg;
		IRecordset* pResult;
		void            Release()
		{
			if (NULL != pResult)
				pResult->Release();

			pResult = NULL;
			delete this;
		};

		sql_result& operator = (sql_result* Result)
		{
			if (this == Result)
			{
				return *this;
			}

			this->callback = Result->callback;
			this->pPlayer = Result->pPlayer;
			this->Falg = Result->Falg;
			this->pResult = Result->pResult;

			return *this;
		}
	}*PSQL_RESULT, SQL_RESULT;

	typedef void (*SQL_RESULT_CALLBACK)(PSQL_RESULT);

	//////////////////////////////////////////////////////////////////////
	enum MODE { MODE_NONE = 0, MODE_READ, MODE_EDIT };
	class IDatabase
	{
	public:
		virtual					~IDatabase() {}
		virtual	unsigned int	Release(void) = 0;

		// create a record set by specified "select" sql
		virtual IRecordset* CreateRecordset(const char* szSQL, MODE eMode) = 0;

		//Execute SP without return value
		virtual bool            Execute(const char* szSQL, MODE eMode) = 0;

		// make a new default record by "insert" sql.
		virtual IRecord* MakeDefaultRecord(const char* szTable, unsigned int id = 0) = 0;

		//get status of dbtool 
		virtual size_t          GetRequestSize() = 0;
		virtual size_t          GetResultSize() = 0;

		// async call
		virtual bool            ExecuteAsyncSQL(const char* pszSQL, void* pUser, SQL_RESULT_CALLBACK callback) = 0;
		virtual bool            CallBackAsyncSQL() = 0;
	};

	//////////////////////////////////////////////////////////////////////
	// create IDatabase interface
	//////////////////////////////////////////////////////////////////////
	IDatabase* DatabaseCreate(const char* szDBServer, const char* szLoginName, const char* szPassword, const char* szDBName, const char* szCharSet = NULL, unsigned int uiPort = MYSQL_PORT, bool bEnableSQLChk = true);
	void		Destroy(dbtool::IDatabase* pDB);
}

