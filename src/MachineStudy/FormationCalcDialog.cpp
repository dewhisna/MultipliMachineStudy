/****************************************************************************
**
** Multipli Machine Study Application
** Copyright (C) 2018 Multipli Machinery, Corp.
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

#include "FormationCalcDialog.h"
#include "ui_FormationCalcDialog.h"

#include "conversion.h"

#include "MainWindow.h"
#include "GradeCalcDialog.h"
#include "PipeFlowCalcDialog.h"
#include "AreaFlowCalcDialog.h"
#include "HeadBoxDilutionDialog.h"
#include "FormConstDialog.h"
#include "YieldPerDayDialog.h"
#include "ProductionCalcDialog.h"
#include "HelpDialog.h"

#include <QPushButton>
#include <QMessageBox>
#include <QPalette>

#include <math.h>

// =============================================================================

namespace {
	// Cylinder Size Formation Constants in imperial Tons Per Day Per Inches of Forming surface width:
	const double g_CylinderSizeConst[] = {		// These indexes must match CYLINDER_SIZE_ENUM
		0.852,		// 36" Cylinder
		1.0,		// 42" Cylinder
		1.126		// 48" Cylinder
	};

	// Board Type Formation Constants in imperial Tons Per Day Per Inches of Forming surface width:
	const double g_BoardTypeConst[] = {			// These indexes must match BOARD_TYPE_ENUM
		0.217,		// Folding Box Board
		0.192		// Tube Board
	};
}

// -----------------------------------------------------------------------------

CFormationCalcDialog::CFormationCalcDialog(bool bStandAlone, bool bMetric, QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
	m_bStandAlone(bStandAlone),
	m_bMetric(bMetric),
	m_bChangingMetricMode(false),
	m_bCylinderBoardTypeUpdateInProgress(false),
	m_pSaveApplyButton(nullptr),
	ui(new Ui::CFormationCalcDialog)
{
	ui->setupUi(this);

	installEventFilter(&m_enterEater);

	QPushButton *pSaveButton = ui->buttonBox->button(QDialogButtonBox::Save);
	assert(pSaveButton != nullptr);
	if (pSaveButton) {
		pSaveButton->setVisible(bStandAlone);
	}
	QPushButton *pApplyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
	assert(pApplyButton != nullptr);
	if (pApplyButton) {
		pApplyButton->setVisible(!bStandAlone);
	}
	QPushButton *pHelpButton = ui->buttonBox->button(QDialogButtonBox::Help);
	if (pHelpButton) {
		connect(pHelpButton, SIGNAL(clicked(bool)), this, SLOT(showHelp()));
	}

	m_pSaveApplyButton = bStandAlone ? pSaveButton : pApplyButton;
	connect(m_pSaveApplyButton, SIGNAL(clicked(bool)), this, SLOT(accept()));

	ui->checkMetric->setChecked(bMetric);
	connect(ui->checkMetric, SIGNAL(toggled(bool)), this, SLOT(setMetric(bool)));

	m_clrEditEnabled = ui->editFormConst->palette().color(QPalette::Base);
	m_clrEditDisabled = ui->editFormConst->palette().color(QPalette::AlternateBase);

	setUIText();

	connect(ui->btnGradeCalc, SIGNAL(clicked(bool)), this, SLOT(doGrade()));
	connect(ui->btnOtherCalc, SIGNAL(clicked(bool)), this, SLOT(doFormConstOtherCalc()));
	connect(ui->btnPipeSizing, SIGNAL(clicked(bool)), this, SLOT(doPipeSizing()));
	connect(ui->btnAreaSizing, SIGNAL(clicked(bool)), this, SLOT(doAreaSizing()));
	connect(ui->btnHeadBoxDilution, SIGNAL(clicked(bool)), this, SLOT(doHeadBoxDilution()));
	connect(ui->btnCalcVats, SIGNAL(clicked(bool)), this, SLOT(doVatsCalc()));
	connect(ui->btnProductionCalc, SIGNAL(clicked(bool)), this, SLOT(doProductionCalc()));

	connect(ui->editCaliper, SIGNAL(textChanged(QString)), this, SLOT(en_changeCaliper()));
	connect(ui->editDensity, SIGNAL(textChanged(QString)), this, SLOT(en_changeDensity()));
	connect(ui->editSheetWidth, SIGNAL(textChanged(QString)), this, SLOT(en_changeSheetWidth()));
	connect(ui->editFormConst, SIGNAL(textChanged(QString)), this, SLOT(en_changeFormConst()));
	connect(ui->editYieldPerDay, SIGNAL(textChanged(QString)), this, SLOT(en_changeYieldPerDay()));
	connect(ui->editConsistency, SIGNAL(textChanged(QString)), this, SLOT(en_changeConsistency()));
	connect(ui->editNumberOfVats, SIGNAL(textChanged(QString)), this, SLOT(en_changeNumberOfVats()));
	connect(ui->editMachineEfficiency, SIGNAL(textChanged(QString)), this, SLOT(en_changeMachineEfficiency()));

	connect(ui->radioCylinderSize36, SIGNAL(toggled(bool)), this, SLOT(en_changeCylinderBoardType()));
	connect(ui->radioCylinderSize42, SIGNAL(toggled(bool)), this, SLOT(en_changeCylinderBoardType()));
	connect(ui->radioCylinderSize48, SIGNAL(toggled(bool)), this, SLOT(en_changeCylinderBoardType()));
	connect(ui->radioBTFoldingBox, SIGNAL(toggled(bool)), this, SLOT(en_changeCylinderBoardType()));
	connect(ui->radioBTTube, SIGNAL(toggled(bool)), this, SLOT(en_changeCylinderBoardType()));
	connect(ui->checkBoxOther, SIGNAL(toggled(bool)), this, SLOT(en_changeCylinderBoardType()));

	connect(ui->checkBoxAutoCalc, SIGNAL(toggled(bool)), this, SLOT(en_changeAutoCalc()));
}

CFormationCalcDialog::~CFormationCalcDialog()
{
	delete ui;
}

void CFormationCalcDialog::setUIText()
{
	if (m_bMetric) {
		ui->lblCaliper->setText(tr("Caliper (points):"));
		ui->lblDensity->setText(tr("Sheet Density (grams/point):"));
		ui->lblBasisWeight->setText(tr("Desired Basis Weight (grams/M Sq.):"));
		ui->lblSheetWidth->setText(tr("Sheet Trim Width (mm):"));
		ui->lblFormConst->setText(tr("Board Type Formation Constant:"));
		ui->lblYieldPerDay->setText(tr("Kilograms Per Day:"));
		ui->lblConsistency->setText(tr("Consistency (%):"));
		ui->lblWaterNeeded->setText(tr("Liters Per Minute:"));
		ui->lblWaterNeededPerVat->setText(tr("Per Vat:"));
		ui->lblMachineSpeed->setText(tr("Max Machine Speed (M/Min):"));
		ui->lblNumberOfVats->setText(tr("Number of Vats:"));
		ui->lblMachineEfficiency->setText(tr("Machine Efficiency (%):"));
	} else {
		ui->lblCaliper->setText(tr("Caliper (points):"));
		ui->lblDensity->setText(tr("Sheet Density (lbs/point):"));
		ui->lblBasisWeight->setText(tr("Desired Basis Weight (lbs/1K Sq.Ft):"));
		ui->lblSheetWidth->setText(tr("Sheet Trim Width (in):"));
		ui->lblFormConst->setText(tr("Board Type Formation Constant:"));
		ui->lblYieldPerDay->setText(tr("Short Tons Per Day:"));
		ui->lblConsistency->setText(tr("Consistency (%):"));
		ui->lblWaterNeeded->setText(tr("Gallons Per Minute:"));
		ui->lblWaterNeededPerVat->setText(tr("Per Vat:"));
		ui->lblMachineSpeed->setText(tr("Machine Speed (Ft/Min):"));
		ui->lblNumberOfVats->setText(tr("Number of Vats:"));
		ui->lblMachineEfficiency->setText(tr("Machine Efficiency (%):"));
	}
}

void CFormationCalcDialog::setMetric(bool bMetric)
{
	if (m_bChangingMetricMode) return;
	saveValues();
	m_bMetric = bMetric;
	setValues();
	setUIText();
	m_bChangingMetricMode = true;
	emit metricModeSelected(bMetric);
	m_bChangingMetricMode = false;
}

void CFormationCalcDialog::setCylinderBoardType(CYLINDER_SIZE_ENUM nCSE, BOARD_TYPE_ENUM nBTE, bool bOtherForm)
{
	m_bCylinderBoardTypeUpdateInProgress = true;

	m_DialogValues.m_nCylinderSize = nCSE;
	switch (nCSE) {
		case CSE_36in:
			ui->radioCylinderSize36->setChecked(true);
			break;
		case CSE_42in:
			ui->radioCylinderSize42->setChecked(true);
			break;
		case CSE_48in:
			ui->radioCylinderSize48->setChecked(true);
			break;
	}

	m_DialogValues.m_nBoardType = nBTE;
	switch (nBTE) {
		case BTE_FoldingBoxBoard:
			ui->radioBTFoldingBox->setChecked(true);
			break;
		case BTE_TubeBoard:
			ui->radioBTTube->setChecked(true);
			break;
	}

	m_DialogValues.m_bOtherFormConst = bOtherForm;
	ui->checkBoxOther->setChecked(bOtherForm);

	m_bCylinderBoardTypeUpdateInProgress = false;
	en_changeCylinderBoardType();		// Force update
}

// -----------------------------------------------------------------------------

bool CFormationCalcDialog::validateCaliper(double *pnCaliper, bool bPrompt)
{
	assert(pnCaliper != nullptr);
	bool bValid = true;
	*pnCaliper = ui->editCaliper->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Caliper"), tr("Caliper must be a proper numeric value."));
	} else if (bValid) {
		if (*pnCaliper <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Caliper"), tr("Caliper must be greater than ZERO!!"));
			}
		} else {
			if (m_bMetric) *pnCaliper = toImperial(*pnCaliper, CaliperConv);
		}
	}
	return bValid;
}

bool CFormationCalcDialog::validateDensity(double *pnDensity, bool bPrompt)
{
	assert(pnDensity != nullptr);
	bool bValid = true;
	*pnDensity = ui->editDensity->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Density"), tr("Density must be a proper numeric value."));
	} else if (bValid) {
		if (*pnDensity <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Density"), tr("Density must be greater than ZERO!!"));
			}
		} else {
			if (m_bMetric) *pnDensity = toImperial(*pnDensity, DensityConv);
		}
	}
	return bValid;
}

bool CFormationCalcDialog::validateSheetWidth(double *pnSheetWidth, bool bPrompt)
{
	assert(pnSheetWidth != nullptr);
	bool bValid = true;
	*pnSheetWidth = ui->editSheetWidth->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Sheet Width"), tr("Sheet Width must be a proper numeric value."));
	} else if (bValid) {
		if (*pnSheetWidth <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Sheet Width"), tr("Sheet Width must be greater than ZERO!!"));
			}
		} else {
			if (m_bMetric) *pnSheetWidth = toImperial(*pnSheetWidth, SheetWidthConv);
		}
	}
	return bValid;
}

bool CFormationCalcDialog::validateFormConst(double *pnFormConst, bool bPrompt)
{
	assert(pnFormConst != nullptr);
	bool bValid = true;
	*pnFormConst = ui->editFormConst->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Formation Constant"), tr("Formation Constant must be a proper numeric value."));
	} else if (bValid) {
		if (*pnFormConst <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Formation Constant"), tr("Formation Constant must be greater than ZERO!!"));
			}
		} else {
			if (m_bMetric) *pnFormConst = toImperial(*pnFormConst, FormConv);
		}
	}
	return bValid;
}

bool CFormationCalcDialog::validateYieldPerDay(double *pnYieldPerDay, bool bPrompt)
{
	assert(pnYieldPerDay != nullptr);
	bool bValid = true;
	*pnYieldPerDay = ui->editYieldPerDay->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		if (m_bMetric) {
			QMessageBox::critical(this, tr("Invalid Kilograms Per Day"), tr("Kilograms Per Day must be a proper numeric value."));
		} else {
			QMessageBox::critical(this, tr("Invalid Tons Per Day"), tr("Tons Per Day must be a proper numeric value."));
		}
	} else if (bValid) {
		if (m_bMetric) *pnYieldPerDay = toImperial(*pnYieldPerDay, YieldPerDayConv);
	}
	return bValid;
}

bool CFormationCalcDialog::validateConsistency(double *pnConsistency, bool bPrompt)
{
	assert(pnConsistency != nullptr);
	bool bValid = true;
	*pnConsistency = ui->editConsistency->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Consistency"), tr("Consistency must be a proper numeric value."));
	} else if (bValid) {
		if (*pnConsistency <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Consistency"), tr("Consistency must be greater than ZERO!!"));
			}
		} else {
			*pnConsistency = *pnConsistency / 100.0;
		}
	}
	return bValid;
}

bool CFormationCalcDialog::validateNumberOfVats(int *pnNumberOfVats, bool bPrompt)
{
	assert(pnNumberOfVats != nullptr);
	bool bValid = true;
	*pnNumberOfVats = ui->editNumberOfVats->text().toInt(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Number of Vats"), tr("Number of Vats must be a proper integer value."));
	} else if (bValid) {
		if (*pnNumberOfVats <= 0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Number of Vats"), tr("Number of Vats must be great than ZERO!!"));
			}
		}
	}
	return bValid;
}

// -----------------------------------------------------------------------------

int CFormationCalcDialog::exec()
{
	// Set AutoCalc flag before values so we'll do AutoCalc while setting values:
	ui->checkBoxAutoCalc->setChecked(m_DialogValues.m_bAutoCalc);

	setValues();

	ui->editCaliper->setFocus();

	return QDialog::exec();
}

void CFormationCalcDialog::accept()
{
	double nCaliper = 0.0;
	double nDensity = 0.0;
	double nSheetWidth = 0.0;
	double nFormConst = 0.0;
	double nYieldPerDay = 0.0;
	double nConsistency = 0.0;
	int nNumberOfVats = 0;
	bool bAutoCalc = (ui->checkBoxAutoCalc->isChecked());

	if (!validateCaliper(&nCaliper, bAutoCalc) && bAutoCalc) return;
	if (!validateDensity(&nDensity, bAutoCalc) && bAutoCalc) return;
	if (!validateSheetWidth(&nSheetWidth, bAutoCalc) && bAutoCalc) return;
	if (!validateFormConst(&nFormConst, bAutoCalc) && bAutoCalc) return;
	if (!validateYieldPerDay(&nYieldPerDay, true)) return;
	if (!validateConsistency(&nConsistency, true)) return;
	if (!validateNumberOfVats(&nNumberOfVats, true)) return;
	if (!calc1()) return;
	if (!calc2()) return;

	m_DialogValues.m_nCaliper = nCaliper;
	m_DialogValues.m_nDensity = nDensity;
	m_DialogValues.m_nSheetWidth = nSheetWidth;
	m_DialogValues.m_nFormConst = nFormConst;
	m_DialogValues.m_nYieldPerDay = nYieldPerDay;
	m_DialogValues.m_nConsistency = nConsistency;
	m_DialogValues.m_nNumberOfVats = nNumberOfVats;
	QDialog::accept();
}

void CFormationCalcDialog::reject()
{
	saveValues();
	QDialog::reject();
}

void CFormationCalcDialog::saveValues()
{
	validateCaliper(&m_DialogValues.m_nCaliper, false);
	validateDensity(&m_DialogValues.m_nDensity, false);
	validateSheetWidth(&m_DialogValues.m_nSheetWidth, false);
	validateFormConst(&m_DialogValues.m_nFormConst, false);
	validateYieldPerDay(&m_DialogValues.m_nYieldPerDay, false);
	validateConsistency(&m_DialogValues.m_nConsistency, false);
	validateNumberOfVats(&m_DialogValues.m_nNumberOfVats, false);
	calc1(false);
	calc2(false);
}

void CFormationCalcDialog::setValues()
{
	ui->editCaliper->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nCaliper, CaliperConv) : m_DialogValues.m_nCaliper));
	ui->editDensity->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nDensity, DensityConv) : m_DialogValues.m_nDensity));
	ui->editSheetWidth->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nSheetWidth, SheetWidthConv) : m_DialogValues.m_nSheetWidth));
	ui->editFormConst->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nFormConst, FormConv) : m_DialogValues.m_nFormConst));
	ui->editYieldPerDay->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nYieldPerDay, YieldPerDayConv) : m_DialogValues.m_nYieldPerDay));
	ui->editConsistency->setText(QString().setNum(m_DialogValues.m_nConsistency * 100.0));
	ui->editNumberOfVats->setText(QString().setNum(m_DialogValues.m_nNumberOfVats));
	ui->editMachineSpeed->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nMachineSpeed, MachineSpeedConv) : m_DialogValues.m_nMachineSpeed));
	ui->editMachineEfficiency->setText(QString().setNum(m_DialogValues.m_nMachineEfficiency * 100.0));
	setCylinderBoardType(m_DialogValues.m_nCylinderSize, m_DialogValues.m_nBoardType, m_DialogValues.m_bOtherFormConst);
	calc1(false);
	calc2(false);
}

// -----------------------------------------------------------------------------

bool CFormationCalcDialog::calc1(bool bPrompt)
{
	double nCaliper = 0.0;
	double nDensity = 0.0;
	double nBasisWeight = 0.0;
	double nSheetWidth = 0.0;
	double nFormConst = 0.0;
	double nYieldPerDay = 0.0;
	double nMachineSpeed = 0.0;
	int nNumberOfVats = 0;
	bool bAutoCalc = (ui->checkBoxAutoCalc->isChecked());

	if ((!validateCaliper(&nCaliper, bPrompt && bAutoCalc)) ||
		(!validateDensity(&nDensity, bPrompt && bAutoCalc))) {
		ui->editBasisWeight->setText(QString());
		if (bAutoCalc) return false;
	} else {
		nBasisWeight = nCaliper * nDensity;
		ui->editBasisWeight->setText(QString().setNum(m_bMetric ? toMetric(nBasisWeight, BasisWeightConv) : nBasisWeight));
	}
	m_DialogValues.m_nBasisWeight = nBasisWeight;

	if (bAutoCalc) {
		if ((!validateSheetWidth(&nSheetWidth, bPrompt)) ||
			(!validateFormConst(&nFormConst, bPrompt)) ||
			(!validateNumberOfVats(&nNumberOfVats, bPrompt)) ||
			(m_DialogValues.m_nMachineEfficiency <= 0.0)) {
			ui->editYieldPerDay->setText(QString());
			ui->editMachineSpeed->setText(QString());
			return false;
		} else {
			nYieldPerDay = (nSheetWidth*nNumberOfVats)*nFormConst*m_DialogValues.m_nMachineEfficiency;
			ui->editYieldPerDay->setText(QString().setNum(m_bMetric ? toMetric(nYieldPerDay, YieldPerDayConv) : nYieldPerDay));
			// Don't set m_DialogValues.m_nYieldPerDay here since it's an edit box that will get returned in accept()
		}
	} else {
		if ((!validateSheetWidth(&nSheetWidth, bPrompt)) ||
			(nBasisWeight <= 0.0) ||
			(!validateYieldPerDay(&nYieldPerDay, bPrompt)) ||
			(m_DialogValues.m_nMachineEfficiency <= 0.0)) {
			ui->editMachineSpeed->setText(QString());
			return false;
		}
	}

	nMachineSpeed = ((nYieldPerDay/hours_per_day)/min_per_hour)*lbs_per_ton;			// Find lbs per min
	nMachineSpeed = (nMachineSpeed/nBasisWeight)*(1000.0/(nSheetWidth/in_per_ft));		// Find ft per min
	nMachineSpeed = nMachineSpeed/m_DialogValues.m_nMachineEfficiency;
	ui->editMachineSpeed->setText(QString().setNum(m_bMetric ? toMetric(nMachineSpeed, MachineSpeedConv) : nMachineSpeed));
	m_DialogValues.m_nMachineSpeed = nMachineSpeed;

	return true;
}

bool CFormationCalcDialog::calc2(bool bPrompt)
{
	double nYieldPerDay = 0.0;
	double nConsistency = 0.0;
	double nWaterNeeded = 0.0;
	double nWaterNeededPerVat = 0.0;
	int nNumberOfVats = 0;

	if ((!validateYieldPerDay(&nYieldPerDay, bPrompt)) ||
		(!validateConsistency(&nConsistency, bPrompt)) ||
		(!validateNumberOfVats(&nNumberOfVats, bPrompt)) ||
		(m_DialogValues.m_nMachineEfficiency <= 0.0)) {
		ui->editWaterNeeded->setText(QString());
		ui->editWaterNeededPerVat->setText(QString());
		return false;
	} else {
		nWaterNeeded = ((nYieldPerDay*lbs_per_ton)/(hours_per_day*min_per_hour))/((nConsistency*(5.0/6.0)*10000.0)/1000.0);
		ui->editWaterNeeded->setText(QString().setNum(m_bMetric ? toMetric(nWaterNeeded, WaterNeededConv) : nWaterNeeded));
		if (nNumberOfVats) {
			nWaterNeededPerVat = nWaterNeeded / static_cast<double>(nNumberOfVats);
		} else {
			nWaterNeededPerVat = nWaterNeeded;
		}
		ui->editWaterNeededPerVat->setText(QString().setNum(m_bMetric ? toMetric(nWaterNeededPerVat, WaterNeededConv) : nWaterNeededPerVat));
	}
	m_DialogValues.m_nWaterNeeded = nWaterNeeded;
	m_DialogValues.m_nWaterNeededPerVat = nWaterNeededPerVat;

	return true;
}

bool CFormationCalcDialog::doVatsCalcValid()
{
	double nSheetWidth = 0.0;
	double nFormConst = 0.0;
	double nYieldPerDay = 0.0;

	if ((!validateSheetWidth(&nSheetWidth, false)) ||
		(!validateFormConst(&nFormConst, false)) ||
		(!validateYieldPerDay(&nYieldPerDay, false)) ||
		(m_DialogValues.m_nMachineEfficiency <= 0.0)) {
		return false;
	}
	return true;
}

void CFormationCalcDialog::doGrade()
{
	CGradeCalcDialog dlgGrade(false, m_bMetric, this);
	validateCaliper(&dlgGrade.m_DialogValues.m_nCaliper, false);
	validateDensity(&dlgGrade.m_DialogValues.m_nDensity, false);
	dlgGrade.m_DialogValues.m_nBasisWeight = m_DialogValues.m_nBasisWeight;		// Use our precomputed value for BasisWeight
	int nResult = dlgGrade.exec();
	if (nResult == QDialog::Accepted) {
		ui->editCaliper->setText(QString().setNum(m_bMetric ? toMetric(dlgGrade.m_DialogValues.m_nCaliper, CaliperConv) : dlgGrade.m_DialogValues.m_nCaliper));
		ui->editDensity->setText(QString().setNum(m_bMetric ? toMetric(dlgGrade.m_DialogValues.m_nDensity, DensityConv) : dlgGrade.m_DialogValues.m_nDensity));
		// Note: Basis Weight will automatically recalculate itself
	}
}

void CFormationCalcDialog::doFormConstOtherCalc()
{
	CFormConstDialog dlgFormConst(false, m_bMetric, this);
	validateFormConst(&dlgFormConst.m_DialogValues.m_nFormConst, false);
	dlgFormConst.m_DialogValues.m_nMachineEfficiency = m_DialogValues.m_nMachineEfficiency;
	double nSheetWidth;
	int nNumberOfVats;
	double nYieldPerDay;
	validateSheetWidth(&nSheetWidth, false);
	dlgFormConst.m_DialogValues.m_nSheetWidth = nSheetWidth;
	validateNumberOfVats(&nNumberOfVats, false);
	dlgFormConst.m_DialogValues.m_nNumberOfVats = nNumberOfVats;
	validateYieldPerDay(&nYieldPerDay, false);
	dlgFormConst.m_DialogValues.m_nYieldPerDay = nYieldPerDay;
	int nResult = dlgFormConst.exec();
	if (nResult == QDialog::Accepted) {
		ui->editFormConst->setText(QString().setNum(m_bMetric ? toMetric(dlgFormConst.m_DialogValues.m_nFormConst, FormConv) : dlgFormConst.m_DialogValues.m_nFormConst));
//			if (dlgFormConst.m_DialogValues.m_nMachineEfficiency != m_DialogValues.m_nMachineEfficiency) {
//				m_DialogValues.m_nMachineEfficiency = dlgFormConst.m_DialogValues.m_nMachineEfficiency;
//				ui->editMachineEfficiency->setText(QString().setNum(m_DialogValues.m_nMachineEfficiency * 100.0));
//			}
//			if (dlgFormConst.m_DialogValues.m_nSheetWidth != nSheetWidth) {
//				ui->editSheetWidth->setText(QString().setNum(m_bMetric ? toMetric(dlgFormConst.m_DialogValues.m_nSheetWidth, SheetWidthConv) : dlgFormConst.m_DialogValues.m_nSheetWidth));
//			}
//			if (dlgFormConst.m_DialogValues.m_nNumberOfVats != nNumberOfVats) {
//				ui->editNumberOfVats->setText(QString().setNum(dlgFormConst.m_DialogValues.m_nNumberOfVats));
//			}
		ui->checkBoxOther->setChecked(true);
	}
}

void CFormationCalcDialog::doPipeSizing()
{
	CPipeFlowCalcDialog dlgPipeFlow(true, m_bMetric, this);
	dlgPipeFlow.m_DialogValues.m_nWaterNeeded = m_DialogValues.m_nWaterNeededPerVat;
	int nResult = dlgPipeFlow.exec();
	if (nResult == QDialog::Accepted) {
		assert(!g_pMyMainWindow.isNull());
		g_pMyMainWindow->insertFlowPipeText(dlgPipeFlow.m_DialogValues);
	}
}

void CFormationCalcDialog::doAreaSizing()
{
	CAreaFlowCalcDialog dlgAreaFlow(true, m_bMetric, this);
	dlgAreaFlow.m_DialogValues.m_nWaterNeeded = m_DialogValues.m_nWaterNeededPerVat;
	int nResult = dlgAreaFlow.exec();
	if (nResult == QDialog::Accepted) {
		assert(!g_pMyMainWindow.isNull());
		g_pMyMainWindow->insertFlowAreaText(dlgAreaFlow.m_DialogValues);
	}
}

void CFormationCalcDialog::doHeadBoxDilution()
{
	CHeadBoxDilutionDialog dlgHeadBox(false, m_bMetric, this);
	validateYieldPerDay(&dlgHeadBox.m_DialogValues.m_nYieldPerDay, false);
	validateConsistency(&dlgHeadBox.m_DialogValues.m_nPresentConsistency, false);
	int nResult = dlgHeadBox.exec();
	if (nResult == QDialog::Accepted) {
		ui->editConsistency->setText(QString().setNum(dlgHeadBox.m_DialogValues.m_nNewConsistency * 100.0));
		// Note: WaterNeeded will automatically recalculate itself
	}
}

void CFormationCalcDialog::doVatsCalc()
{
	double nSheetWidth = 0.0;
	double nFormConst = 0.0;
	if (!validateSheetWidth(&nSheetWidth, false)) return;
	if (!validateFormConst(&nFormConst, false)) return;
	if (m_DialogValues.m_nMachineEfficiency <= 0.0) return;

	CYieldPerDayDialog dlgYieldPerDay(m_bMetric, this);
	validateYieldPerDay(&dlgYieldPerDay.m_DialogValues.m_nYieldPerDay, false);
	int nResult = dlgYieldPerDay.exec();
	if (nResult == QDialog::Accepted) {
		int nNumberOfVats = static_cast<int>(ceil(((dlgYieldPerDay.m_DialogValues.m_nYieldPerDay/m_DialogValues.m_nMachineEfficiency)/nFormConst)/nSheetWidth));
		ui->editNumberOfVats->setText(QString().setNum(nNumberOfVats));
	}
}

void CFormationCalcDialog::doProductionCalc()
{
	double nOrigFormConst;
	validateFormConst(&nOrigFormConst, false);
	CProductionCalcDialog dlgProd(false, m_bMetric, this);
	validateSheetWidth(&dlgProd.m_DialogValues.m_nSheetWidth, false);
	dlgProd.m_DialogValues.m_nMachineSpeed = m_DialogValues.m_nMachineSpeed;
	dlgProd.m_DialogValues.m_nMachineEfficiency = m_DialogValues.m_nMachineEfficiency;
	validateCaliper(&dlgProd.m_DialogValues.m_nCaliper, false);
	validateDensity(&dlgProd.m_DialogValues.m_nDensity, false);
	int nResult = dlgProd.exec();
	if (nResult == QDialog::Accepted) {
		bool bAutoCalcSave = ui->checkBoxAutoCalc->isChecked();
		ui->checkBoxAutoCalc->setChecked(false);
		ui->editSheetWidth->setText(QString().setNum(m_bMetric ? toMetric(dlgProd.m_DialogValues.m_nSheetWidth, SheetWidthConv) : dlgProd.m_DialogValues.m_nSheetWidth));
		m_DialogValues.m_nMachineEfficiency = dlgProd.m_DialogValues.m_nMachineEfficiency;
		ui->editMachineEfficiency->setText(QString().setNum(m_DialogValues.m_nMachineEfficiency * 100.0));
		ui->editCaliper->setText(QString().setNum(m_bMetric ? toMetric(dlgProd.m_DialogValues.m_nCaliper, CaliperConv) : dlgProd.m_DialogValues.m_nCaliper));
		ui->editDensity->setText(QString().setNum(m_bMetric ? toMetric(dlgProd.m_DialogValues.m_nDensity, DensityConv) : dlgProd.m_DialogValues.m_nDensity));
		// Note: Basis Weight will automatically recalculate itself
		ui->editYieldPerDay->setText(QString().setNum(m_bMetric ? toMetric(dlgProd.m_DialogValues.m_nYieldPerDay, YieldPerDayConv) : dlgProd.m_DialogValues.m_nYieldPerDay));
		// Note: Machine Speed will automatically recalculate itself
		double nYieldPerDay;
		validateYieldPerDay(&nYieldPerDay, false);
		if (m_DialogValues.m_nMachineEfficiency) nYieldPerDay /= m_DialogValues.m_nMachineEfficiency;
		double nSheetWidth;
		int nNumberOfVats;
		double nFormConst = 0.0;
		bool bFormConstValid = false;
		bool bFormDelta = true;
		if ((validateSheetWidth(&nSheetWidth, false)) &&
			(validateNumberOfVats(&nNumberOfVats, false))) {
			nFormConst = (nYieldPerDay/nNumberOfVats)/nSheetWidth;
			bFormConstValid = true;
			bFormDelta = (fabs(nFormConst-nOrigFormConst) > 0.0001);
		}
		if (bFormConstValid) {
			if (bFormDelta && QMessageBox::question(this, tr("Recalculation Request"), tr("Do you wish to recalc the forming rate?")) == QMessageBox::Yes) {
				ui->checkBoxOther->setChecked(true);
				ui->editFormConst->setText(QString().setNum(m_bMetric ? toMetric(nFormConst, FormConv) : nFormConst));
				if (bAutoCalcSave) {
					ui->checkBoxAutoCalc->setChecked(true);
				}
			} else {
				if (bAutoCalcSave && !bFormDelta) {
					ui->checkBoxAutoCalc->setChecked(true);
				}
			}
		}
	}
}

// -----------------------------------------------------------------------------

void CFormationCalcDialog::en_changeCaliper()
{
	bool bCalc1 = calc1(false);
	m_pSaveApplyButton->setEnabled(calc2(false) && bCalc1);		// Reverse short-circuiting so that we call both calc functions
}

void CFormationCalcDialog::en_changeDensity()
{
	bool bCalc1 = calc1(false);
	m_pSaveApplyButton->setEnabled(calc2(false) && bCalc1);		// Reverse short-circuiting so that we call both calc functions
}

void CFormationCalcDialog::en_changeSheetWidth()
{
	ui->btnCalcVats->setEnabled(doVatsCalcValid());
	bool bCalc1 = calc1(false);
	m_pSaveApplyButton->setEnabled(calc2(false) && bCalc1);		// Reverse short-circuiting so that we call both calc functions
}

void CFormationCalcDialog::en_changeFormConst()
{
	ui->btnCalcVats->setEnabled(doVatsCalcValid());
	bool bCalc1 = calc1(false);
	m_pSaveApplyButton->setEnabled(calc2(false) && bCalc1);		// Reverse short-circuiting so that we call both calc functions
}

void CFormationCalcDialog::en_changeYieldPerDay()
{
	bool bCalc1 = calc1(false);
	m_pSaveApplyButton->setEnabled(calc2(false) && bCalc1);		// Reverse short-circuiting so that we call both calc functions
}

void CFormationCalcDialog::en_changeConsistency()
{
	bool bCalc1 = calc1(false);
	m_pSaveApplyButton->setEnabled(calc2(false) && bCalc1);		// Reverse short-circuiting so that we call both calc functions
}

void CFormationCalcDialog::en_changeNumberOfVats()
{
	bool bCalc1 = calc1(false);
	m_pSaveApplyButton->setEnabled(calc2(false) && bCalc1);		// Reverse short-circuiting so that we call both calc functions
}

void CFormationCalcDialog::en_changeMachineEfficiency()
{
	ui->btnCalcVats->setEnabled(doVatsCalcValid());
	bool bCalc1 = calc1(false);
	m_pSaveApplyButton->setEnabled(calc2(false) && bCalc1);		// Reverse short-circuiting so that we call both calc functions
}

void CFormationCalcDialog::en_changeCylinderBoardType()
{
	if (ui->checkBoxOther->isChecked()) {
		m_DialogValues.m_bOtherFormConst = true;
		ui->groupBoxCylinderSize->setDisabled(true);
		ui->groupBoxBoardType->setDisabled(true);
		ui->editFormConst->setReadOnly(false);
		ui->btnOtherCalc->setEnabled(true);
		QPalette pal = ui->editFormConst->palette();
		pal.setColor(QPalette::Base, m_clrEditEnabled);
		ui->editFormConst->setPalette(pal);
	} else {
		m_DialogValues.m_bOtherFormConst = false;
		ui->groupBoxCylinderSize->setDisabled(false);
		ui->groupBoxBoardType->setDisabled(false);
		ui->editFormConst->setReadOnly(true);
		ui->btnOtherCalc->setEnabled(false);
		QPalette pal = ui->editFormConst->palette();
		pal.setColor(QPalette::Base, m_clrEditDisabled);
		ui->editFormConst->setPalette(pal);

		if (ui->radioCylinderSize36->isChecked()) {
			m_DialogValues.m_nCylinderSize = CSE_36in;
		} else if (ui->radioCylinderSize42->isChecked()) {
			m_DialogValues.m_nCylinderSize = CSE_42in;
		} else if (ui->radioCylinderSize48->isChecked()) {
			m_DialogValues.m_nCylinderSize = CSE_48in;
		}

		if (ui->radioBTFoldingBox->isChecked()) {
			m_DialogValues.m_nBoardType = BTE_FoldingBoxBoard;
		} else if (ui->radioBTTube->isChecked()) {
			m_DialogValues.m_nBoardType = BTE_TubeBoard;
		}

		if (m_bCylinderBoardTypeUpdateInProgress) return;

		double nFormConst = g_CylinderSizeConst[m_DialogValues.m_nCylinderSize] * g_BoardTypeConst[m_DialogValues.m_nBoardType];
		ui->editFormConst->setText(QString().setNum(m_bMetric ? toMetric(nFormConst, FormConv) : nFormConst));
	}
}

void CFormationCalcDialog::en_changeAutoCalc()
{
	bool bAutoCalc = (ui->checkBoxAutoCalc->isChecked());

	m_DialogValues.m_bAutoCalc = bAutoCalc;
	bool bCalc1 = calc1(false);
	m_pSaveApplyButton->setEnabled(calc2(false) && bCalc1);		// Reverse short-circuiting so that we call both calc functions
	ui->editYieldPerDay->setReadOnly(bAutoCalc);
	QPalette pal = ui->editYieldPerDay->palette();
	pal.setColor(QPalette::Base, bAutoCalc ? m_clrEditDisabled : m_clrEditEnabled);
	ui->editYieldPerDay->setPalette(pal);
}

// -----------------------------------------------------------------------------

void CFormationCalcDialog::showHelp()
{
	if (!g_pMyMainWindow.isNull()) {
		g_pMyMainWindow->showHelp(CHelpDialog::helpDocument(CHelpDialog::HELPDOC_ABOUT));
	}
}

// =============================================================================
