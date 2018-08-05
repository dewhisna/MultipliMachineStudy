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

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QApplication>

// =============================================================================

CAboutDialog::CAboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CAboutDialog)
{
	ui->setupUi(this);

	ui->lblProgramName->setText(ui->lblProgramName->text() + QApplication::applicationVersion());
}

CAboutDialog::~CAboutDialog()
{
	delete ui;
}

// =============================================================================
