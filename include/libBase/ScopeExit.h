/****************************************************************
	创建日期：  2021年7月24日
	文件名称：  ScopeExit.h
	说    明：  退出作用域自动执行相应操作

	当前版本：  1.00
	作    者：  shengjiechen
	概    述：

******************************************************************/
#pragma once

template <typename F>
struct ScopeExit {
	ScopeExit(F f) : f(f) {}
	~ScopeExit() { f(); }
	F f;
};

template <typename F>
ScopeExit<F> MakeScopeExit(F f) {
	return ScopeExit<F>(f);
};

#define STRING_JOIN2(arg1, arg2) DO_STRING_JOIN2(arg1, arg2)
#define DO_STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define SCOPE_EXIT(code) \
	auto STRING_JOIN2(_scope_exit_, __LINE__) = MakeScopeExit([=](){code;})
#define SCOPE_EXIT_REF(code) \
	auto STRING_JOIN2(_scope_exit_, __LINE__) = MakeScopeExit([&](){code;})
