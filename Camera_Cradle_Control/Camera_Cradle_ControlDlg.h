
// Camera_Cradle_ControlDlg.h : ͷ�ļ�
//

#pragma once
#include "mscomm1.h"



// CCamera_Cradle_ControlDlg �Ի���
class CCamera_Cradle_ControlDlg : public CDialog
{

// ����
public:
	CCamera_Cradle_ControlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CAMERA_CRADLE_CONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnDropdownComboPortList();
private:
	// ��ע������ҳ����ڵĴ��ںŲ�д��һ����ַ��
	void FindSerialPort(CComboBox *pComboBox);
public:
	afx_msg void OnBnClickedButtonPortConnect();
private:
	CMscomm1 m_MsComm;
public:
	// �����ʱ�����ʼ��
	void Motor_Init(void);
private:
	// ����MsComm�ؼ�ͨ�����ڷ���ָ��
	void Serial_Port_Send_Data(UCHAR* char_array_in, int element_num);
private:
	// ����ָ����ƶ��ת��
	void Motor_Run(UCHAR ID, int Goal_Position);
	// ������ָ��CHECK_SUM����
	UCHAR Check_Sum(UCHAR* Command_Array,UCHAR Last_Index);
public:
	afx_msg void OnNMCustomdrawSliderMotor0Position(NMHDR *pNMHDR, LRESULT *pResult);
private:
	bool Allow_Serial_Port_Transmit;
	int Motor_0_Initial_Position;
	int Motor_1_Initial_Position;
	double Sefl_Rotate_Max,Sefl_Rotate_Min;
	double Pitch_Max,Pitch_Min;
	char APP_Path[100];
	CString INI_Path;
public:
	// //����slider�ؼ�ֵ��ˢ��Edit�ؼ�ֵ
	void Show_Data(void);
	afx_msg void OnNMCustomdrawSliderMotor1Position(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonApplyInit();
	afx_msg void OnBnClickedRadioMotorDrive();
	afx_msg void OnBnClickedRadioAngleDrive();
	void Angle_Range_Calculate_And_Show(void);
	void Speed_Modify(void);
	afx_msg void OnNMReleasedcaptureSliderMotor0Position(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderMotor1Position(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSlider0ValueImplement();
	afx_msg void OnBnClickedButtonSlider1ValueImplement();
	// ������ʾָʾsliderֵ��STATIC�ؼ�
	void Show_STATIC_Slider_Value(void);
};
