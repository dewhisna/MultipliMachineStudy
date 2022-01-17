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

#ifndef FORMATIONCALCDIALOG_H
#define FORMATIONCALCDIALOG_H

#include <QDialog>
#include <QColor>
#include "KeyPressFilter.h"

// =============================================================================

namespace Ui {
	class CFormationCalcDialog;
}

class CFormationCalcDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CFormationCalcDialog(bool bStandAlone, bool bMetric, QWidget *parent = nullptr);
	~CFormationCalcDialog();

	enum CYLINDER_SIZE_ENUM {
		CSE_36in = 0,
		CSE_42in = 1,
		CSE_48in = 2,
	};

	enum BOARD_TYPE_ENUM {
		BTE_FoldingBoxBoard = 0,
		BTE_TubeBoard = 1,
	};

	struct TDialogValues {
		TDialogValues()
			:	m_nCaliper(0.0),
				m_nDensity(0.0),
				m_nBasisWeight(0.0),
				m_nSheetWidth(0.0),
				m_nFormConst(0.0),
				m_nCylinderSize(CSE_42in),			// Initial defaults Cylinder Size
				m_nBoardType(BTE_FoldingBoxBoard),	// Initial defaults Board Type
				m_bOtherFormConst(false),
				m_bAutoCalc(true),
				m_nYieldPerDay(0.0),
				m_nConsistency(0.0),
				m_nWaterNeeded(0.0),
				m_nWaterNeededPerVat(0.0),
				m_nNumberOfVats(1),
				m_nMachineSpeed(0.0),
				m_nMachineEfficiency(1.0)
		{ }
		double m_nCaliper;				// In Imperial Units (points)
		double m_nDensity;				// In Imperial Units (lbs per point)
		double m_nBasisWeight;			// In Imperial Units (lbs per 1K Sq. Inches), (calculated output only)
		double m_nSheetWidth;			// In Imperial Units (inches)
		double m_nFormConst;			// In Imperial Units (Tons Per Day Per Inches of Forming surface width)
		CYLINDER_SIZE_ENUM m_nCylinderSize;		// Cylinder Size index for formation constant
		BOARD_TYPE_ENUM m_nBoardType;	// Board Type index for formation constant
		bool m_bOtherFormConst;			// Other Formation Constant than these from Cylinder Size and Board Type
		bool m_bAutoCalc;				// True if automatically calculating production values
		double m_nYieldPerDay;			// In Imperial Units (Tons Per Day)
		double m_nConsistency;			// Normalized Percentage (unitless) 0.0 - 1.0
		double m_nWaterNeeded;			// In Imperial Units (Gallons Per Minute) (calculated output only)
		double m_nWaterNeededPerVat;	// In Imperial Units (Gallons Per Minute) (calculated output only)
		int m_nNumberOfVats;			// Unitless
		double m_nMachineSpeed;			// In Imperial Units (Ft/min)
		double m_nMachineEfficiency;	// Normalized Percentage (unitless) 0.0 - 1.0
	} m_DialogValues;			// Values to set coming in or final values (after validation) going out

signals:
	void metricModeSelected(bool bMetric);		// Option toggle

public slots:
	void setMetric(bool bMetric);

	virtual int exec() override;

protected slots:
	void saveValues();
	void setValues();
	bool calc1(bool bPrompt = true);	// Called on change of Caliper/Density/SheetWidth/FormConst/NumVats/MachEff -> Calcs BasisWeight/YieldPerDay/MachineSpeed
	bool calc2(bool bPrompt = true);	// Called on change of YieldPerDay/Consistency/NumVats/MachEff -> Calcs WaterNeeded/WaterNeededPerVat/NumVats?
	bool doVatsCalcValid();

	void doGrade();
	void doFormConstOtherCalc();
	void doPipeSizing();
	void doAreaSizing();
	void doHeadBoxDilution();
	void doVatsCalc();
	void doProductionCalc();

	void showHelp();

	void en_changeCaliper();
	void en_changeDensity();
	void en_changeSheetWidth();
	void en_changeFormConst();
	void en_changeYieldPerDay();
	void en_changeConsistency();
	void en_changeNumberOfVats();
	void en_changeMachineEfficiency();

	void en_changeCylinderBoardType();		// Triggered on any of the Cylinder Type or Board Type Radio Buttons or the 'other' checkbox

	void en_changeAutoCalc();

	virtual void accept() override;
	virtual void reject() override;

protected:
	void setUIText();
	void setCylinderBoardType(CYLINDER_SIZE_ENUM nCSE, BOARD_TYPE_ENUM nBTE, bool bOtherForm);

	bool validateCaliper(double *pnCaliper, bool bPrompt);
	bool validateDensity(double *pnDensity, bool bPrompt);
	bool validateSheetWidth(double *pnSheetWidth, bool bPrompt);
	bool validateFormConst(double *pnFormConst, bool bPrompt);
	bool validateYieldPerDay(double *pnYieldPerDay, bool bPrompt);
	bool validateConsistency(double *pnConsistency, bool bPrompt);
	bool validateNumberOfVats(int *pnNumberOfVats, bool bPrompt);

private:
	bool m_bStandAlone;
	bool m_bMetric;
	bool m_bChangingMetricMode;
	bool m_bCylinderBoardTypeUpdateInProgress;	// True if currently updating the radio buttons, so calculations can be done only once
	QColor m_clrEditEnabled;
	QColor m_clrEditDisabled;
	QPushButton *m_pSaveApplyButton;
	CKeyPressEnterEater m_enterEater;
	Ui::CFormationCalcDialog *ui;
};

// =============================================================================

#endif // FORMATIONCALCDIALOG_H
