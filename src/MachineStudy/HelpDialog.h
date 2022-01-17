/****************************************************************************
**
** Multipli Machine Study Application
** Copyright (C) 1994, 2018-2022 Multipli Machinery, Corp.
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

#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

// =============================================================================

namespace Ui {
	class CHelpDialog;
}

class CHelpDialog : public QDialog
{
	Q_OBJECT

public:
	enum HELP_DOCUMENT_ENUM {
		HELPDOC_MAIN = 0,
		HELPDOC_ABOUT = 1,
		HELPDOC_APPLICATION = 2,
		HELPDOC_CONTROL_TERMS = 3,
		HELPDOC_DISCLAIMER = 4,
		HELPDOC_IN_CASE_OF_TROUBLE = 5,
		HELPDOC_INSTALLATION = 6,
		HELPDOC_OTHER_HELPS = 7,
		HELPDOC_PRINT_AND_EDIT = 8,
		HELPDOC_TERMS = 9,
		HELPDOC_WHY_DEVELOPED = 10,
	};
	static const QString helpDocument(HELP_DOCUMENT_ENUM nDocument);

	explicit CHelpDialog(QWidget *parent = nullptr);
	~CHelpDialog();

public slots:
	void navigateTo(const QString &strDocument = QString());

private:
	Ui::CHelpDialog *ui;
};

// =============================================================================

#endif // HELPDIALOG_H
