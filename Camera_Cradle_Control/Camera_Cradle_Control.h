
// Camera_Cradle_Control.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCamera_Cradle_ControlApp:
// �йش����ʵ�֣������ Camera_Cradle_Control.cpp
//

class CCamera_Cradle_ControlApp : public CWinAppEx
{
public:
	CCamera_Cradle_ControlApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCamera_Cradle_ControlApp theApp;