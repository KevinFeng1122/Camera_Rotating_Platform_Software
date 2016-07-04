
// Camera_Cradle_ControlDlg.h : 头文件
//

#pragma once
#include "mscomm1.h"



// CCamera_Cradle_ControlDlg 对话框
class CCamera_Cradle_ControlDlg : public CDialog
{

// 构造
public:
	CCamera_Cradle_ControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CAMERA_CRADLE_CONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnDropdownComboPortList();
private:
	// 从注册表内找出存在的串口号并写入一个地址中
	void FindSerialPort(CComboBox *pComboBox);
public:
	afx_msg void OnBnClickedButtonPortConnect();
private:
	CMscomm1 m_MsComm;
public:
	// 软件打开时舵机初始化
	void Motor_Init(void);
private:
	// 利用MsComm控件通过串口发送指令
	void Serial_Port_Send_Data(UCHAR* char_array_in, int element_num);
private:
	// 发送指令控制舵机转动
	void Motor_Run(UCHAR ID, int Goal_Position);
	// 计算舵机指令CHECK_SUM参数
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
	// //根据slider控件值，刷新Edit控件值
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
	// 更新显示指示slider值的STATIC控件
	void Show_STATIC_Slider_Value(void);
};
