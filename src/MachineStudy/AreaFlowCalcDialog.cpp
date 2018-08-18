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

#include "AreaFlowCalcDialog.h"
#include "ui_AreaFlowCalcDialog.h"

#include "conversion.h"

#include "MainWindow.h"
#include "HelpDialog.h"

#include <QPushButton>
#include <QMessageBox>

// =============================================================================

CAreaFlowCalcDialog::CAreaFlowCalcDialog(bool bStandAlone, bool bMetric, QWidget *parent) :
	QDialog(parent),
	m_bStandAlone(bStandAlone),
	m_bMetric(bMetric),
	m_pSaveApplyButton(nullptr),
	ui(new Ui::CAreaFlowCalcDialog)
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

	setUIText();

	connect(ui->btnCalcVelocity, SIGNAL(clicked(bool)), this, SLOT(calcVelocity()));
	connect(ui->btnCalcWaterNeeded, SIGNAL(clicked(bool)), this, SLOT(calcWaterNeeded()));
	connect(ui->btnCalcCrossSectArea, SIGNAL(clicked(bool)), this, SLOT(calcCrossSectArea()));

	connect(ui->editVelocity, SIGNAL(textChanged(QString)), this, SLOT(en_changeVelocity()));
	connect(ui->editWaterNeeded, SIGNAL(textChanged(QString)), this, SLOT(en_changeWaterNeeded()));
	connect(ui->editCrossSectArea, SIGNAL(textChanged(QString)), this, SLOT(en_changeCrossSectArea()));
}

CAreaFlowCalcDialog::~CAreaFlowCalcDialog()
{
	delete ui;
}

// -----------------------------------------------------------------------------

void CAreaFlowCalcDialog::setUIText()
{
	if (m_bMetric) {
		ui->lblVelocity->setText(tr("Velocity (M/Sec):"));
		ui->lblWaterNeeded->setText(tr("Liters Per Minute:"));
		ui->lblCrossSectArea->setText(tr("Cross Sectional Area (Sq.mm):"));
	} else {
		ui->lblVelocity->setText(tr("Velocity (Ft/Sec):"));
		ui->lblWaterNeeded->setText(tr("Gallons Per Minute:"));
		ui->lblCrossSectArea->setText(tr("Cross Sectional Area (Sq.In):"));
	}
}

void CAreaFlowCalcDialog::setMetric(bool bMetric)
{
	m_bMetric = bMetric;
	setUIText();
}

// -----------------------------------------------------------------------------

bool CAreaFlowCalcDialog::validateVelocity(double *pnVelocity, bool bPrompt)
{
	assert(pnVelocity != nullptr);
	bool bValid = true;
	*pnVelocity = ui->editVelocity->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Velocity"), tr("Velocity must be a proper numeric value."));
	} else if (bValid) {
		if (*pnVelocity <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Velocity"), tr("Velocity must be greater than ZERO!!"));
			}
		} else {
			if (m_bMetric) *pnVelocity = toImperial(*pnVelocity, VelocityConv);
		}
	}
	return bValid;
}

bool CAreaFlowCalcDialog::validateWaterNeeded(double *pnWaterNeeded, bool bPrompt)
{
	assert(pnWaterNeeded != nullptr);
	bool bValid = true;
	*pnWaterNeeded = ui->editWaterNeeded->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		if (m_bMetric) {
			QMessageBox::critical(this, tr("Invalid Liters Per Minute"), tr("Liters Per Minute must be a proper numeric value."));
		} else {
			QMessageBox::critical(this, tr("Invalid Gallons Per Minute"), tr("Gallons Per Minute must be a proper numeric value."));
		}
	} else if (bValid) {
		if (*pnWaterNeeded <= 0.0) {
			bValid = false;
			if (bPrompt) {
				if (m_bMetric) {
					QMessageBox::critical(this, tr("Invalid Liters Per Minute"), tr("Liters Per Minute must be greater than ZERO!!"));
				} else {
					QMessageBox::critical(this, tr("Invalid Gallons Per Minute"), tr("Gallons Per Minute must be greater than ZERO!!"));
				}
			}
		} else {
			if (m_bMetric) *pnWaterNeeded = toImperial(*pnWaterNeeded, WaterNeededConv);
		}
	}
	return bValid;
}

bool CAreaFlowCalcDialog::validateCrossSectArea(double *pnCrossSectArea, bool bPrompt)
{
	assert(pnCrossSectArea != nullptr);
	bool bValid = true;
	*pnCrossSectArea = ui->editCrossSectArea->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Cross Section Area"), tr("Cross Section Area must be a proper numeric value."));
	} else if (bValid) {
		if (*pnCrossSectArea <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Cross Section Area"), tr("Cross Section Area must be greater than ZERO!!"));
			}
		} else {
			if (m_bMetric) *pnCrossSectArea = toImperial(*pnCrossSectArea, AreaConv);
		}
	}
	return bValid;
}

// -----------------------------------------------------------------------------

int CAreaFlowCalcDialog::exec()
{
	ui->editVelocity->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nVelocity, VelocityConv) : m_DialogValues.m_nVelocity));
	ui->editWaterNeeded->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nWaterNeeded, WaterNeededConv) : m_DialogValues.m_nWaterNeeded));
	ui->editCrossSectArea->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nCrossSectArea, AreaConv) : m_DialogValues.m_nCrossSectArea));

	ui->editVelocity->setFocus();

	return QDialog::exec();
}

void CAreaFlowCalcDialog::accept()
{
	double nVelocity = 0.0;
	double nWaterNeeded = 0.0;
	double nCrossSectArea = 0.0;

	if (!validateVelocity(&nVelocity, true)) return;
	if (!validateWaterNeeded(&nWaterNeeded, true)) return;
	if (!validateCrossSectArea(&nCrossSectArea, true)) return;

	if (fabs(nWaterNeeded - ((nVelocity*sec_per_min)*(nCrossSectArea/sqin_per_sqft)*gallons_per_cubft)) > 0.3) {
		QMessageBox::critical(this, tr("Area Flow Calculation Error"),
				tr("Cannot apply or save the area flow information until you have run calculate."));
		return;
	}

	m_DialogValues.m_nVelocity = nVelocity;
	m_DialogValues.m_nWaterNeeded = nWaterNeeded;
	m_DialogValues.m_nCrossSectArea = nCrossSectArea;
	QDialog::accept();
}

// -----------------------------------------------------------------------------

void CAreaFlowCalcDialog::calcVelocity()
{
	double nWaterNeeded;
	double nCrossSectArea;
	if (!validateWaterNeeded(&nWaterNeeded, true)) return;
	if (!validateCrossSectArea(&nCrossSectArea, true)) return;

	double nVelocity = ((nWaterNeeded/gallons_per_cubft)/(nCrossSectArea/sqin_per_sqft))/sec_per_min;
	ui->editVelocity->setText(QString().setNum(m_bMetric ? toMetric(nVelocity, VelocityConv) : nVelocity));
}

void CAreaFlowCalcDialog::calcWaterNeeded()
{
	double nVelocity;
	double nCrossSectArea;
	if (!validateVelocity(&nVelocity, true)) return;
	if (!validateCrossSectArea(&nCrossSectArea, true)) return;

	double nWaterNeeded = ((nVelocity*sec_per_min)*(nCrossSectArea/sqin_per_sqft)*gallons_per_cubft);
	ui->editWaterNeeded->setText(QString().setNum(m_bMetric ? toMetric(nWaterNeeded, WaterNeededConv) : nWaterNeeded));
}

void CAreaFlowCalcDialog::calcCrossSectArea()
{
	double nVelocity;
	double nWaterNeeded;
	if (!validateVelocity(&nVelocity, true)) return;
	if (!validateWaterNeeded(&nWaterNeeded, true)) return;

	double nCrossSectArea = (((nWaterNeeded/gallons_per_cubft)/(nVelocity*sec_per_min))*sqin_per_sqft);
	ui->editCrossSectArea->setText(QString().setNum(m_bMetric ? toMetric(nCrossSectArea, AreaConv) : nCrossSectArea));
}

// -----------------------------------------------------------------------------

void CAreaFlowCalcDialog::en_changeVelocity()
{
	double nDummy = 0.0;
	ui->btnCalcWaterNeeded->setEnabled(validateVelocity(&nDummy, false) && validateCrossSectArea(&nDummy, false));
	ui->btnCalcCrossSectArea->setEnabled(validateVelocity(&nDummy, false) && validateWaterNeeded(&nDummy, false));
	setEnableSaveApply();
}

void CAreaFlowCalcDialog::en_changeWaterNeeded()
{
	double nDummy = 0.0;
	ui->btnCalcVelocity->setEnabled(validateWaterNeeded(&nDummy, false) && validateCrossSectArea(&nDummy, false));
	ui->btnCalcCrossSectArea->setEnabled(validateWaterNeeded(&nDummy, false) && validateVelocity(&nDummy, false));
	setEnableSaveApply();
}

void CAreaFlowCalcDialog::en_changeCrossSectArea()
{
	double nDummy = 0.0;
	ui->btnCalcVelocity->setEnabled(validateCrossSectArea(&nDummy, false) && validateWaterNeeded(&nDummy, false));
	ui->btnCalcWaterNeeded->setEnabled(validateCrossSectArea(&nDummy, false) && validateVelocity(&nDummy, false));
	setEnableSaveApply();
}

void CAreaFlowCalcDialog::setEnableSaveApply()
{
	double nDummy = 0.0;
	assert(m_pSaveApplyButton != nullptr);
	m_pSaveApplyButton->setEnabled(	validateVelocity(&nDummy, false) &&
									validateWaterNeeded(&nDummy, false) &&
									validateCrossSectArea(&nDummy, false));
}

// -----------------------------------------------------------------------------

void CAreaFlowCalcDialog::showHelp()
{
	if (!g_pMyMainWindow.isNull()) {
		g_pMyMainWindow->showHelp(CHelpDialog::helpDocument(CHelpDialog::HELPDOC_OTHER_HELPS));
	}
}

// =============================================================================
