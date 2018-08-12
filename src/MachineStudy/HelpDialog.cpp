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

#include "HelpDialog.h"
#include "ui_HelpDialog.h"

#include <QtHelp>
#include <QApplication>

// =============================================================================

CHelpDialog::CHelpDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CHelpDialog)
{
	ui->setupUi(this);

	QHelpEngine* pHelpEngine = new QHelpEngine(
					QApplication::applicationDirPath() +
					"/machstdy.qhc", this);
	pHelpEngine->setupData();
	ui->helpBrowser->setHelpEngine(pHelpEngine);

	ui->tabWidget->clear();
//	ui->tabWidget->addTab(pHelpEngine->contentWidget(), tr("Contents"));
	ui->tabWidget->addTab(pHelpEngine->indexWidget(), tr("Index"));

	ui->helpBrowser->setSource(QUrl("qthelp://com.multiplimachine.study/doc/html/mach0wc8.htm"));

	connect(pHelpEngine->contentWidget(), SIGNAL(linkActivated(QUrl)), ui->helpBrowser, SLOT(setSource(QUrl)));
	connect(pHelpEngine->indexWidget(), SIGNAL(linkActivated(QUrl,QString)), ui->helpBrowser, SLOT(setSource(QUrl)));
}

CHelpDialog::~CHelpDialog()
{
	delete ui;
}

// =============================================================================
