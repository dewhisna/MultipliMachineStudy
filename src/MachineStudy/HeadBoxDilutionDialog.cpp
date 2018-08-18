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

#include "HeadBoxDilutionDialog.h"
#include "ui_HeadBoxDilutionDialog.h"

#include "conversion.h"

#include "MainWindow.h"
#include "HelpDialog.h"

#include <QPushButton>
#include <QMessageBox>

// =============================================================================

CHeadBoxDilutionDialog::CHeadBoxDilutionDialog(bool bStandAlone, bool bMetric, QWidget *parent) :
	QDialog(parent),
	m_bStandAlone(bStandAlone),
	m_bMetric(bMetric),
	m_pCalcButton(nullptr),
	m_pSaveApplyButton(nullptr),
	ui(new Ui::CHeadBoxDilutionDialog)
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

	connect(ui->editYieldPerDay, SIGNAL(textChanged(QString)), this, SLOT(en_changeYieldPerDay()));
	connect(ui->editPresentConsistency, SIGNAL(textChanged(QString)), this, SLOT(en_changePresentConsistency()));
	connect(ui->editNewConsistency, SIGNAL(textChanged(QString)), this, SLOT(en_changeNewConsistency()));
}

CHeadBoxDilutionDialog::~CHeadBoxDilutionDialog()
{
	delete ui;
}

// -----------------------------------------------------------------------------

void CHeadBoxDilutionDialog::setUIText()
{
	if (m_bMetric) {
		ui->lblYieldPerDay->setText(tr("Kilograms Per Day:"));
		ui->lblPresentConsistency->setText(tr("Present Consistency (%):"));
		ui->lblNewConsistency->setText(tr("New Consistency (%):"));
		ui->lblWaterNeeded->setText(tr("Water Needed (LPM):"));
	} else {
		ui->lblYieldPerDay->setText(tr("Tons Per Day:"));
		ui->lblPresentConsistency->setText(tr("Present Consistency (%):"));
		ui->lblNewConsistency->setText(tr("New Consistency (%):"));
		ui->lblWaterNeeded->setText(tr("Water Needed (GPM):"));
	}
}

void CHeadBoxDilutionDialog::setMetric(bool bMetric)
{
	m_bMetric = bMetric;
	setUIText();
}

// -----------------------------------------------------------------------------

bool CHeadBoxDilutionDialog::validateYieldPerDay(double *pnYieldPerDay, bool bPrompt)
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

bool CHeadBoxDilutionDialog::validatePresentConsistency(double *pnPresentConsistency, bool bPrompt)
{
	assert(pnPresentConsistency != nullptr);
	bool bValid = true;
	*pnPresentConsistency = ui->editPresentConsistency->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Present Consistency"), tr("Present Consistency must be a proper numeric value."));
	} else if (bValid) {
		if (*pnPresentConsistency <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Present Consistency"), tr("Present Consistency must be greater than ZERO!!"));
			}
		} else {
			*pnPresentConsistency = *pnPresentConsistency / 100.0;
		}
	}
	return bValid;
}

bool CHeadBoxDilutionDialog::validateNewConsistency(double *pnNewConsistency, bool bPrompt)
{
	assert(pnNewConsistency != nullptr);
	bool bValid = true;
	*pnNewConsistency = ui->editNewConsistency->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid New Consistency"), tr("New Consistency must be a proper numeric value."));
	} else if (bValid) {
		if (*pnNewConsistency <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid New Consistency"), tr("New Consistency must be greater than ZERO!!"));
			}
		} else {
			*pnNewConsistency = *pnNewConsistency / 100.0;
		}
	}
	return bValid;
}

// -----------------------------------------------------------------------------

int CHeadBoxDilutionDialog::exec()
{
	ui->editYieldPerDay->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nYieldPerDay, YieldPerDayConv) : m_DialogValues.m_nYieldPerDay));
	ui->editPresentConsistency->setText(QString().setNum(m_DialogValues.m_nPresentConsistency * 100.0));
	ui->editNewConsistency->setText(QString().setNum(m_DialogValues.m_nNewConsistency * 100.0));

	ui->editYieldPerDay->setFocus();

	return QDialog::exec();
}

void CHeadBoxDilutionDialog::accept()
{
	double nYieldPerDay = 0.0;
	double nPresentConsistency = 1.0;
	double nNewConsistency = 1.0;

	if (!validateYieldPerDay(&nYieldPerDay, true)) return;
	if (!validatePresentConsistency(&nPresentConsistency, true)) return;
	if (!validateNewConsistency(&nNewConsistency, true)) return;
	if (!calc()) return;

	m_DialogValues.m_nYieldPerDay = nYieldPerDay;
	m_DialogValues.m_nPresentConsistency = nPresentConsistency;
	m_DialogValues.m_nNewConsistency = nNewConsistency;
	QDialog::accept();
}

// -----------------------------------------------------------------------------

bool CHeadBoxDilutionDialog::calc(bool bPrompt)
{
	double nYieldPerDay = 0.0;
	double nPresentConsistency = 1.0;
	double nNewConsistency = 1.0;

	if ((!validateYieldPerDay(&nYieldPerDay, bPrompt)) ||
		(!validatePresentConsistency(&nPresentConsistency, bPrompt)) ||
		(!validateNewConsistency(&nNewConsistency, bPrompt))) {
		ui->editWaterNeeded->setText(QString());
		m_pCalcButton->setEnabled(false);
		return false;
	}

	m_DialogValues.m_nWaterNeeded = ((nYieldPerDay*((1.0/6.0)))/nNewConsistency) - ((nYieldPerDay*((1.0/6.0)))/nPresentConsistency);
	ui->editWaterNeeded->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nWaterNeeded, WaterNeededConv) : m_DialogValues.m_nWaterNeeded));
	m_pCalcButton->setEnabled(true);
	return true;
}

// -----------------------------------------------------------------------------

void CHeadBoxDilutionDialog::en_changeYieldPerDay()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

void CHeadBoxDilutionDialog::en_changePresentConsistency()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

void CHeadBoxDilutionDialog::en_changeNewConsistency()
{
	m_pSaveApplyButton->setEnabled(calc(false));
}

// -----------------------------------------------------------------------------

void CHeadBoxDilutionDialog::showHelp()
{
	if (!g_pMyMainWindow.isNull()) {
		g_pMyMainWindow->showHelp(CHelpDialog::helpDocument(CHelpDialog::HELPDOC_OTHER_HELPS));
	}
}

// =============================================================================

