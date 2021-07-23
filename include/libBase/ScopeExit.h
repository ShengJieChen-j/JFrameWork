/****************************************************************
	�������ڣ�  2021��7��24��
	�ļ����ƣ�  ScopeExit.h
	˵    ����  �˳��������Զ�ִ����Ӧ����

	��ǰ�汾��  1.00
	��    �ߣ�  shengjiechen
	��    ����

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
