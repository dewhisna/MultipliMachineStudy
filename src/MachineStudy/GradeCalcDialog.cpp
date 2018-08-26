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

#include "GradeCalcDialog.h"
#include "ui_GradeCalcDialog.h"

#include "conversion.h"

#include "MainWindow.h"

#include <QPushButton>
#include <QMessageBox>

// =============================================================================

CGradeCalcDialog::CGradeCalcDialog(bool bStandAlone, bool bMetric, QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
	m_bStandAlone(bStandAlone),
	m_bMetric(bMetric),
	m_pSaveApplyButton(nullptr),
	ui(new Ui::CGradeCalcDialog)
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
		connect(pHelpButton, SIGNAL(clicked(bool)), g_pMyMainWindow.data(), SLOT(showHelp()));
	}

	m_pSaveApplyButton = bStandAlone ? pSaveButton : pApplyButton;
	connect(m_pSaveApplyButton, SIGNAL(clicked(bool)), this, SLOT(accept()));

	setUIText();

	connect(ui->btnCalcCaliper, SIGNAL(clicked(bool)), this, SLOT(calcCaliper()));
	connect(ui->btnCalcDensity, SIGNAL(clicked(bool)), this, SLOT(calcDensity()));
	connect(ui->btnCalcBasisWeight, SIGNAL(clicked(bool)), this, SLOT(calcBasisWeight()));

	connect(ui->editCaliper, SIGNAL(textChanged(QString)), this, SLOT(en_changeCaliper()));
	connect(ui->editDensity, SIGNAL(textChanged(QString)), this, SLOT(en_changeDensity()));
	connect(ui->editBasisWeight, SIGNAL(textChanged(QString)), this, SLOT(en_changeBasisWeight()));
}

CGradeCalcDialog::~CGradeCalcDialog()
{
	delete ui;
}

// -----------------------------------------------------------------------------

void CGradeCalcDialog::setUIText()
{
	if (m_bMetric) {
		ui->lblCaliper->setText(tr("Caliper (points):"));
		ui->lblDensity->setText(tr("Sheet Density (grams/point):"));
		ui->lblBasisWeight->setText(tr("Basis Weight (grams/M Sq.):"));
	} else {
		ui->lblCaliper->setText(tr("Caliper (points):"));
		ui->lblDensity->setText(tr("Sheet Density (lbs/point):"));
		ui->lblBasisWeight->setText(tr("Basis Weight (lbs/1K Sq. In.):"));
	}
}

void CGradeCalcDialog::setMetric(bool bMetric)
{
	m_bMetric = bMetric;
	setUIText();
}

// -----------------------------------------------------------------------------

bool CGradeCalcDialog::validateCaliper(double *pnCaliper, bool bPrompt)
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

bool CGradeCalcDialog::validateDensity(double *pnDensity, bool bPrompt)
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

bool CGradeCalcDialog::validateBasisWeight(double *pnBasisWeight, bool bPrompt)
{
	assert(pnBasisWeight != nullptr);
	bool bValid = true;
	*pnBasisWeight = ui->editBasisWeight->text().toDouble(&bValid);
	if ((!bValid) && (bPrompt)) {
		QMessageBox::critical(this, tr("Invalid Basis Weight"), tr("Basis Weight must be a proper numeric value."));
	} else if (bValid) {
		if (*pnBasisWeight <= 0.0) {
			bValid = false;
			if (bPrompt) {
				QMessageBox::critical(this, tr("Invalid Basis Weight"), tr("Basis Weight must be greater than ZERO!!"));
			}
		} else {
			if (m_bMetric) *pnBasisWeight = toImperial(*pnBasisWeight, BasisWeightConv);
		}
	}
	return bValid;
}

// -----------------------------------------------------------------------------

int CGradeCalcDialog::exec()
{
	ui->editCaliper->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nCaliper, CaliperConv) : m_DialogValues.m_nCaliper));
	ui->editDensity->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nDensity, DensityConv) : m_DialogValues.m_nDensity));
	ui->editBasisWeight->setText(QString().setNum(m_bMetric ? toMetric(m_DialogValues.m_nBasisWeight, BasisWeightConv) : m_DialogValues.m_nBasisWeight));

	ui->editCaliper->setFocus();

	return QDialog::exec();
}

void CGradeCalcDialog::accept()
{
	double nCaliper = 0.0;
	double nDensity = 0.0;
	double nBasisWeight = 0.0;

	if (!validateCaliper(&nCaliper, true)) return;
	if (!validateDensity(&nDensity, true)) return;
	if (!validateBasisWeight(&nBasisWeight, true)) return;

	if (fabs(nBasisWeight - (nCaliper*nDensity)) > 0.3) {
		QMessageBox::critical(this, tr("Grade Calculation Error"),
				tr("Cannot apply or save the grade information until you have run calculate."));
		return;
	}

	m_DialogValues.m_nCaliper = nCaliper;
	m_DialogValues.m_nDensity = nDensity;
	m_DialogValues.m_nBasisWeight = nBasisWeight;
	QDialog::accept();
}

// -----------------------------------------------------------------------------

void CGradeCalcDialog::calcCaliper()
{
	double nDensity;
	double nBasisWeight;
	if (!validateDensity(&nDensity, true)) return;
	if (!validateBasisWeight(&nBasisWeight, true)) return;

	double nCaliper = nBasisWeight / nDensity;
	ui->editCaliper->setText(QString().setNum(m_bMetric ? toMetric(nCaliper, CaliperConv) : nCaliper));
}

void CGradeCalcDialog::calcDensity()
{
	double nCaliper;
	double nBasisWeight;
	if (!validateCaliper(&nCaliper, true)) return;
	if (!validateBasisWeight(&nBasisWeight, true)) return;

	double nDensity = nBasisWeight / nCaliper;
	ui->editDensity->setText(QString().setNum(m_bMetric ? toMetric(nDensity, DensityConv) : nDensity));
}

void CGradeCalcDialog::calcBasisWeight()
{
	double nCaliper;
	double nDensity;
	if (!validateCaliper(&nCaliper, true)) return;
	if (!validateDensity(&nDensity, true)) return;

	double nBasisWeight = nCaliper * nDensity;
	ui->editBasisWeight->setText(QString().setNum(m_bMetric ? toMetric(nBasisWeight, BasisWeightConv) : nBasisWeight));
}

// -----------------------------------------------------------------------------

void CGradeCalcDialog::en_changeCaliper()
{
	double nDummy = 0.0;
	ui->btnCalcDensity->setEnabled(validateCaliper(&nDummy, false) && validateBasisWeight(&nDummy, false));
	ui->btnCalcBasisWeight->setEnabled(validateCaliper(&nDummy, false) && validateDensity(&nDummy, false));
	setEnableSaveApply();
}

void CGradeCalcDialog::en_changeDensity()
{
	double nDummy = 0.0;
	ui->btnCalcCaliper->setEnabled(validateDensity(&nDummy, false) && validateBasisWeight(&nDummy, false));
	ui->btnCalcBasisWeight->setEnabled(validateDensity(&nDummy, false) && validateCaliper(&nDummy, false));
	setEnableSaveApply();
}

void CGradeCalcDialog::en_changeBasisWeight()
{
	double nDummy = 0.0;
	ui->btnCalcCaliper->setEnabled(validateBasisWeight(&nDummy, false) && validateDensity(&nDummy, false));
	ui->btnCalcDensity->setEnabled(validateBasisWeight(&nDummy, false) && validateCaliper(&nDummy, false));
	setEnableSaveApply();
}

void CGradeCalcDialog::setEnableSaveApply()
{
	double nDummy = 0.0;
	assert(m_pSaveApplyButton != nullptr);
	m_pSaveApplyButton->setEnabled(	validateCaliper(&nDummy, false) &&
									validateDensity(&nDummy, false) &&
									validateBasisWeight(&nDummy, false));
}

// =============================================================================
