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

#include "HelpDialog.h"
#include "ui_HelpDialog.h"

#include <QtHelp>
#include <QApplication>

// =============================================================================

namespace {
	const QString gconstrBaseHelpURL = "qthelp://com.multiplimachine.study/doc/html";
	const QString gconstMainHelpPage = "mach0wc8.htm";
	const QString gconstAboutHelpPage = "mach15f8.htm";
	const QString gconstApplicationHelpPage = "mach5mb2.htm";
	const QString gconstControlTermsHelpPage = "mach008j.htm";
	const QString gconstDisclaimerHelpPage = "mach3igi.htm";
	const QString gconstInCaseOfTroubleHelpPage = "mach6gv9.htm";
	const QString gconstInstallationHelpPage = "mach8ib2.htm";
	const QString gconstOtherHelpsHelpPage = "mach0r5f.htm";
	const QString gconstPrintAndEditHelpPage = "mach1i9g.htm";
	const QString gconstTermsHelpPage = "mach0jlf.htm";
	const QString gconstWhyDevelopedHelpPage = "mach8azx.htm";
}

const QString CHelpDialog::helpDocument(CHelpDialog::HELP_DOCUMENT_ENUM nDocument)
{
	switch(nDocument) {
		case HELPDOC_MAIN:
			return gconstMainHelpPage;
		case HELPDOC_ABOUT:
			return gconstAboutHelpPage;
		case HELPDOC_APPLICATION:
			return gconstApplicationHelpPage;
		case HELPDOC_CONTROL_TERMS:
			return gconstControlTermsHelpPage;
		case HELPDOC_DISCLAIMER:
			return gconstDisclaimerHelpPage;
		case HELPDOC_IN_CASE_OF_TROUBLE:
			return gconstInCaseOfTroubleHelpPage;
		case HELPDOC_INSTALLATION:
			return gconstInstallationHelpPage;
		case HELPDOC_OTHER_HELPS:
			return gconstOtherHelpsHelpPage;
		case HELPDOC_PRINT_AND_EDIT:
			return gconstPrintAndEditHelpPage;
		case HELPDOC_TERMS:
			return gconstTermsHelpPage;
		case HELPDOC_WHY_DEVELOPED:
			return gconstWhyDevelopedHelpPage;
		default:
			return QString();
	}
}

// -----------------------------------------------------------------------------

CHelpDialog::CHelpDialog(QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
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

	navigateTo();

	connect(pHelpEngine->contentWidget(), SIGNAL(linkActivated(QUrl)), ui->helpBrowser, SLOT(setSource(QUrl)));
	connect(pHelpEngine->indexWidget(), SIGNAL(linkActivated(QUrl,QString)), ui->helpBrowser, SLOT(setSource(QUrl)));
}

CHelpDialog::~CHelpDialog()
{
	delete ui;
}

void CHelpDialog::navigateTo(const QString &strDocument)
{
	ui->helpBrowser->setSource(QUrl(gconstrBaseHelpURL + "/" + (strDocument.isEmpty() ? gconstMainHelpPage : strDocument)));
}

// =============================================================================
