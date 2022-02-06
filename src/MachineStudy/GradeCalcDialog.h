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

#ifndef GRADECALCDIALOG_H
#define GRADECALCDIALOG_H

#include <QDialog>
#include "KeyPressFilter.h"

// =============================================================================

namespace Ui {
	class CGradeCalcDialog;
}

class CGradeCalcDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CGradeCalcDialog(bool bStandAlone, bool bMetric, QWidget *parent = nullptr);
	virtual ~CGradeCalcDialog();

	struct TDialogValues {
		TDialogValues()
			:	m_nCaliper(0.0),
				m_nDensity(0.0),
				m_nBasisWeight(0.0)
		{ }
		double m_nCaliper;		// In Imperial Units (points)
		double m_nDensity;		// In Imperial Units (lbs per point)
		double m_nBasisWeight;	// In Imperial Units (lbs per 1K Sq. Inches)
	} m_DialogValues;			// Values to set coming in or final values (after validation) going out

public slots:
	void setMetric(bool bMetric);

	virtual int exec() override;

protected slots:
	void calcCaliper();
	void calcDensity();
	void calcBasisWeight();

	void en_changeCaliper();
	void en_changeDensity();
	void en_changeBasisWeight();
	void setEnableSaveApply();

	virtual void accept() override;

protected:
	void setUIText();

	bool validateCaliper(double *pnCaliper, bool bPrompt);
	bool validateDensity(double *pnDensity, bool bPrompt);
	bool validateBasisWeight(double *pnBasisWeight, bool bPrompt);

private:
	bool m_bStandAlone;
	bool m_bMetric;
	QPushButton *m_pSaveApplyButton;
	CKeyPressEnterEater m_enterEater;
	Ui::CGradeCalcDialog *ui;
};

// =============================================================================

#endif // GRADECALCDIALOG_H
