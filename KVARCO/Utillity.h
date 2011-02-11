#pragma once

//Set%_Name()�֐������B�\���͕̂s�B���ԂȂ�
#ifndef SETTER_
#define SETTER_(_Type,_Name) void Set##_Name(const _Type x);
#endif
//Get%_Name()�֐������B�\���͕̂s�B���ԂȂ�
#ifndef GETTER_
#define GETTER_(_Type,_Name) _Type Get##_Name() const;
#endif

//SETTER��GETTER�̂܂Ƃ߁B���ԂȂ�
#ifndef SET_GET_
#define SET_GET_(_Type,_Name)\
	SETTER_(_Type,_Name);\
	GETTER_(_Type,_Name);
#endif

//Set%_Name()�֐������B�\���͕̂s��
#ifndef SETTER
#define SETTER(_Type,_Name) void Set##_Name(const _Type x) { _Name=x; }
#endif
//Get%_Name()�֐������B�\���͕̂s��
#ifndef GETTER
#define GETTER(_Type,_Name) _Type Get##_Name() const { return _Name; }
#endif

//SETTER��GETTER�̂܂Ƃ�
#ifndef SET_GET
#define SET_GET(_Type,_Name)\
	SETTER(_Type,_Name)\
	GETTER(_Type,_Name)
#endif

//Xtal�o�C���h�p�}�N��
#ifndef BIND_XTAL_FUN_DEFNAME
#define BIND_XTAL_FUN_DEFNAME(x) xtal::lib()->def(Xid(x),xtal::fun(&x) )
#endif

#ifndef BIND_XTAL_CLASSFUN_DEFNAME_IT
#define BIND_XTAL_CLASSFUN_DEFNAME_IT(_class,_fun) it->def(Xid(_fun),xtal::method(&_class::_fun))
#endif

#ifndef XTAL_BIND_VAR_DEFNAME
#define XTAL_BIND_VAR_DEFNAME(_class,n) it->def_var(Xid(n),&_class::n)
#endif

//singleton pattern
//Xtal�Ƀo�C���h���Ȃ�class
#define SINGLETON_PATTERN(_class,_class_ptr)\
private:\
	_class() { ;}\
public:\
	static _class_ptr GetInst();\
	void Release()	{ }

//Xtal�Ƀo�C���h����class
//Release()�͊e���ŗp�ӂ���
//friend�͎g�p����Xtal�ɂ���ĈႤ�̂Œ���
//�R���p�C���G���[���N�����ꍇ�͂���friend�錾��S�č폜���A
//�R���p�C���G���[���Q�l�ɏ����Ȃ����Ă�������
#define SINGLETON_PATTERNX(_class,_class_ptr)\
private:\
	friend xtal::SmartPtr<_class> xtal::xnew();\
	friend class xtal::SmartPtr<_class>;\
	friend struct xtal::XNew<_class>;\
	friend struct xtal::XXNew<_class,3>;\
	_class() { ;}\
public:\
	static _class_ptr GetInst();

//tRECT,tPOINT���C���X�^���X���������̂�SmartPtr