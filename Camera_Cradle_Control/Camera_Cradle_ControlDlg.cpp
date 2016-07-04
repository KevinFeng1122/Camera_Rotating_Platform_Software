
// Camera_Cradle_ControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Camera_Cradle_Control.h"
#include "Camera_Cradle_ControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCamera_Cradle_ControlDlg �Ի���




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


// CCamera_Cradle_ControlDlg ��Ϣ�������

BOOL CCamera_Cradle_ControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	Allow_Serial_Port_Transmit=FALSE;//����δ��֮ǰ������ͨѶ
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCamera_Cradle_ControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCamera_Cradle_ControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//[����]
void CCamera_Cradle_ControlDlg::OnCbnDropdownComboPortList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox *pointer_port = (CComboBox *)GetDlgItem(IDC_COMBO_Port_List);
	pointer_port->ResetContent();//�����Ͽ���������
	FindSerialPort(pointer_port);
}

// [����]��ע������ҳ����ڵĴ��ںŲ�д��һ����ַ��
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
		&hKey) == ERROR_SUCCESS) // �򿪴���ע���
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
				&dwSize ) == ERROR_NO_MORE_ITEMS ) // ö�ٴ���
				break;
			pComboBox->AddString( commName ); // commName���Ǵ�������
			//AfxMessageBox(commName);
			i++;
		}

		if( pComboBox->GetCount() == 0 )
		{
			::AfxMessageBox("��HKEY_LOCAL_MACHINE:Hardware\\DeviceMap\\SerialComm���Ҳ�������!!!" );
		}
		RegCloseKey(hKey);
	}
}



// [����]���ڵ�������Ͽ�
void CCamera_Cradle_ControlDlg::OnBnClickedButtonPortConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�жϴ����Ƿ��
	if(m_MsComm.get_PortOpen())
	{	//�رմ���
		m_MsComm.put_PortOpen(FALSE);
		//�ٴ��жϴ����Ƿ�ر�
		if(!m_MsComm.get_PortOpen())
		{
			GetDlgItem(IDC_STATIC_Port_State)->SetWindowText("δ����");
			GetDlgItem(IDC_BUTTON_Port_Connect)->SetWindowText("����");
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
		//���ж��Ƿ�ѡ���˴��ں�
		CComboBox *pointer_port = (CComboBox *)GetDlgItem(IDC_COMBO_Port_List);
		char port_count=pointer_port->GetCount();
		if((port_count==0) | (pointer_port->GetCurSel()==CB_ERR))
		{
			AfxMessageBox("��δѡ�񴮿ںţ��޷���������");
			return;
		}

		//��ȡҪ���ӵĴ��ںţ���char����port_num��ʾ
		char port_num;
		CString port_name;
		GetDlgItemText(IDC_COMBO_Port_List,port_name);
		port_name.Replace("COM", "");
		port_num = atoi(port_name.GetBuffer(0));
		port_name.ReleaseBuffer();
		//���ô�������
		m_MsComm.put_CommPort(port_num); //ѡ��COM1�˿�
		m_MsComm.put_InputMode(1); //���뷽ʽΪ�����Ʒ�ʽ
		m_MsComm.put_InBufferSize(1024); //�������뻺����
		m_MsComm.put_OutBufferSize(512); //�������������
		m_MsComm.put_Settings(TEXT("19200,n,8,1"));//�����ʣ���У�飬������λ����ֹͣλ
		//�򿪴���
		m_MsComm.put_PortOpen(TRUE);
		m_MsComm.put_RThreshold(1); //ÿ�����ջ�������1���ַ�����մ�������
		m_MsComm.put_InputLen(0); //���õ�ǰ����������Ϊ
		m_MsComm.get_Input(); //Ԥ���������������������
		//�ٴ��жϴ����Ƿ�򿪣�

		//�ٴ��жϴ����Ƿ��
		if(m_MsComm.get_PortOpen())
		{
			GetDlgItem(IDC_STATIC_Port_State)->SetWindowText("������");
			GetDlgItem(IDC_BUTTON_Port_Connect)->SetWindowText("�Ͽ�");
			Allow_Serial_Port_Transmit=TRUE;
			Speed_Modify();//���ö���ٶ�
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
			Motor_Init();//��ʼ�����λ��
		}
	}
}

// �����ʱ�����ʼ��
void CCamera_Cradle_ControlDlg::Motor_Init(void)
{
	//��ini�����ļ��ж�ȡ�����ʼλ�ö�Ӧ�Ķ��λ��ֵ
	GetModuleFileName(NULL,APP_Path,100);
	INI_Path=APP_Path;
	int pos = INI_Path.ReverseFind('\\'); 
	INI_Path = INI_Path.Left(pos)+"\\Config.ini"; 
	Motor_0_Initial_Position=GetPrivateProfileInt("Motor Initial Position","Motor0",512,INI_Path);
	Motor_1_Initial_Position=GetPrivateProfileInt("Motor Initial Position","Motor1",512,INI_Path);
	//������ת�Ǻ͸����ǵķ�Χ�����ڿؼ�����ʾ
	Angle_Range_Calculate_And_Show();
	
	//Slider�ؼ�
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	pointer_Slider_Motor_0->SetRange(0,4095);//��Ӧ���λ�ò����ķ�Χ
	pointer_Slider_Motor_0->SetPos(Motor_0_Initial_Position);
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	pointer_Slider_Motor_1->SetRange(0,4095);//��Ӧ���λ�ò����ķ�Χ
	pointer_Slider_Motor_1->SetPos(Motor_1_Initial_Position);
	
	//���ƶ��
	Motor_Run(0x00,Motor_0_Initial_Position);
	Sleep(100);
	Motor_Run(0x01,Motor_1_Initial_Position);
	Sleep(100);
	Motor_Run(0x00,Motor_0_Initial_Position);
	Sleep(100);
	Motor_Run(0x01,Motor_1_Initial_Position);
	
	//Edit�ؼ�
	Show_Data();
	SetDlgItemInt(IDC_EDIT_Motor_0_Init,Motor_0_Initial_Position);
	SetDlgItemInt(IDC_EDIT_Motor_1_Init,Motor_1_Initial_Position);

	//STATICָʾ�ı�
	CString Indication_Content_0="#0Ŀ��";
	CString Indication_Content_1="#1Ŀ��";
	GetDlgItem(IDC_STATIC_Indication_0)->SetWindowText(Indication_Content_0);
	GetDlgItem(IDC_STATIC_Indication_1)->SetWindowText(Indication_Content_1);
}

// ����MsComm�ؼ�ͨ�����ڷ���ָ��
void CCamera_Cradle_ControlDlg::Serial_Port_Send_Data(UCHAR* char_array_in, int element_num)
{
	if (!Allow_Serial_Port_Transmit)//����δ��ʱ���ɷ������ݣ�Allow_Serial_Port_Transmit����ĳ�Ա����
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
		varOut=COleVariant(bytMiddleArr); //������ת��Ϊ������������
		m_MsComm.put_Output(varOut); //��������
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



//�϶�Slider�ؼ�
void CCamera_Cradle_ControlDlg::OnNMCustomdrawSliderMotor0Position(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����slider�Ϸ�static�ؼ�ֵ:
	Show_STATIC_Slider_Value();

	*pResult = 0;
}

void CCamera_Cradle_ControlDlg::OnNMCustomdrawSliderMotor1Position(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����slider�Ϸ�static�ؼ�ֵ:
	Show_STATIC_Slider_Value();

	*pResult = 0;
}

// ����ָ����ƶ��ת��
void CCamera_Cradle_ControlDlg::Motor_Run(UCHAR ID, int Goal_Position)
{
	//���ɶ��ת��ָ��
	UCHAR Goal_Position_High,Goal_Position_Low;
	UCHAR Motor_Run_Instruction[9]={0xff, 0xff, 0x00, 0x05, 0x03, 0x1e, 0x00, 0x00,0x00};
	Goal_Position_High=Goal_Position / 256; //ȡ��
	Goal_Position_Low=Goal_Position % 256; //ȡ��
	Motor_Run_Instruction[2]=ID;
	Motor_Run_Instruction[6]=Goal_Position_Low;
	Motor_Run_Instruction[7]=Goal_Position_High;
	Motor_Run_Instruction[8]=Check_Sum(Motor_Run_Instruction,7);

	//���ڷ���ָ��
	Serial_Port_Send_Data(Motor_Run_Instruction,9);

}

// ������ָ��CHECK_SUM����
UCHAR CCamera_Cradle_ControlDlg::Check_Sum(UCHAR* Command_Array, UCHAR Last_Index)
{
	UCHAR i;
	UCHAR SUM=0;
	for (i=2;i<=Last_Index;i++)//��index=2��Ԫ�ؿ�ʼ����Ӧ��IDֵ
	{
		SUM=SUM+Command_Array[i];		
	}
	SUM=~SUM;
	return SUM;
}
// //����slider�ؼ�ֵ��ˢ��Edit�ؼ�ֵ
void CCamera_Cradle_ControlDlg::Show_Data(void)
{
		int Slider_0_Position,Slider_1_Position;
		CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
		CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
		Slider_0_Position=pointer_Slider_Motor_0->GetPos();
		Slider_1_Position=pointer_Slider_Motor_1->GetPos();
		if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
		{		
			//���λ��EDIT��ʾ
			int Goal_Position_0,Goal_Position_1;
			Goal_Position_0=Slider_0_Position;
			Goal_Position_1=Slider_1_Position;
			SetDlgItemInt(IDC_EDIT_Motor_0_Position,Goal_Position_0);
			SetDlgItemInt(IDC_EDIT_Motor_1_Position,Goal_Position_1);

			//ת�Ǽ��㼰��EDIT�ؼ���ʾ
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
			//��ת�Ƕȼ��㼰��EDIT�ؼ���ʾ
			double Self_Rotate_Angle_Value=(double)Slider_0_Position*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
			CString Self_Rotate_Angle_string;
			Self_Rotate_Angle_string.Format("%.1f",Self_Rotate_Angle_Value);
			SetDlgItemText(IDC_EDIT_Self_Rotate_Angle,Self_Rotate_Angle_string);
			
			//�����Ƕȼ��㼰��EDIT�ؼ���ʾ
			double Pitch_Angle_Value=(double)Slider_1_Position*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;
			CString Pitch_Angle_string;
			Pitch_Angle_string.Format("%.1f",Pitch_Angle_Value);
			SetDlgItemText(IDC_EDIT_Pitch_Angle,Pitch_Angle_string);

			//���λ�ü��㼰��EDIT�ؼ���ʾ
			int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
			int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;
			SetDlgItemInt(IDC_EDIT_Motor_0_Position,Motor0_Pos);
			SetDlgItemInt(IDC_EDIT_Motor_1_Position,Motor1_Pos);

		}
}


//Ӧ���µĶ����ʼλ��ֵ
void CCamera_Cradle_ControlDlg::OnBnClickedButtonApplyInit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//����INI�ļ�
	char Motor_0_Init_Value[10],Motor_1_Init_Value[10];
	GetDlgItemText(IDC_EDIT_Motor_0_Init,Motor_0_Init_Value,10);
	GetDlgItemText(IDC_EDIT_Motor_1_Init,Motor_1_Init_Value,10);
	WritePrivateProfileString("Motor Initial Position","Motor0",Motor_0_Init_Value,INI_Path);
	WritePrivateProfileString("Motor Initial Position","Motor1",Motor_1_Init_Value,INI_Path);

	//����ȫ�ֱ���
	Motor_0_Initial_Position=GetDlgItemInt(IDC_EDIT_Motor_0_Init);
	Motor_1_Initial_Position=GetDlgItemInt(IDC_EDIT_Motor_1_Init);

	//������ת�Ǻ͸����ǵķ�Χ�����ڿؼ�����ʾ
	Angle_Range_Calculate_And_Show();

	//����slider�ؼ�ֵ����EDIT��ʾ
	Show_Data();

	//����slider�Ϸ�static�ؼ�ֵ:
	Show_STATIC_Slider_Value();
}

//����ģʽѡ��
void CCamera_Cradle_ControlDlg::OnBnClickedRadioMotorDrive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//Slider�ؼ��ָ��ɳ�ʼλ��
	Motor_Init();
	//STATICָʾ�ı�
	CString Indication_Content_0="#0Ŀ��";
	CString Indication_Content_1="#1Ŀ��";
	GetDlgItem(IDC_STATIC_Indication_0)->SetWindowText(Indication_Content_0);
	GetDlgItem(IDC_STATIC_Indication_1)->SetWindowText(Indication_Content_1);
}

void CCamera_Cradle_ControlDlg::OnBnClickedRadioAngleDrive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//STATICָʾ�ı�
	CString Indication_Content_0="��תĿ��";
	CString Indication_Content_1="����Ŀ��";
	GetDlgItem(IDC_STATIC_Indication_0)->SetWindowText(Indication_Content_0);
	GetDlgItem(IDC_STATIC_Indication_1)->SetWindowText(Indication_Content_1);

	//Slider�ؼ��ָ��ɳ�ʼλ��
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	int Slider_Pos_Self_Rotate=(-Sefl_Rotate_Min*4095)/(Sefl_Rotate_Max-Sefl_Rotate_Min);
	pointer_Slider_Motor_0->SetPos(Slider_Pos_Self_Rotate);
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	int Slider_Pos_Pitch=(-Pitch_Min*4095)/(Pitch_Max-Pitch_Min);
	pointer_Slider_Motor_1->SetPos(Slider_Pos_Pitch);
	
	//���ƶ��
	Motor_Run(0x00,Motor_0_Initial_Position);
	Sleep(100);
	Motor_Run(0x01,Motor_1_Initial_Position);
	Sleep(100);
	Motor_Run(0x00,Motor_0_Initial_Position);
	Sleep(100);
	Motor_Run(0x01,Motor_1_Initial_Position);

	//EDIT�ؼ�����
	Show_Data();
}

//������ת�Ǻ͸����ǵķ�Χ�����ڿؼ�����ʾ
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

//���Ķ���ƶ��ٶ�
void CCamera_Cradle_ControlDlg::Speed_Modify(void)
{
	//���ɶ��ת��ָ��
	WORD Moving_Speed=20;
	UCHAR Speed_High,Speed_Low;
	UCHAR Moving_Speed_Instruction[9]={0xff, 0xff, 0x00, 0x05, 0x03, 0x20, 0x00, 0x00,0x00};
	Speed_High=Moving_Speed / 256; //ȡ��
	Speed_Low=Moving_Speed % 256; //ȡ��
	Moving_Speed_Instruction[6]=Speed_Low;
	Moving_Speed_Instruction[7]=Speed_High;

	Moving_Speed_Instruction[2]=0x00;//ID
	Moving_Speed_Instruction[8]=Check_Sum(Moving_Speed_Instruction,7);
	Serial_Port_Send_Data(Moving_Speed_Instruction,9);//���ڷ���ָ��
	
	Sleep(100);

	Moving_Speed_Instruction[2]=0x01;//ID
	Moving_Speed_Instruction[8]=Check_Sum(Moving_Speed_Instruction,7);
	Serial_Port_Send_Data(Moving_Speed_Instruction,9);//���ڷ���ָ��
}

void CCamera_Cradle_ControlDlg::OnNMReleasedcaptureSliderMotor0Position(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�ؼ�ֵ
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	int Slider_Position=pointer_Slider_Motor_0->GetPos();
	int Goal_Position=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		Goal_Position=Slider_Position;
		//�ƶ����
		Motor_Run(0x00,Goal_Position);	
	}
	else
	{
		//��ת�Ƕ�
		double Self_Rotate_Angle_Value=(double)Slider_Position*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
	
		//���λ�û��㼰���EDIT�ؼ���ʾ
		CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
		double Pitch_Angle_Value=pointer_Slider_Motor_1->GetPos()*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;
		int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
		int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;
	
		//ת�����
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

	//EDIT�ؼ���ʾ
	Show_Data();

	*pResult = 0;
}

void CCamera_Cradle_ControlDlg::OnNMReleasedcaptureSliderMotor1Position(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�ؼ�ֵ
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	int Slider_Position=pointer_Slider_Motor_1->GetPos();
	int Goal_Position=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		Goal_Position=Slider_Position;
		//�ƶ����
		Motor_Run(0x01,Goal_Position);	
		
	}
	else
	{
		//�����Ƕ�
		double Pitch_Angle_Value=(double)Slider_Position*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;

		//���λ�û��㼰���EDIT�ؼ���ʾ
		CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
		double Self_Rotate_Angle_Value=pointer_Slider_Motor_0->GetPos()*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
		int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
		int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;

		//ת�����
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

	//EDIT�ؼ���ʾ
	Show_Data();

	*pResult = 0;
}

void CCamera_Cradle_ControlDlg::OnBnClickedButtonSlider0ValueImplement()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�ؼ�ֵ
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	int Slider_Position=pointer_Slider_Motor_0->GetPos();
	int Goal_Position=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		Goal_Position=Slider_Position;
		//�ƶ����
		Motor_Run(0x00,Goal_Position);	
	}
	else
	{
		//��ת�Ƕ�
		double Self_Rotate_Angle_Value=(double)Slider_Position*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;

		//���λ�û��㼰���EDIT�ؼ���ʾ
		CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
		double Pitch_Angle_Value=pointer_Slider_Motor_1->GetPos()*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;
		int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
		int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;

		//ת�����
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

	//EDIT�ؼ���ʾ
	Show_Data();

}

void CCamera_Cradle_ControlDlg::OnBnClickedButtonSlider1ValueImplement()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�ؼ�ֵ
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	int Slider_Position=pointer_Slider_Motor_1->GetPos();
	int Goal_Position=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		Goal_Position=Slider_Position;
		//�ƶ����
		Motor_Run(0x01,Goal_Position);	

	}
	else
	{
		//�����Ƕ�
		double Pitch_Angle_Value=(double)Slider_Position*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;

		//���λ�û��㼰���EDIT�ؼ���ʾ
		CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
		double Self_Rotate_Angle_Value=pointer_Slider_Motor_0->GetPos()*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
		int Motor0_Pos=(int)((Self_Rotate_Angle_Value+Pitch_Angle_Value)*11.375)+Motor_0_Initial_Position;
		int Motor1_Pos=(int)((Self_Rotate_Angle_Value-Pitch_Angle_Value)*11.375)+Motor_1_Initial_Position;

		//ת�����
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

	//EDIT�ؼ���ʾ
	Show_Data();

}

// ������ʾָʾsliderֵ��STATIC�ؼ�
void CCamera_Cradle_ControlDlg::Show_STATIC_Slider_Value(void)
{
	//��ȡ�ؼ�ֵ
	CSliderCtrl *pointer_Slider_Motor_0=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_0_Position);
	int Slider_Position_0=pointer_Slider_Motor_0->GetPos();
	//STATIC�ؼ���ʾ
	CString Motor0_string;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		//���λ��
		Motor0_string.Format("%d",Slider_Position_0);
		Motor0_string="��"+Motor0_string+"��";
		GetDlgItem(IDC_STATIC_Slider_0)->SetWindowText(Motor0_string);
	}
	else
	{
		//��ת�Ƕ�
		double Self_Rotate_Angle_Value=(double)Slider_Position_0*(Sefl_Rotate_Max-Sefl_Rotate_Min)/4095+Sefl_Rotate_Min;
		CString Self_Rotate_Angle_string;
		Self_Rotate_Angle_string.Format("%.1f",Self_Rotate_Angle_Value);
		Self_Rotate_Angle_string="��"+Self_Rotate_Angle_string+"��";
		GetDlgItem(IDC_STATIC_Slider_0)->SetWindowText(Self_Rotate_Angle_string);
	}

	//��ȡ�ؼ�ֵ
	CSliderCtrl *pointer_Slider_Motor_1=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_Motor_1_Position);
	int Slider_Position_1=pointer_Slider_Motor_1->GetPos();
	//STATIC�ؼ���ʾ
	CString Motor1_string;
	if (((CButton *)GetDlgItem(IDC_RADIO_Motor_Drive))->GetCheck())
	{
		//���λ��
		Motor1_string.Format("%d",Slider_Position_1);
		Motor1_string="��"+Motor1_string+"��";
		GetDlgItem(IDC_STATIC_Slider_1)->SetWindowText(Motor1_string);
	}
	else
	{
		//��ת�Ƕ�
		double Pitch_Angle_Value=(double)Slider_Position_1*(Pitch_Max-Pitch_Min)/4095+Pitch_Min;
		CString Pitch_Angle_string;
		Pitch_Angle_string.Format("%.1f",Pitch_Angle_Value);
		Pitch_Angle_string="��"+Pitch_Angle_string+"��";
		GetDlgItem(IDC_STATIC_Slider_1)->SetWindowText(Pitch_Angle_string);
	}

}
