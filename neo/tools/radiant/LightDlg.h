/*
===========================================================================

KROOM 3 GPL Source Code

This file is part of the KROOM 3 Source Code, originally based
on the Doom 3 with bits and pieces from Doom 3 BFG edition GPL Source Codes both published in 2011 and 2012.

KROOM 3 Source Code is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version. For details, see LICENSE.TXT.

Extra attributions can be found on the CREDITS.txt file

===========================================================================
*/

#pragma once

#include "../common/GLWidget.h"

class CLightInfo
{
public:
	CLightInfo();

	bool		pointLight;
	float		fallOff;
	CString		strTexture;
	bool		equalRadius;
	bool		explicitStartEnd;
	idVec3		lightStart;
	idVec3		lightEnd;
	idVec3		lightUp;
	idVec3		lightRight;
	idVec3		lightTarget;
	idVec3		lightCenter;
	idVec3		color;
	bool		fog;
	idVec4		fogDensity;

	bool		strobe;
	float		strobeSpeed;
	bool		rotate;
	float		rotateSpeed;

	idVec3		lightRadius;
	bool		castShadows;
	bool		castSpecular;
	bool		castDiffuse;
	bool		hasCenter;
	bool		isParallel;

	void		Defaults();
	void		DefaultProjected();
	void		DefaultPoint();
	void		FromDict( const idDict* e );
	void		ToDict( idDict* e );
	void		ToDictFromDifferences( idDict* e, const idDict* differences );
	void		ToDictWriteAllInfo( idDict* e );
};

// CLightDlg dialog

class CLightDlg : public CDialogEx
{
public:
	CLightDlg( CWnd* pParent = NULL );   // standard constructor
	~CLightDlg();

	void			UpdateDialogFromLightInfo();
	void			UpdateDialog( bool updateChecks );
	void			UpdateLightInfoFromDialog();
	void			UpdateColor( float r, float g, float b, float a );
	void			SetSpecifics();
	void			EnableControls();
	void			LoadLightTextures();
	void			ColorButtons();
	void			SaveLightInfo( const idDict* differences );

	enum { IDD = IDD_DIALOG_LIGHT };
	idGLWidget m_wndPreview;
	CComboBox	m_wndLights;
	CSliderCtrl	m_wndFalloff;
	BOOL	m_bEqualRadius;
	BOOL	m_bExplicitFalloff;
	BOOL	m_bPointLight;
	BOOL	m_bCheckProjected;
	float	m_fFallloff;
	int		m_nFalloff;
	BOOL	m_bRotate;
	BOOL	m_bShadows;
	BOOL	m_bSpecular;
	BOOL	m_bDiffuse;
	float	m_fEndX;
	float	m_fEndY;
	float	m_fEndZ;
	float	m_fRadiusX;
	float	m_fRadiusY;
	float	m_fRadiusZ;
	float	m_fRightX;
	float	m_fRightY;
	float	m_fRightZ;
	float	m_fRotate;
	float	m_fStartX;
	float	m_fStartY;
	float	m_fStartZ;
	float	m_fTargetX;
	float	m_fTargetY;
	float	m_fTargetZ;
	float	m_fUpX;
	float	m_fUpY;
	float	m_fUpZ;
	BOOL	m_hasCenter;
	float	m_centerX;
	float	m_centerY;
	float	m_centerZ;
	BOOL    m_bIsParallel;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnTexture();
	afx_msg void OnCheckEqualradius();
	afx_msg void OnCheckExplicitfalloff();
	afx_msg void OnCheckPoint();
	afx_msg void OnCheckProjected();
	afx_msg void OnRadioFalloff();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnBtnColor();
	afx_msg void OnBtnFog();
	afx_msg void OnCheckFog();
	afx_msg void OnCheckRotate();
	afx_msg void OnCheckStrobe();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	afx_msg void OnSelchangeComboTexture();
	afx_msg void OnCheckCenter();
	afx_msg void OnCheckParallel();
	afx_msg void OnApplyDifferences();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBitmap					colorBitmap;
	CBitmap					fogBitmap;
	CLightInfo				lightInfo;
	CLightInfo				lightInfoOriginal;
	idVec3					color;
	idGLDrawableMaterial* 	m_drawMaterial;
};