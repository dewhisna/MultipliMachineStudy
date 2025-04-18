/****************************************************************************
**
** Multipli Machine Study Application
** Copyright (C) 1994, 2018-2025 Multipli Machinery, Corp.
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

#ifndef AREAFLOWCALCDIALOG_H
#define AREAFLOWCALCDIALOG_H

#include <QDialog>
#include "KeyPressFilter.h"

// =============================================================================

namespace Ui {
	class CAreaFlowCalcDialog;
}

class CAreaFlowCalcDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CAreaFlowCalcDialog(bool bStandAlone, bool bMetric, QWidget *parent = nullptr);
	virtual ~CAreaFlowCalcDialog();

	struct TDialogValues {
		TDialogValues()
			:	m_nVelocity(0.0),
				m_nWaterNeeded(0.0),
				m_nCrossSectArea(0.0)
		{ }
		double m_nVelocity;			// In Imperial Units (Ft/Sec)
		double m_nWaterNeeded;		// In Imperial Units (Gallons Per Minute)
		double m_nCrossSectArea;	// In Imperial Units (Sq. Inches)
	} m_DialogValues;			// Values to set coming in or final values (after validation) going out

public slots:
	void setMetric(bool bMetric);

	virtual int exec() override;

protected slots:
	void calcVelocity();
	void calcWaterNeeded();
	void calcCrossSectArea();

	void showHelp();

	void en_changeVelocity();
	void en_changeWaterNeeded();
	void en_changeCrossSectArea();
	void setEnableSaveApply();

	virtual void accept() override;

protected:
	void setUIText();

	bool validateVelocity(double *pnVelocity, bool bPrompt);
	bool validateWaterNeeded(double *pnWaterNeeded, bool bPrompt);
	bool validateCrossSectArea(double *pnCrossSectArea, bool bPrompt);

private:
	bool m_bStandAlone;
	bool m_bMetric;
	QPushButton *m_pSaveApplyButton;
	CKeyPressEnterEater m_enterEater;
	Ui::CAreaFlowCalcDialog *ui;
};

// =============================================================================

#endif // AREAFLOWCALCDIALOG_H
