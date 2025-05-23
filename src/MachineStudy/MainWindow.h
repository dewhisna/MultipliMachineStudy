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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QPointer>
#include <QFont>

#include "PipeFlowCalcDialog.h"
#include "AreaFlowCalcDialog.h"

// =============================================================================

// Forward declarations:
class QPrinter;
class FindDialog;
class FindReplaceDialog;
class CHelpDialog;

namespace Ui {
	class CMainWindow;
}

class CMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit CMainWindow(QWidget *parent = nullptr);
	virtual ~CMainWindow();

signals:
	void metricModeSelected(bool bMetric);		// Inbound or menu option toggle

protected slots:
	// --- File Menu
	void en_NewFile();
	void en_OpenFile();
	void en_SaveFile();
	void en_SaveFileAs();
	void en_PrintFile();
	void en_PrintPreview();
	void en_print(QPrinter *printer);
	void en_quit();

	// --- Edit Menu
	void en_Search();
	void en_SearchReplace();

	// --- Setup Menu
	void en_SetupMetric();						// Menu option toggle
	void en_changeMetric(bool bMetric);			// Inbound option toggle (from subdialogs)

	// --- Formation Menu
	void en_FormationSheetFormation();
	void en_FormationProduction();
	void en_FormationGrade();
	void en_FormationHeadboxDilution();
	void en_FormationFormationConst();

	// --- Flow Menu
	void en_FlowPipe();
	void en_FlowArea();

	// --- Help Menu
	void en_HelpHelp();
	void en_HelpAbout();

	// ----------
	void en_TextChanged();

public slots:
	void insertText(const QString &strText);
	void insertFlowPipeText(const CPipeFlowCalcDialog::TDialogValues &values);
	void insertFlowAreaText(const CAreaFlowCalcDialog::TDialogValues &values);
	void showHelp(const QString &strDocument = QString());

protected:
	void TextOutputStart(bool bSkipCustomer = false, bool bSkipAppend = false);
	bool PromptLoseChanges();

private:
	bool m_bMetric;
	bool m_bHaveTextOutput;
	QString m_strLastCustomer;
	bool m_bDirty;
	QString m_strFilename;
	QString m_strLastDirectory;
	QAction *m_pMetricAction;
	QString m_strMainWindowTitle;
	QPointer<FindDialog> m_pFindDialog;
	QPointer<FindReplaceDialog> m_pFindReplaceDialog;
	QPointer<CHelpDialog> m_pHelpDialog;
	QFont m_fntFixed;
	Ui::CMainWindow *ui;
};

extern QPointer<CMainWindow> g_pMyMainWindow;

// =============================================================================

#endif // MAINWINDOW_H
