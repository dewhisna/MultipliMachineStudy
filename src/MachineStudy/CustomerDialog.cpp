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

#include "CustomerDialog.h"
#include "ui_CustomerDialog.h"

// =============================================================================

CCustomerDialog::CCustomerDialog(QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
	ui(new Ui::CCustomerDialog)
{
	ui->setupUi(this);
}

CCustomerDialog::~CCustomerDialog()
{
	delete ui;
}

// -----------------------------------------------------------------------------

int CCustomerDialog::exec()
{
	ui->editCustomer->setText(m_DialogValues.m_strCustomer);

	return QDialog::exec();
}

void CCustomerDialog::accept()
{
	m_DialogValues.m_strCustomer = ui->editCustomer->text();
	QDialog::accept();
}

// =============================================================================
