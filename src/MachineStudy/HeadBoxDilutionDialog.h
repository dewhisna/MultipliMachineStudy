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

#ifndef HEADBOXDILUTIONDIALOG_H
#define HEADBOXDILUTIONDIALOG_H

#include <QDialog>
#include <QPushButton>

#include "KeyPressFilter.h"

// =============================================================================

namespace Ui {
	class CHeadBoxDilutionDialog;
}

class CHeadBoxDilutionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CHeadBoxDilutionDialog(bool bStandAlone, bool bMetric, QWidget *parent = nullptr);
	~CHeadBoxDilutionDialog();

	struct TDialogValues {
		TDialogValues()
			:	m_nYieldPerDay(0.0),
				m_nPresentConsistency(1.0),
				m_nNewConsistency(1.0),
				m_nWaterNeeded(0.0)
		{ }
		double m_nYieldPerDay;			// In Imperial Units (Tons Per Day)
		double m_nPresentConsistency;	// Normalized Percentage (unitless) 0.0 - 1.0
		double m_nNewConsistency;		// Normalized Percentage (unitless) 0.0 - 1.0
		double m_nWaterNeeded;			// In Imperial Units (Gallons Per Minute), (calculated output only)
	} m_DialogValues;			// Values to set coming in or final values (after validation) going out

public slots:
	void setMetric(bool bMetric);

	virtual int exec() override;

protected slots:
	bool calc(bool bPrompt = true);

	void showHelp();

	void en_changeYieldPerDay();
	void en_changePresentConsistency();
	void en_changeNewConsistency();

	virtual void accept() override;

protected:
	void setUIText();

	bool validateYieldPerDay(double *pnYieldPerDay, bool bPrompt);
	bool validatePresentConsistency(double *pnPresentConsistency, bool bPrompt);
	bool validateNewConsistency(double *pnNewConsistency, bool bPrompt);

private:
	bool m_bStandAlone;
	bool m_bMetric;
	QPushButton *m_pCalcButton;
	QPushButton *m_pSaveApplyButton;
	CKeyPressEnterEater m_enterEater;
	Ui::CHeadBoxDilutionDialog *ui;
};

// =============================================================================

#endif // HEADBOXDILUTIONDIALOG_H
