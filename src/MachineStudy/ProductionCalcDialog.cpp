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

#include "ProductionCalcDialog.h"
#include "ui_ProductionCalcDialog.h"

#include "conversion.h"

#include "GradeCalcDialog.h"
#include "MainWindow.h"
#include "HelpDialog.h"

#include <QPushButton>
#include <QMessageBox>

// =============================================================================

CProductionCalcDialog::CProductionCalcDialog(bool bStandAlone, bool bMetric, QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
	m_bStandAlone(bStandAlone),
	m_bMetric(bMetric),
	m_pCalcButton(nullptr),
	m_pSaveApplyButton(nullptr),
	ui(new Ui::CProductionCalcDialog)
{
	ui->setupUi(this);

	installEventFilter(&m_enterEater);

	m_pCalcButton = ui->buttonBox->addButton(tr("&Calc"), QDialogButtonBox::ActionRole);

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
	m_pCalcButton->setVisible(false);		// Leave invisible since we are currently doing automatic calculations
	connect(m_pSaveApplyButton, SIGNAL(clicked(bool)), this, SLOT(accept()));

	setUIText();

	connect(m_pCalcButton, SIGNAL(clicked(bool)), this, SLOT(calc()));		// NOTE: Don't pass bool argument on as they mean different things!

	connect(ui->btnGradeCalc, SIGNAL(clicked(bool)), this, SLOT(doGrade()));

	connect(ui->editSheetWidth, SIGNAL(textChanged(QString)), this, SLOT(en_changeSheetWidth()));
	connect(ui->editMachineSpeed, SIGNAL(textChanged(QString)), this, SLOT(en_changeMachineSpeed()));
	connect(ui->editMachineEfficiency, SIGNAL(textChanged(QString)), this, SLOT(en_changeMachineEfficiency()));
	connect(ui->editCaliper, SIGNAL(textChanged(QString)), this, SLOT(en_changeCaliper()));
	connect(ui->editDensity, SIGNAL(textChanged(QString)), this, SLOT(en_changeDensity()));
}

CProductionCalcDialog::~CProductionCalcDialog()
{
	delete ui;
}

// -----------------------------------------------------------------------------

void CProductionCalcDialog::setUIText()
{
	if (m_bMetric) {
		ui->lblSheetWidth->setText(tr("Sheet Width After Deckling (mm):"));
		ui->lblMachineSpeed->setText(tr("Machine Speed (Meters/Min):"));
		ui->lblMachineEfficiency->setText(tr("Machine Efficiency (%):"));
		ui->lblCaliper->setText(tr("Caliper (points):"));
		ui->lblDensity->setText(tr("Sheet Density (grams/point):"));
		ui->lblBasisWeight->setText(tr("Desired Basis Weight (grams/M Sq.):"));
		ui->lblYieldPerDay->setText(tr("Kilograms Per Day:"));
	} else {
		ui->lblSheetWidth->setText(tr("Sheet Width After Deckling (in):"));
		ui->lblMachineSpeed->setText(tr("Machine Speed (Ft/Min):"));
		ui->lblMachineEfficiency->setText(tr("Machine Efficiency (%):"));
		ui->lblCaliper->setText(tr("Caliper (points):"));
		ui->lblDensity->setText(tr("Sheet Density (lbs/point):"));
		ui->lblBasisWeight->setText(tr("Desired Basis Weight (lbs/1K Sq.Ft):"));
		ui->lblYieldPerDay->setText(tr("Tons Per Day:"));
	}
}

void CProductionCalcDialog::setMetric(bool bMetric)
{
	m_bMetric = bMetric;
	setUIText();
}

// -----------------------------------------------------------------------------

bool CProductionCalcDialog::validateSheetWidth(double *pnSheetWidth, bool bPrompt)
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

bool CProductionCalcDialog::validateMachineSpeed(double *pnMachineSpeed, bool bPrompt)
{
	assert(pnMachineSpeed != nullptr);
	bool bValid = true;
	*pnMachineSpeed = ui->editMachineSpeed->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Machine Speed"), tr("Machine Speed must be a proper numeric value."));
	} else if (bValid) {
		if (*pnMachineSpeed <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Machine Speed"), tr("Machine Speed must be greater than ZERO!!"));
			}
		} else {
			if (m_bMetric) *pnMachineSpeed = toImperial(*pnMachineSpeed, MachineSpeedConv);
		}
	}
	return bValid;
}

bool CProductionCalcDialog::validateMachineEfficiency(double *pnMachineEfficiency, bool bPrompt)
{
	assert(pnMachineEfficiency != nullptr);
	bool bValid = true;
	*pnMachineEfficiency = ui->editMachineEfficiency->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Machine Efficiency"), tr("Machine Efficiency must be a proper numeric value."));
	} else if (bValid) {
		if (*pnMachineEfficiency <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Machine Efficiency"), tr("Machine Efficiency must be greater than ZERO!!"));
			}
		} else {
			*pnMachineEfficiency = *pnMachineEfficiency / 100.0;
		}
	}
	return bValid;
}

bool CProductionCalcDialog::validateCaliper(double *pnCaliper, bool bPrompt)
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

bool CProductionCalcDialog::validateDensity(double *pnDensity, bool bPrompt)
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

// -----------------------------------------------------------------------------

int CProductionCalcDialog::exec()
{
	ui->editSheetWidth->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nSheetWidth, SheetWidthConv) : m_DialogValues.m_nSheetWidth));
	ui->editMachineSpeed->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nMachineSpeed, MachineSpeedConv) : m_DialogValues.m_nMachineSpeed));
	ui->editMachineEfficiency->setText(QString().setNum(m_DialogValues.m_nMachineEfficiency * 100.0));
	ui->editCaliper->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nCaliper, CaliperConv) : m_DialogValues.m_nCaliper));
	ui->editDensity->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nDensity, DensityConv) : m_DialogValues.m_nDensity));

	ui->editSheetWidth->setFocus();

	return QDialog::exec();
}

void CProductionCalcDialog::accept()
{
	double nSheetWidth = 0.0;
	double nMachineSpeed = 0.0;
	double nMachineEfficiency = 1.0;
	double nCaliper = 0.0;
	double nDensity = 0.0;

	if (!validateSheetWidth(&nSheetWidth, true)) return;
	if (!validateMachineSpeed(&nMachineSpeed, true)) return;
	if (!validateMachineEfficiency(&nMachineEfficiency, true)) return;
	if (!validateCaliper(&nCaliper, true)) return;
	if (!validateDensity(&nDensity, true)) return;
	if (!calc()) return;

	m_DialogValues.m_nSheetWidth = nSheetWidth;
	m_DialogValues.m_nMachineSpeed = nMachineSpeed;
	m_DialogValues.m_nMachineEfficiency = nMachineEfficiency;
	m_DialogValues.m_nCaliper = nCaliper;
	m_DialogValues.m_nDensity = nDensity;
	QDialog::accept();
}

// -----------------------------------------------------------------------------

bool CProductionCalcDialog::calc(bool bPrompt)
{
	double nSheetWidth = 0.0;
	double nMachineSpeed = 0.0;
	double nMachineEfficiency = 1.0;
	double nCaliper = 0.0;
	double nDensity = 0.0;

	// Always calculate/update BasisWeight if we can, but
	//		save prompting to later:
	if ((!validateCaliper(&nCaliper, false)) ||
		(!validateDensity(&nDensity, false))) {
		ui->editBasisWeight->setText(QString());
	} else {
		m_DialogValues.m_nBasisWeight = nCaliper * nDensity;
		ui->editBasisWeight->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nBasisWeight, BasisWeightConv) : m_DialogValues.m_nBasisWeight));
	}

	if ((!validateSheetWidth(&nSheetWidth, bPrompt)) ||
		(!validateMachineSpeed(&nMachineSpeed, bPrompt)) ||
		(!validateMachineEfficiency(&nMachineEfficiency, bPrompt)) ||
		(!validateCaliper(&nCaliper, bPrompt)) ||
		(!validateDensity(&nDensity, bPrompt))) {
		ui->editYieldPerDay->setText(QString());
		m_pCalcButton->setEnabled(false);
		return false;
	}

	m_DialogValues.m_nYieldPerDay = (((nMachineSpeed/(1000.0/(nSheetWidth/in_per_ft)))*
										m_DialogValues.m_nBasisWeight*min_per_hour)/lbs_per_ton)*
										nMachineEfficiency*hours_per_day;
	ui->editYieldPerDay->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nYieldPerDay, YieldPerDayConv) : m_DialogValues.m_nYieldPerDay));
	m_pCalcButton->setEnabled(true);
	return true;
}

void CProductionCalcDialog::doGrade()
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

// -----------------------------------------------------------------------------

void CProductionCalcDialog::en_changeSheetWidth()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

void CProductionCalcDialog::en_changeMachineSpeed()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

void CProductionCalcDialog::en_changeMachineEfficiency()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

void CProductionCalcDialog::en_changeCaliper()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

void CProductionCalcDialog::en_changeDensity()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

// -----------------------------------------------------------------------------

void CProductionCalcDialog::showHelp()
{
	if (!g_pMyMainWindow.isNull()) {
		g_pMyMainWindow->showHelp(CHelpDialog::helpDocument(CHelpDialog::HELPDOC_OTHER_HELPS));
	}
}

// =============================================================================

