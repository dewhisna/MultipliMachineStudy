/****************************************************************************
**
** Multipli Machine Study Application
** Copyright (C) 1994, 2018-2022 Multipli Machinery, Corp.
** Contact: http://multiplimachine.com/
**
** This file is part of the Multipli Machine Study Application,
** written and developed for Multipli Machinery by Donald Whisnant.
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU General Public License
** version 3.0 as published by the Free Software Foundation and appearing
** in the file gpl-3.0.txt included in the packaging of this file. Please
** review the following information to ensure the GNU General Public License
** version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and
** Multipli Machinery Corp.
**
****************************************************************************/

#ifndef FORMCONSTDIALOG_H
#define FORMCONSTDIALOG_H

#include <QDialog>
#include "KeyPressFilter.h"

// =============================================================================

namespace Ui {
	class CFormConstDialog;
}

class CFormConstDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CFormConstDialog(bool bStandAlone, bool bMetric, QWidget *parent = nullptr);
	virtual ~CFormConstDialog();

	struct TDialogValues {
		TDialogValues()
			:	m_nYieldPerDay(0.0),
				m_nNumberOfVats(0),
				m_nSheetWidth(0.0),
				m_nMachineEfficiency(1.0),
				m_nFormConst(0.0)
		{ }
		double m_nYieldPerDay;			// In Imperial Units (Tons Per Day)
		int m_nNumberOfVats;			// Unitless (counts), whole number
		double m_nSheetWidth;			// In Imperial Units (inches)
		double m_nMachineEfficiency;	// Normalized Percentage (unitless) 0.0 - 1.0
		double m_nFormConst;			// In Imperial Units (Tons Per Day Per Inches of Forming surface width) (calculated output only)
	} m_DialogValues;			// Values to set coming in or final values (after validation) going out

public slots:
	void setMetric(bool bMetric);

	virtual int exec() override;

protected slots:
	bool calc(bool bPrompt = true);

	void en_changeYieldPerDay();
	void en_changeNumberOfVats();
	void en_changeSheetWidth();
	void en_changeMachineEfficiency();

	virtual void accept() override;

protected:
	void setUIText();

	bool validateYieldPerDay(double *pnYieldPerDay, bool bPrompt);
	bool validateNumberOfVats(int *pnNumberOfVats, bool bPrompt);
	bool validateSheetWidth(double *pnSheetWidth, bool bPrompt);
	bool validateMachineEfficiency(double *pnMachineEfficiency, bool bPrompt);

private:
	bool m_bStandAlone;
	bool m_bMetric;
	QPushButton *m_pCalcButton;
	QPushButton *m_pSaveApplyButton;
	CKeyPressEnterEater m_enterEater;
	Ui::CFormConstDialog *ui;
};

// =============================================================================

#endif // FORMCONSTDIALOG_H
