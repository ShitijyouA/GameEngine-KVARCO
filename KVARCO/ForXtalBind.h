#pragma once

//Xtal�o�C���h�p�}�N��
#define USE_XDEFZ(class_)	typedef class_ Self
//���Ƃ�Xtal�W����Xdef_fun,Xdef_method_Xdef_var���g�p

//Xtal�Ƀo�C���h����class
//Release()�͊e���ŗp�ӂ���
//friend�͎g�p����Xtal�ɂ���ĈႤ�̂Œ���
//�R���p�C���G���[���N�����ꍇ�͂���friend�錾��S�č폜���A
//�R���p�C���G���[���Q�l�ɏ����Ȃ���
#define FRIENDS_FOR_XTAL(class_)\
	friend xtal::SmartPtr<class_> xtal::xnew();\
	friend class xtal::SmartPtr<class_>;\
	friend struct xtal::XNew<class_>;\
	friend struct xtal::XXNew<class_,3>;

#define SINGLETON_PATTERNX(class_,class_ptr_)\
private:\
	FRIENDS_FOR_XTAL(class_)\
	class_() { ;}\
public:\
	static class_ptr_ GetInst();

//Xtal��C++���ł��邾���������邽�߂̃o�C���h�p�}�N��

#define NAME_IN_X(name_)	name_##X

#ifndef Xdef_funx
#define Xdef_funx(name_)	xtal::lib()->def(Xid(name_),xtal::fun(&NAME_IN_X(name_)))
#endif

#ifndef Xdef_methodx
#define Xdef_methodx(name_) it->def(Xid(name_),xtal::method(&Self::NAME_IN_X(name_)))
#endif

#ifndef Xdef_varx
#define Xdef_varx(name_)	it->def_var(Xid(name_),&Self::NAME_IN_X(name_))
#endif

#define CLASS_IN_X(class_name_) class NAME_IN_X(class_name_) : public class_name_