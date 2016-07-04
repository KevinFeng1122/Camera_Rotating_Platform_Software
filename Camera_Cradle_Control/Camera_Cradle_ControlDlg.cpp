
// Camera_Cradle_ControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Camera_Cradle_Control.h"
#include "Camera_Cradle_ControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCamera_Cradle_ControlDlg 对话框




CCamera_Cradle_ControlDlg::CCamera_Cradle_ControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCamera_Cradle_ControlDlg::IDD, pParent)
	, Allow_Serial_Port_Transmit(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCamera_Cradle_ControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_MsComm);
}

BEGIN_MESSAGE_MAP(CCamera_Cradle_ControlDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_CBN_DROPDOWN(IDC_COMBO_Port_List, &CCamera_Cradle_ControlDlg::OnCbnDropdownComboPortList)
	ON_BN_CLICKED(IDC_BUTTON_Port_Connect, &CCamera_Cradle_ControlDlg::OnBnClickedButtonPortConnect)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Motor_0_Position, &CCamera_Cradle_ControlDlg::OnNMCustomdrawSliderMotor0Position)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Motor_1_Position, &CCamera_Cradle_ControlDlg::OnNMCustomdrawSliderMotor1Position)
	ON_BN_CLICKED(IDC_BUTTON_Apply_Init, &CCamera_Cradle_ControlDlg::OnBnClickedButtonApplyInit)
	ON_BN_CLICKED(IDC_RADIO_Motor_Drive, &CCamera_Cradle_ControlDlg::OnBnClickedRadioMotorDrive)
	ON_BN_CLICKED(IDC_RADIO_Angle_Drive, &CCamera_Cradle_ControlDlg::OnBnClickedRadioAngleDrive)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_Motor_0_Position, &CCamera_Cradle_ControlDlg::OnNMReleasedcaptureSliderMotor0Position)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_Motor_1_Position, &CCamera_Cradle_ControlDlg::OnNMReleasedcaptureSliderMotor1Position)
	ON_BN_CLICKED(IDC_BUTTON_Slider_0_Value_Implement, &CCamera_Cradle_ControlDlg::OnBnClickedButtonSlider0ValueImplement)
	ON_BN_CLICKED(IDC_BUTTON_Slider_1_Value_Implement, &CCamera_Cradle_ControlDlg::OnBnClickedButtonSlider1ValueImplement)
END_MESSAGE_MAP()


// CCamera_Cradle_ControlDlg 消息处理程序

BOOL CCamera_Cradle_ControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	
	Allow_Serial_Port_Transmit=FALSE;//串口未打开之前不允许通讯
	Motor_0_Initial_Position=0;
	Motor_1_Initial_Position=0;
	
	GetDlgItem(IDC_SLIDER_Motor_0_Position)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_Motor_1_Position)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Apply_Init)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_Motor_Drive)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_Angle_Drive)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Slider_0_Value_Implement)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Slider_1_Value_Implement)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->SetCheck(TRUE);

	GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCamera_Cradle_ControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCamera_Cradle_ControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//[函数]
void CCamera_Cradle_ControlDlg::OnCbnDropdownComboPortList()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox *pointer_port = (CComboBox *)GetDlgItem(IDC_COMBO_Port_List);
	pointer_port->ResetContent();//清除组合框中已有项
	FindSerialPort(pointer_port);
}

// [函数]从注册表内找出存在的串口号并写入一个地址中
void CCamera_Cradle_ControlDlg::FindSerialPort(CComboBox *pComboBox)
{
	HKEY hKey;
#ifdef _DEBUG
	ASSERT( pComboBox != NULL );
	pComboBox->AssertValid();
#endif
	if( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		"Hardware\\DeviceMap\\SerialComm",
		NULL,
		KEY_READ,
		&hKey) == ERROR_SUCCESS) // 打开串口注册表
	{
		int i=0;
		char portName[256],commName[256];
		DWORD dwLong,dwSize;
		while(1)
		{
			dwLong = dwSize = sizeof(portName);
			if( ::RegEnumValue( hKey,
				i,
				portName,
				&dwLong,
				NULL,
				NULL,
				(PUCHAR)commName,
				&dwSize ) == ERROR_NO_MORE_ITEMS ) // 枚举串口
				break;
			pComboBox->AddString( commName ); // commName就是串口名字
			//AfxMessageBox(commName);
			i++;
		}

		if( pComboBox->GetCount() == 0 )
		{
			::AfxMessageBox("在HKEY_LOCAL_MACHINE:Hardware\\DeviceMap\\SerialComm里找不到串口!!!" );
		}
		RegCloseKey(hKey);
	}
}



// [函数]串口的连接与断开
void CCamera_Cradle_ControlDlg::OnBnClickedButtonPortConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	//判断串口是否打开
	if(m_MsComm.get_PortOpen())
	{	//关闭串口
		m_MsComm.put_PortOpen(FALSE);
		//再次判断串口是否关闭
		if(!m_MsComm.get_PortOpen())
		{
			GetDlgItem(IDC_STATIC_Port_State)->SetWindowText("未连接");
			GetDlgItem(IDC_BUTTON_Port_Connect)->SetWindowText("连接");
			Allow_Serial_Port_Transmit=FALSE;
			GetDlgItem(IDC_SLIDER_Motor_0_Position)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLIDER_Motor_1_Position)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_Motor_Drive)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_Angle_Drive)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_Apply_Init)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_Slider_0_Value_Implement)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_Slider_1_Value_Implement)->EnableWindow(FALSE);
		}
	}
	else
	{
		//先判断是否选择了串口号
		CComboBox *pointer_port = (CComboBox *)GetDlgItem(IDC_COMBO_Port_List);
		char port_count=pointer_port->GetCount();
		if((port_count==0) | (pointer_port->GetCurSel()==CB_ERR))
		{
			AfxMessageBox("尚未选择串口号，无法进行连接");
			return;
		}

		//获取要连接的串口号，用char变量port_num表示
		char port_num;
		CString port_name;
		GetDlgItemText(IDC_COMBO_Port_List,port_name);
		port_name.Replace("COM", "");
		port_num = atoi(port_name.GetBuffer(0));
		port_name.ReleaseBuffer();
		//设置串口属性
		m_MsComm.put_CommPort(port_num); //选择COM1端口
		m_MsComm.put_InputMode(1); //输入方式为二进制方式
		m_MsComm.put_InBufferSize(1024); //设置输入缓冲区
		m_MsComm.put_OutBufferSize(512); //设置输出缓冲区
		m_MsComm.put_Settings(TEXT("19200,n,8,1"));//波特率，无校验，个数据位，个停止位
		//打开串口
		m_MsComm.put_PortOpen(TRUE);
		m_MsComm.put_RThreshold(1); //每当接收缓冲区有1个字符则接收串口数据
		m_MsComm.put_InputLen(0); //设置当前缓冲区长度为
		m_MsComm.get_Input(); //预读缓冲区以清除残留数据
		//再次判断串口是否打开，

		//再次判断串口是否打开
		if(m_MsComm.get_PortOpen())
		{
			GetDlgItem(IDC_STATIC_Port_State)->SetWindowText("已连接");
			GetDlgItem(IDC_BUTTON_Port_Connect)->SetWindowText("断开");
			Allow_Serial_Port_Transmit=TRUE;
			Speed_Modify();//设置舵机速度
			GetDlgItem(IDC_SLIDER_Motor_0_Position)->EnableWindow(TRUE);
			GetDlgItem(IDC_SLIDER_Motor_1_Position)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_Motor_Drive)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_Angle_Drive)->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_RADIO_Angle_Drive))->SetCheck(FALSE);
			GetDlgItem(IDC_BUTTON_Apply_Init)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_Slider_0_Value_Implement)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_Slider_1_Value_Implement)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_HIDE);
			Motor_Init();//初始化舵机位置
		}
	}
}

// 软件打开时舵机初始化
void CCamera_Cradle_ControlDlg::Motor_Init(void)
{
	//从ini配置文件中读取相机初始位置对应的舵机位置值
	GetModuleFileName(NULL,APP_Path,100);
	INI_Path=APP_Path;
	int pos = INI_Path.ReverseFind('\\'); 
	INI_Path = INI_Path.Left(pos)+"\\Config.ini"; 
	Motor_0_Initial_Position=GetPrivateProfileInt("Motor Initial Position","Motor0",512,INI_Path);
	Motor_1_Initial_Position=GetPrivateProfileInt("Motor Initial Position","Motor1",512,INI_Path);
	//计算自转角和俯仰角的范围，并在控件中显示
	Angle_Range_Calculate_And_Show();
	
	//Slider控件
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	pointer_Slider_Motor_0->SetRange(0,4095);//对应舵机位置参数的范围
	pointer_Slider_Motor_0->SetPos(Motor_0_Initial_Position);
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	pointer_Slider_Motor_1->SetRange(0,4095);//对应舵机位置参数的范围
	pointer_Slider_Motor_1->SetPos(Motor_1_Initial_Position);
	
	//控制舵机
	Motor_Run(0x00,Motor_0_Initial_Position);
	Sleep(100);
	Motor_Run(0x01,Motor_1_Initial_Position);
	Sleep(100);
	Motor_Run(0x00,Motor_0_Initial_Position);
	Sleep(100);
	Motor_Run(0x01,Motor_1_Initial_Position);
	
	//Edit控件
	Show_Data();
	SetDlgItemInt(IDC_EDIT_Motor_0_Init,Motor_0_Initial_Position);
	SetDlgItemInt(IDC_EDIT_Motor_1_Init,Motor_1_Initial_Position);

	//STATIC指示改变
	CString Indication_Content_0="#0目标";
	CString Indication_Content_1="#1目标";
	GetDlgItem(IDC_STATIC_Indication_0)->SetWindowText(Indication_Content_0);
	GetDlgItem(IDC_STATIC_Indication_1)->SetWindowText(Indication_Content_1);
}

// 利用MsComm控件通过串口发送指令
void CCamera_Cradle_ControlDlg::Serial_Port_Send_Data(UCHAR* char_array_in, int element_num)
{
	if (!Allow_Serial_Port_Transmit)//串口未打开时不可发送数据，Allow_Serial_Port_Transmit是类的成员变量
	{
		return;
	}
	TRY 
	{
		CByteArray bytMiddleArr;
		bytMiddleArr.SetSize(element_num);
		bytMiddleArr.RemoveAll();
		for (UCHAR i=0;i<element_num;i++)
		{
			char num_char=char_array_in[i];
			bytMiddleArr.Add( num_char);
		}

		COleVariant varOut;
		varOut=COleVariant(bytMiddleArr); //将数据转换为变体数据类型
		m_MsComm.put_Output(varOut); //发送数据
	}
	CATCH (CMemoryException, e)
	{
		TCHAR   szCause[255];
		CString strFormatted;

		e->GetErrorMessage(szCause, 255);

		strFormatted = _T("Excpetion: ");
		strFormatted += szCause;

		AfxMessageBox(strFormatted);

	}
	END_CATCH
}



//拖动Slider控件
void CCamera_Cradle_ControlDlg::OnNMCustomdrawSliderMotor0Position(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//更新slider上方static控件值:
	Show_STATIC_Slider_Value();

	*pResult = 0;
}

void CCamera_Cradle_ControlDlg::OnNMCustomdrawSliderMotor1Position(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//更新slider上方static控件值:
	Show_STATIC_Slider_Value();

	*pResult = 0;
}

// 发送指令控制舵机转动
void CCamera_Cradle_ControlDlg::Motor_Run(UCHAR ID, int Goal_Position)
{
	//生成舵机转动指令
	UCHAR Goal_Position_High,Goal_Position_Low;
	UCHAR Motor_Run_Instruction[9]={0xff, 0xff, 0x00, 0x05, 0x03, 0x1e, 0x00, 0x00,0x00};
	Goal_Position_High=Goal_Position / 256; //取整
	Goal_Position_Low=Goal_Position % 256; //取余
	Motor_Run_Instruction[2]=ID;
	Motor_Run_Instruction[6]=Goal_Position_Low;
	Motor_Run_Instruction[7]=Goal_Position_High;
	Motor_Run_Instruction[8]=Check_Sum(Motor_Run_Instruction,7);

	//串口发送指令
	Serial_Port_Send_Data(Motor_Run_Instruction,9);

}

// 计算舵机指令CHECK_SUM参数
UCHAR CCamera_Cradle_ControlDlg::Check_Sum(UCHAR* Command_Array, UCHAR Last_Index)
{
	UCHAR i;
	UCHAR SUM=0;
	for (i=2;i<=Last_Index;i++)//从index=2的元素开始，对应于ID值
	{
		SUM=SUM+Command_Array[i];		
	}
	SUM=~SUM;
	return SUM;
}
// //根据slider控件值，刷新Edit控件值
void CCamera_Cradle_ControlDlg::Show_Data(void)
{
		int Slider_0_Position,Slider_1_Position;
		CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
		CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
		Slider_0_Position=pointer_Slider_Motor_0->GetPos();
		Slider_1_Position=pointer_Slider_Motor_1->GetPos();
		if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
		{		
			//舵机位置EDIT显示
			int Goal_Position_0,Goal_Position_1;
			Goal_Position_0=Slider_0_Position;
			Goal_Position_1=Slider_1_Position;
			SetDlgItemInt(IDC_EDIT_Motor_0_Position,Goal_Position_0);
			SetDlgItemInt(IDC_EDIT_Motor_1_Position,Goal_Position_1);

			//转角计算及其EDIT控件显示
			double Self_Rotate_Angle,Pitch_Angle;
			Self_Rotate_Angle=((Goal_Position_0-Motor_0_Initial_Position)+(Goal_Position_1-Motor_1_Initial_Position))/2/11.375;
			Pitch_Angle=((Goal_Position_0-Motor_0_Initial_Position)-(Goal_Position_1-Motor_1_Initial_Position))/2/11.375;
			CString Self_Rotate_Angle_string,Pitch_Angle_string;
			Self_Rotate_Angle_string.Format("%.1f",Self_Rotate_Angle);
			Pitch_Angle_string.Format("%.1f",Pitch_Angle);
			SetDlgItemText(IDC_EDIT_Self_Rotate_Angle,Self_Rotate_Angle_string);
			SetDlgItemText(IDC_EDIT_Pitch_Angle,Pitch_Angle_string);
		} 
		else
		{
			//自转角度计算及其EDIT控件显示
			double Self_Rotate_Angle_Value=(double)Slider_0_Position*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
			CString Self_Rotate_Angle_string;
			Self_Rotate_Angle_string.Format("%.1f",Self_Rotate_Angle_Value);
			SetDlgItemText(IDC_EDIT_Self_Rotate_Angle,Self_Rotate_Angle_string);
			
			//俯仰角度计算及其EDIT控件显示
			double Pitch_Angle_Value=(double)Slider_1_Position*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;
			CString Pitch_Angle_string;
			Pitch_Angle_string.Format("%.1f",Pitch_Angle_Value);
			SetDlgItemText(IDC_EDIT_Pitch_Angle,Pitch_Angle_string);

			//舵机位置计算及其EDIT控件显示
			int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
			int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;
			SetDlgItemInt(IDC_EDIT_Motor_0_Position,Motor0_Pos);
			SetDlgItemInt(IDC_EDIT_Motor_1_Position,Motor1_Pos);

		}
}


//应用新的舵机初始位置值
void CCamera_Cradle_ControlDlg::OnBnClickedButtonApplyInit()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//更新INI文件
	char Motor_0_Init_Value[10],Motor_1_Init_Value[10];
	GetDlgItemText(IDC_EDIT_Motor_0_Init,Motor_0_Init_Value,10);
	GetDlgItemText(IDC_EDIT_Motor_1_Init,Motor_1_Init_Value,10);
	WritePrivateProfileString("Motor Initial Position","Motor0",Motor_0_Init_Value,INI_Path);
	WritePrivateProfileString("Motor Initial Position","Motor1",Motor_1_Init_Value,INI_Path);

	//更新全局变量
	Motor_0_Initial_Position=GetDlgItemInt(IDC_EDIT_Motor_0_Init);
	Motor_1_Initial_Position=GetDlgItemInt(IDC_EDIT_Motor_1_Init);

	//计算自转角和俯仰角的范围，并在控件中显示
	Angle_Range_Calculate_And_Show();

	//根据slider控件值更新EDIT显示
	Show_Data();

	//更新slider上方static控件值:
	Show_STATIC_Slider_Value();
}

//驱动模式选择
void CCamera_Cradle_ControlDlg::OnBnClickedRadioMotorDrive()
{
	// TODO: 在此添加控件通知处理程序代码
	//Slider控件恢复成初始位置
	Motor_Init();
	//STATIC指示改变
	CString Indication_Content_0="#0目标";
	CString Indication_Content_1="#1目标";
	GetDlgItem(IDC_STATIC_Indication_0)->SetWindowText(Indication_Content_0);
	GetDlgItem(IDC_STATIC_Indication_1)->SetWindowText(Indication_Content_1);
}

void CCamera_Cradle_ControlDlg::OnBnClickedRadioAngleDrive()
{
	// TODO: 在此添加控件通知处理程序代码

	//STATIC指示改变
	CString Indication_Content_0="自转目标";
	CString Indication_Content_1="俯仰目标";
	GetDlgItem(IDC_STATIC_Indication_0)->SetWindowText(Indication_Content_0);
	GetDlgItem(IDC_STATIC_Indication_1)->SetWindowText(Indication_Content_1);

	//Slider控件恢复成初始位置
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	int Slider_Pos_Self_Rotate=(-Sefl_Rotate_Min*4095)/(Sefl_Rotate_Max-Sefl_Rotate_Min);
	pointer_Slider_Motor_0->SetPos(Slider_Pos_Self_Rotate);
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	int Slider_Pos_Pitch=(-Pitch_Min*4095)/(Pitch_Max-Pitch_Min);
	pointer_Slider_Motor_1->SetPos(Slider_Pos_Pitch);
	
	//控制舵机
	Motor_Run(0x00,Motor_0_Initial_Position);
	Sleep(100);
	Motor_Run(0x01,Motor_1_Initial_Position);
	Sleep(100);
	Motor_Run(0x00,Motor_0_Initial_Position);
	Sleep(100);
	Motor_Run(0x01,Motor_1_Initial_Position);

	//EDIT控件更新
	Show_Data();
}

//计算自转角和俯仰角的范围，并在控件中显示
void CCamera_Cradle_ControlDlg::Angle_Range_Calculate_And_Show(void)
{
	Sefl_Rotate_Max=(8190-Motor_0_Initial_Position-Motor_1_Initial_Position)/2/11.375;
	Sefl_Rotate_Min=(0-Motor_0_Initial_Position-Motor_1_Initial_Position)/2/11.375;
	Pitch_Max=(4095-Motor_0_Initial_Position+Motor_1_Initial_Position)/2/11.375;
	Pitch_Min=(0-Motor_0_Initial_Position-4095+Motor_1_Initial_Position)/2/11.375;
	CString Self_Rotate_Max_string;
	Self_Rotate_Max_string.Format("%.1f",Sefl_Rotate_Max);
	CString Self_Rotate_Min_string;
	Self_Rotate_Min_string.Format("%.1f",Sefl_Rotate_Min);
	CString Pitch_Max_string;
	Pitch_Max_string.Format("%.1f",Pitch_Max);
	CString Pitch_Min_string;
	Pitch_Min_string.Format("%.1f",Pitch_Min);
	CString Self_Rotae_Range_string="(";
	Self_Rotae_Range_string=Self_Rotae_Range_string+Self_Rotate_Min_string;
	Self_Rotae_Range_string=Self_Rotae_Range_string+"~";
	Self_Rotae_Range_string=Self_Rotae_Range_string+Self_Rotate_Max_string;
	Self_Rotae_Range_string=Self_Rotae_Range_string+")";
	CString Pitch_Range_string="(";
	Pitch_Range_string=Pitch_Range_string+Pitch_Min_string;
	Pitch_Range_string=Pitch_Range_string+"~";
	Pitch_Range_string=Pitch_Range_string+Pitch_Max_string;
	Pitch_Range_string=Pitch_Range_string+")";
	GetDlgItem(IDC_STATIC_Self_Rotate_Range)->SetWindowText(Self_Rotae_Range_string);
	GetDlgItem(IDC_STATIC_Pitch_Range)->SetWindowText(Pitch_Range_string);
}

//更改舵机移动速度
void CCamera_Cradle_ControlDlg::Speed_Modify(void)
{
	//生成舵机转动指令
	WORD Moving_Speed=20;
	UCHAR Speed_High,Speed_Low;
	UCHAR Moving_Speed_Instruction[9]={0xff, 0xff, 0x00, 0x05, 0x03, 0x20, 0x00, 0x00,0x00};
	Speed_High=Moving_Speed / 256; //取整
	Speed_Low=Moving_Speed % 256; //取余
	Moving_Speed_Instruction[6]=Speed_Low;
	Moving_Speed_Instruction[7]=Speed_High;

	Moving_Speed_Instruction[2]=0x00;//ID
	Moving_Speed_Instruction[8]=Check_Sum(Moving_Speed_Instruction,7);
	Serial_Port_Send_Data(Moving_Speed_Instruction,9);//串口发送指令
	
	Sleep(100);

	Moving_Speed_Instruction[2]=0x01;//ID
	Moving_Speed_Instruction[8]=Check_Sum(Moving_Speed_Instruction,7);
	Serial_Port_Send_Data(Moving_Speed_Instruction,9);//串口发送指令
}

void CCamera_Cradle_ControlDlg::OnNMReleasedcaptureSliderMotor0Position(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//获取控件值
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	int Slider_Position=pointer_Slider_Motor_0->GetPos();
	int Goal_Position=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		Goal_Position=Slider_Position;
		//移动舵机
		Motor_Run(0x00,Goal_Position);	
	}
	else
	{
		//自转角度
		double Self_Rotate_Angle_Value=(double)Slider_Position*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
	
		//舵机位置换算及舵机EDIT控件显示
		CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
		double Pitch_Angle_Value=pointer_Slider_Motor_1->GetPos()*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;
		int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
		int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;
	
		//转动舵机
		if (Motor0_Pos>=0 & Motor1_Pos>=0 & Motor0_Pos<=4095 & Motor1_Pos<=4095)
		{
			GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_HIDE);
			Motor_Run(0x00,Motor0_Pos);
			Sleep(100);
			Motor_Run(0x01,Motor1_Pos);
		}
		else
		{
			GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_SHOW);
		}
	}

	//EDIT控件显示
	Show_Data();

	*pResult = 0;
}

void CCamera_Cradle_ControlDlg::OnNMReleasedcaptureSliderMotor1Position(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//获取控件值
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	int Slider_Position=pointer_Slider_Motor_1->GetPos();
	int Goal_Position=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		Goal_Position=Slider_Position;
		//移动舵机
		Motor_Run(0x01,Goal_Position);	
		
	}
	else
	{
		//俯仰角度
		double Pitch_Angle_Value=(double)Slider_Position*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;

		//舵机位置换算及舵机EDIT控件显示
		CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
		double Self_Rotate_Angle_Value=pointer_Slider_Motor_0->GetPos()*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
		int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
		int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;

		//转动舵机
		if (Motor0_Pos>=0 & Motor1_Pos>=0 & Motor0_Pos<=4095 & Motor1_Pos<=4095)
		{
			GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_HIDE);
			Motor_Run(0x00,Motor0_Pos);
			Sleep(100);
			Motor_Run(0x01,Motor1_Pos);
		} 
		else
		{
			GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_SHOW);
		}

	}

	//EDIT控件显示
	Show_Data();

	*pResult = 0;
}

void CCamera_Cradle_ControlDlg::OnBnClickedButtonSlider0ValueImplement()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取控件值
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	int Slider_Position=pointer_Slider_Motor_0->GetPos();
	int Goal_Position=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		Goal_Position=Slider_Position;
		//移动舵机
		Motor_Run(0x00,Goal_Position);	
	}
	else
	{
		//自转角度
		double Self_Rotate_Angle_Value=(double)Slider_Position*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;

		//舵机位置换算及舵机EDIT控件显示
		CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
		double Pitch_Angle_Value=pointer_Slider_Motor_1->GetPos()*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;
		int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
		int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;

		//转动舵机
		if (Motor0_Pos>=0 & Motor1_Pos>=0 & Motor0_Pos<=4095 & Motor1_Pos<=4095)
		{
			GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_HIDE);
			Motor_Run(0x00,Motor0_Pos);
			Sleep(100);
			Motor_Run(0x01,Motor1_Pos);
		}
		else
		{
			GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_SHOW);
		}
	}

	//EDIT控件显示
	Show_Data();

}

void CCamera_Cradle_ControlDlg::OnBnClickedButtonSlider1ValueImplement()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取控件值
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	int Slider_Position=pointer_Slider_Motor_1->GetPos();
	int Goal_Position=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		Goal_Position=Slider_Position;
		//移动舵机
		Motor_Run(0x01,Goal_Position);	

	}
	else
	{
		//俯仰角度
		double Pitch_Angle_Value=(double)Slider_Position*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;

		//舵机位置换算及舵机EDIT控件显示
		CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
		double Self_Rotate_Angle_Value=pointer_Slider_Motor_0->GetPos()*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
		int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
		int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;

		//转动舵机
		if (Motor0_Pos>=0 & Motor1_Pos>=0 & Motor0_Pos<=4095 & Motor1_Pos<=4095)
		{
			GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_HIDE);
			Motor_Run(0x00,Motor0_Pos);
			Sleep(100);
			Motor_Run(0x01,Motor1_Pos);
		} 
		else
		{
			GetDlgItem(IDC_STATIC_Error)->ShowWindow(SW_SHOW);
		}

	}

	//EDIT控件显示
	Show_Data();

}

// 更新显示指示slider值的STATIC控件
void CCamera_Cradle_ControlDlg::Show_STATIC_Slider_Value(void)
{
	//获取控件值
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	int Slider_Position_0=pointer_Slider_Motor_0->GetPos();
	//STATIC控件显示
	CString Motor0_string;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		//舵机位置
		Motor0_string.Format("%d",Slider_Position_0);
		Motor0_string="【"+Motor0_string+"】";
		GetDlgItem(IDC_STATIC_Slider_0)->SetWindowText(Motor0_string);
	}
	else
	{
		//自转角度
		double Self_Rotate_Angle_Value=(double)Slider_Position_0*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
		CString Self_Rotate_Angle_string;
		Self_Rotate_Angle_string.Format("%.1f",Self_Rotate_Angle_Value);
		Self_Rotate_Angle_string="【"+Self_Rotate_Angle_string+"】";
		GetDlgItem(IDC_STATIC_Slider_0)->SetWindowText(Self_Rotate_Angle_string);
	}

	//获取控件值
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	int Slider_Position_1=pointer_Slider_Motor_1->GetPos();
	//STATIC控件显示
	CString Motor1_string;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		//舵机位置
		Motor1_string.Format("%d",Slider_Position_1);
		Motor1_string="【"+Motor1_string+"】";
		GetDlgItem(IDC_STATIC_Slider_1)->SetWindowText(Motor1_string);
	}
	else
	{
		//自转角度
		double Pitch_Angle_Value=(double)Slider_Position_1*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;
		CString Pitch_Angle_string;
		Pitch_Angle_string.Format("%.1f",Pitch_Angle_Value);
		Pitch_Angle_string="【"+Pitch_Angle_string+"】";
		GetDlgItem(IDC_STATIC_Slider_1)->SetWindowText(Pitch_Angle_string);
	}

}
