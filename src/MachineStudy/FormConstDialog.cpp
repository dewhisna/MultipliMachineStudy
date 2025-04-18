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

#include "FormConstDialog.h"
#include "ui_FormConstDialog.h"

#include "conversion.h"

#include "MainWindow.h"

#include <QPushButton>
#include <QMessageBox>

// =============================================================================

CFormConstDialog::CFormConstDialog(bool bStandAlone, bool bMetric, QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
	m_bStandAlone(bStandAlone),
	m_bMetric(bMetric),
	m_pCalcButton(nullptr),
	m_pSaveApplyButton(nullptr),
	ui(new Ui::CFormConstDialog)
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
		connect(pHelpButton, SIGNAL(clicked(bool)), g_pMyMainWindow.data(), SLOT(showHelp()));
	}

	m_pSaveApplyButton = bStandAlone ? pSaveButton : pApplyButton;
	m_pCalcButton->setVisible(false);		// Leave invisible since we are currently doing automatic calculations
	connect(m_pSaveApplyButton, SIGNAL(clicked(bool)), this, SLOT(accept()));

	setUIText();

	connect(m_pCalcButton, SIGNAL(clicked(bool)), this, SLOT(calc()));		// NOTE: Don't pass bool argument on as they mean different things!

	connect(ui->editYieldPerDay, SIGNAL(textChanged(QString)), this, SLOT(en_changeYieldPerDay()));
	connect(ui->editNumberOfVats, SIGNAL(textChanged(QString)), this, SLOT(en_changeNumberOfVats()));
	connect(ui->editSheetWidth, SIGNAL(textChanged(QString)), this, SLOT(en_changeSheetWidth()));
	connect(ui->editMachineEfficiency, SIGNAL(textChanged(QString)), this, SLOT(en_changeMachineEfficiency()));
}

CFormConstDialog::~CFormConstDialog()
{
	delete ui;
}


// -----------------------------------------------------------------------------

void CFormConstDialog::setUIText()
{
	if (m_bMetric) {
		ui->lblYieldPerDay->setText(tr("Kilograms Per Day:"));
		ui->lblNumberOfVats->setText(tr("Number of Vats:"));
		ui->lblSheetWidth->setText(tr("Sheet Width (mm):"));
		ui->lblMachineEfficiency->setText(tr("Machine Efficiency (%):"));
		ui->lblFormConst->setText(tr("Formation Constant:"));
	} else {
		ui->lblYieldPerDay->setText(tr("Tons Per Day:"));
		ui->lblNumberOfVats->setText(tr("Number of Vats:"));
		ui->lblSheetWidth->setText(tr("Sheet Width (in):"));
		ui->lblMachineEfficiency->setText(tr("Machine Efficiency (%):"));
		ui->lblFormConst->setText(tr("Formation Constant:"));
	}
}

void CFormConstDialog::setMetric(bool bMetric)
{
	m_bMetric = bMetric;
	setUIText();
}

// -----------------------------------------------------------------------------

bool CFormConstDialog::validateYieldPerDay(double *pnYieldPerDay, bool bPrompt)
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

bool CFormConstDialog::validateNumberOfVats(int *pnNumberOfVats, bool bPrompt)
{
	assert(pnNumberOfVats != nullptr);
	bool bValid = true;
	*pnNumberOfVats = ui->editNumberOfVats->text().toInt(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Number of Vats"), tr("Number of Vats must be a proper numeric value."));
	} else if (bValid) {
		if (*pnNumberOfVats <= 0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Number of Vats"), tr("Number of Vats must be greater than ZERO!!"));
			}
		}
	}
	return bValid;
}

bool CFormConstDialog::validateSheetWidth(double *pnSheetWidth, bool bPrompt)
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

bool CFormConstDialog::validateMachineEfficiency(double *pnMachineEfficiency, bool bPrompt)
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

// -----------------------------------------------------------------------------

int CFormConstDialog::exec()
{
	ui->editYieldPerDay->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nYieldPerDay, YieldPerDayConv) : m_DialogValues.m_nYieldPerDay));
	ui->editNumberOfVats->setText(QString().setNum(m_DialogValues.m_nNumberOfVats));
	ui->editSheetWidth->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nSheetWidth, SheetWidthConv) : m_DialogValues.m_nSheetWidth));
	ui->editMachineEfficiency->setText(QString().setNum(m_DialogValues.m_nMachineEfficiency * 100.0));

	ui->editYieldPerDay->setFocus();

	return QDialog::exec();
}

void CFormConstDialog::accept()
{
	double nYieldPerDay = 0.0;
	int nNumberOfVats = 0;
	double nSheetWidth = 0.0;
	double nMachineEfficiency = 1.0;

	if (!validateYieldPerDay(&nYieldPerDay, true)) return;
	if (!validateNumberOfVats(&nNumberOfVats, true)) return;
	if (!validateSheetWidth(&nSheetWidth, true)) return;
	if (!validateMachineEfficiency(&nMachineEfficiency, true)) return;
	if (!calc()) return;

	m_DialogValues.m_nYieldPerDay = nYieldPerDay;
	m_DialogValues.m_nNumberOfVats = nNumberOfVats;
	m_DialogValues.m_nSheetWidth = nSheetWidth;
	m_DialogValues.m_nMachineEfficiency = nMachineEfficiency;
	QDialog::accept();
}

// -----------------------------------------------------------------------------

bool CFormConstDialog::calc(bool bPrompt)
{
	double nYieldPerDay = 0.0;
	int nNumberOfVats = 0;
	double nSheetWidth = 0.0;
	double nMachineEfficiency = 1.0;

	if ((!validateYieldPerDay(&nYieldPerDay, bPrompt)) ||
		(!validateNumberOfVats(&nNumberOfVats, bPrompt)) ||
		(!validateSheetWidth(&nSheetWidth, bPrompt)) ||
		(!validateMachineEfficiency(&nMachineEfficiency, bPrompt))) {
		ui->editFormConst->setText(QString());
		m_pCalcButton->setEnabled(false);
		return false;
	}

	if (nMachineEfficiency) nYieldPerDay /= nMachineEfficiency;
	m_DialogValues.m_nFormConst = (nYieldPerDay/nNumberOfVats)/nSheetWidth;
	ui->editFormConst->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nFormConst, FormConv) : m_DialogValues.m_nFormConst));
	m_pCalcButton->setEnabled(true);
	return true;
}

// -----------------------------------------------------------------------------

void CFormConstDialog::en_changeYieldPerDay()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

void CFormConstDialog::en_changeNumberOfVats()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

void CFormConstDialog::en_changeSheetWidth()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

void CFormConstDialog::en_changeMachineEfficiency()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

// =============================================================================

