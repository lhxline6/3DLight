#pragma once


// CSkyBoxEditor �Ի���

class CSkyBoxEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CSkyBoxEditor)

public:
	CSkyBoxEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSkyBoxEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };
	
//attributes
	CString m_strUpPath;
	CString m_strDownPath;
	CString m_strLeftPath;
	CString m_strRightPath;
	CString m_strFrontPath;
	CString m_strBackPath;

	CRect m_rectUp;
	CRect m_rectDown;
	CRect m_rectLeft;
	CRect m_rectRight;
	CRect m_rectFront;
	CRect m_rectBack;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedSkyboxUp();
	afx_msg void OnBnClickedSkyboxLeft();
	afx_msg void OnBnClickedSkyboxDown();
	afx_msg void OnBnClickedSkyboxRight();
	afx_msg void OnBnClickedSkyboxFront();
	afx_msg void OnBnClickedSkyboxBack();
};
