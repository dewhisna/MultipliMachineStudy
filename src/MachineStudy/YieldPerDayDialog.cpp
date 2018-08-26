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

#include "YieldPerDayDialog.h"
#include "ui_YieldPerDayDialog.h"

#include "conversion.h"

#include <QMessageBox>

// =============================================================================

CYieldPerDayDialog::CYieldPerDayDialog(bool bMetric, QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
	m_bMetric(bMetric),
	m_pOKButton(nullptr),
	ui(new Ui::CYieldPerDayDialog)
{
	ui->setupUi(this);

	setUIText();

	m_pOKButton = ui->buttonBox->button(QDialogButtonBox::Ok);
	assert(m_pOKButton != nullptr);

	connect(ui->editYieldPerDay, SIGNAL(textChanged(QString)), this, SLOT(en_changeYieldPerDay()));
}

CYieldPerDayDialog::~CYieldPerDayDialog()
{
	delete ui;
}

// -----------------------------------------------------------------------------

void CYieldPerDayDialog::setUIText()
{
	if (m_bMetric) {
		setWindowTitle("Enter Kilograms Per Day");
		ui->lblYieldPerDay->setText(tr("Kilograms Per Day:"));
	} else {
		setWindowTitle("Enter Tons Per Day");
		ui->lblYieldPerDay->setText(tr("Tons Per Day:"));
	}
}

void CYieldPerDayDialog::setMetric(bool bMetric)
{
	m_bMetric = bMetric;
	setUIText();
}

// -----------------------------------------------------------------------------

bool CYieldPerDayDialog::validateYieldPerDay(double *pnYieldPerDay, bool bPrompt)
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

// -----------------------------------------------------------------------------

int CYieldPerDayDialog::exec()
{
	ui->editYieldPerDay->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nYieldPerDay, YieldPerDayConv) : m_DialogValues.m_nYieldPerDay));
	ui->editYieldPerDay->setFocus();
	return QDialog::exec();
}

void CYieldPerDayDialog::accept()
{
	double nYieldPerDay = 0.0;
	if (!validateYieldPerDay(&nYieldPerDay, true)) return;
	m_DialogValues.m_nYieldPerDay = nYieldPerDay;
	QDialog::accept();
}

// -----------------------------------------------------------------------------

void CYieldPerDayDialog::en_changeYieldPerDay()
{
	double nYieldPerDay;
	m_pOKButton->setEnabled(validateYieldPerDay(&nYieldPerDay, false));
}

// =============================================================================
