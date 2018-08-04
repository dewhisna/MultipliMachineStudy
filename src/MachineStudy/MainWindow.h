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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

// =============================================================================

namespace Ui {
	class CMainWindow;
}

class CMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit CMainWindow(QWidget *parent = nullptr);
	~CMainWindow();

signals:
	void metricModeSelected(bool bMetric);

protected slots:
	// --- File Menu
	void en_NewFile();
	void en_OpenFile();
	void en_SaveFile();
	void en_SaveFileAs();
	void en_PrintFile();
	void en_PrintSetup();

	// --- Edit Menu
	void en_Search();
	void en_SearchReplace();

	// --- Setup Menu
	void en_SetupMetric();

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
	void en_InsertText(const QString &strText);
	void en_TextChanged();

protected:
	void TextOutputStart();

private:
	bool m_bMetric;
	bool m_bHaveTextOutput;
	QString m_strLastCustomer;
	bool m_bDirty;
	QAction *m_pMetricAction;
	Ui::CMainWindow *ui;
};

// =============================================================================

#endif // MAINWINDOW_H
