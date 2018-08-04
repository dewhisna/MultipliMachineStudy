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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "conversion.h"

#include <QMenu>
#include <QApplication>
#include <QMessageBox>

#include "CustomerDialog.h"
#include "FormationCalcDialog.h"
#include "ProductionCalcDialog.h"
#include "GradeCalcDialog.h"
#include "HeadBoxDilutionDialog.h"
#include "FormConstDialog.h"
#include "PipeFlowCalcDialog.h"
#include "AreaFlowCalcDialog.h"

// =============================================================================

CMainWindow::CMainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_bMetric(false),
	m_bHaveTextOutput(false),
	m_bDirty(false),
	m_pMetricAction(nullptr),
	ui(new Ui::CMainWindow)
{
	ui->setupUi(this);

	QAction *pAction;

	// Setup Main Menu:
	// ----------------
	// --- File Menu
	QMenu *pFileMenu = ui->menuBar->addMenu(tr("&File", "MainMenu"));
	pAction = pFileMenu->addAction(QIcon(":/res/file-new-icon2.png"), tr("&New", "MainMenu"), this, SLOT(en_NewFile()), QKeySequence(Qt::CTRL + Qt::Key_N));
	pAction->setStatusTip(tr("Create New Text Document", "MainMenu"));
	pAction->setToolTip(tr("Create New Text Document", "MainMenu"));
	ui->mainToolBar->addAction(pAction);
	pAction = pFileMenu->addAction(QIcon(":/res/open-file-icon3.png"), tr("&Open", "MainMenu"), this, SLOT(en_OpenFile()), QKeySequence(Qt::CTRL + Qt::Key_O));
	pAction->setStatusTip(tr("Open Text Document", "MainMenu"));
	pAction->setToolTip(tr("Open Text Document", "MainMenu"));
	ui->mainToolBar->addAction(pAction);
	pAction = pFileMenu->addAction(QIcon(":/res/save-file-icon3.png"), tr("&Save", "MainMenu"), this, SLOT(en_SaveFile()), QKeySequence(Qt::CTRL + Qt::Key_S));
	pAction->setStatusTip(tr("Save Text Document", "MainMenu"));
	pAction->setToolTip(tr("Save Text Document", "MainMenu"));
	ui->mainToolBar->addAction(pAction);
	pAction = pFileMenu->addAction(tr("Save &As", "MainMenu"), this, SLOT(en_SaveFileAs()));
	pAction->setStatusTip(tr("Save Text Document As", "MainMenu"));
	pAction->setToolTip(tr("Save Text Document As", "MainMenu"));
	pFileMenu->addSeparator();
	ui->mainToolBar->addSeparator();
	pAction = pFileMenu->addAction(QIcon(":/res/printer1.png"), tr("&Print", "MainMenu"), this, SLOT(en_PrintFile()));
	pAction->setStatusTip(tr("Print Text Document", "MainMenu"));
	pAction->setToolTip(tr("Print Text Document", "MainMenu"));
	ui->mainToolBar->addAction(pAction);
	pAction = pFileMenu->addAction(tr("P&rint Setup", "MainMenu"), this, SLOT(en_PrintSetup()));
	pAction->setStatusTip(tr("Printer Setup", "MainMenu"));
	pAction->setToolTip(tr("Printer Setup", "MainMenu"));
	pFileMenu->addSeparator();
	ui->mainToolBar->addSeparator();
	pAction = pFileMenu->addAction(QIcon(":/res/exit.png"), tr("E&xit", "MainMenu"), QCoreApplication::instance(), SLOT(quit()), QKeySequence(Qt::CTRL + Qt::Key_Q));
	pAction->setStatusTip(tr("Quit Application", "MainMenu"));
	pAction->setToolTip(tr("Quit Application", "MainMenu"));
	ui->mainToolBar->addAction(pAction);

	// --- Edit Menu
	QMenu *pEditMenu = ui->menuBar->addMenu(tr("&Edit", "MainMenu"));
	pAction = pEditMenu->addAction(tr("&Undo", "MainMenu"), ui->editMainText, SLOT(undo()), QKeySequence(Qt::CTRL + Qt::Key_Z));
	pAction->setStatusTip(tr("Undo last edit"));
	pAction->setToolTip(tr("Undo"));
	connect(ui->editMainText, SIGNAL(undoAvailable(bool)), pAction, SLOT(setEnabled(bool)));
	pAction = pEditMenu->addAction(tr("&Redo", "MainMenu"), ui->editMainText, SLOT(redo()), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_K));
	pAction->setStatusTip(tr("Redo last edit"));
	pAction->setToolTip(tr("Redo Last Edit"));
	connect(ui->editMainText, SIGNAL(redoAvailable(bool)), pAction, SLOT(setEnabled(bool)));
	pEditMenu->addSeparator();
	pAction = pEditMenu->addAction(tr("Cu&t", "MainMenu"), ui->editMainText, SLOT(cut()), QKeySequence(Qt::SHIFT + Qt::Key_Delete));
	pAction->setStatusTip(tr("Cut Text"));
	pAction->setToolTip(tr("Cut Text"));
	connect(ui->editMainText, SIGNAL(copyAvailable(bool)), pAction, SLOT(setEnabled(bool)));
	pAction = pEditMenu->addAction(tr("&Copy", "MainMenu"), ui->editMainText, SLOT(copy()), QKeySequence(Qt::CTRL + Qt::Key_Insert));
	pAction->setStatusTip(tr("Copy Text"));
	pAction->setToolTip(tr("Copy Text"));
	connect(ui->editMainText, SIGNAL(copyAvailable(bool)), pAction, SLOT(setEnabled(bool)));
	pAction = pEditMenu->addAction(tr("&Paste", "MainMenu"), ui->editMainText, SLOT(paste()), QKeySequence(Qt::SHIFT + Qt::Key_Insert));
	pAction->setStatusTip(tr("Paste Text"));
	pAction->setToolTip(tr("Paste Text"));
	pAction = pEditMenu->addAction(tr("Select &All", "MainMenu"), ui->editMainText, SLOT(selectAll()), QKeySequence(Qt::CTRL + Qt::Key_A));
	pAction->setStatusTip(tr("Select All"));
	pAction->setToolTip(tr("Select All"));
	pEditMenu->addSeparator();
	pAction = pEditMenu->addAction(tr("&Search", "MainMenu"), this, SLOT(en_Search()), QKeySequence(Qt::CTRL + Qt::Key_F));
	pAction->setStatusTip(tr("Search"));
	pAction->setToolTip(tr("Search"));
	pAction = pEditMenu->addAction(tr("Search and &Replace", "MainMenu"), this, SLOT(en_SearchReplace()), QKeySequence(Qt::CTRL + Qt::Key_H));
	pAction->setStatusTip(tr("Search and Replace"));
	pAction->setToolTip(tr("Search and Replace"));

	// --- Setup Menu
	QMenu *pSetupMenu = ui->menuBar->addMenu(tr("&Setup", "MainMenu"));
	pAction = pSetupMenu->addAction(tr("&Metric", "MainMenu"), this, SLOT(en_SetupMetric()));
	pAction->setStatusTip(tr("Select Metric units or Imperial units"));
	pAction->setToolTip(tr("Select Metric units or Imperial units"));
	pAction->setCheckable(true);
	m_pMetricAction = pAction;
	connect(this, SIGNAL(metricModeSelected(bool)), pAction, SLOT(setChecked(bool)));

	// --- Formation Menu
	QMenu *pFormationMenu = ui->menuBar->addMenu(tr("F&ormation", "MainMenu"));
	pAction = pFormationMenu->addAction(tr("&Sheet Formation", "MainMenu"), this, SLOT(en_FormationSheetFormation()));
	pAction->setStatusTip(tr("Calculate Sheet Formation"));
	pAction->setToolTip(tr("Calculate Sheet Formation"));
	pAction = pFormationMenu->addAction(tr("&Production", "MainMenu"), this, SLOT(en_FormationProduction()));
	pAction->setStatusTip(tr("Calculate Production"));
	pAction->setToolTip(tr("Calculate Production"));
	pAction = pFormationMenu->addAction(tr("&Grade", "MainMenu"), this, SLOT(en_FormationGrade()));
	pAction->setStatusTip(tr("Calculate Grade"));
	pAction->setToolTip(tr("Calculate Grade"));
	pAction = pFormationMenu->addAction(tr("&Headbox Dilution"), this, SLOT(en_FormationHeadboxDilution()));
	pAction->setStatusTip(tr("Calculate Headbox Dilution"));
	pAction->setToolTip(tr("Calculate Headbox Dilution"));
	pAction = pFormationMenu->addAction(tr("&Formation Const"), this, SLOT(en_FormationFormationConst()));
	pAction->setStatusTip(tr("Calculate Formation Constant"));
	pAction->setToolTip(tr("Calculate Formation Constant"));

	// --- Flow Menu
	QMenu *pFlowMenu = ui->menuBar->addMenu(tr("F&low", "MainMenu"));
	pAction = pFlowMenu->addAction(tr("&Pipe", "MainMenu"), this, SLOT(en_FlowPipe()));
	pAction->setStatusTip(tr("Calculate Pipe Flow"));
	pAction->setToolTip(tr("Calculate Pipe Flow"));
	pAction = pFlowMenu->addAction(tr("&Area", "MainMenu"), this, SLOT(en_FlowArea()));
	pAction->setStatusTip(tr("Calculate Area Flow"));
	pAction->setToolTip(tr("Calculate Area Flow"));

	// --- Help Menu
	QMenu *pHelpMenu = ui->menuBar->addMenu(tr("&Help", "MainMenu"));
	pAction = pHelpMenu->addAction(tr("&Help", "MainMenu"), this, SLOT(en_HelpHelp()));
	pAction->setStatusTip(tr("Display Help Documentation"));
	pAction->setToolTip(tr("Display Help Documentation"));
	pAction = pHelpMenu->addAction(tr("About &Qt", "MainMenu"), QApplication::instance(), SLOT(aboutQt()));
	pAction->setStatusTip(tr("Details about the Qt Library used"));
	pAction->setToolTip(tr("Details about the Qt Library used"));
	pAction = pHelpMenu->addAction(tr("&About Machine Study", "MainMenu"), this, SLOT(en_HelpAbout()));
	pAction->setStatusTip(tr("Details About Multipli Machine Study"));
	pAction->setToolTip(tr("Details About Multipli Machine Study"));

	connect(ui->editMainText, SIGNAL(textChanged()), this, SLOT(en_TextChanged()));
}

CMainWindow::~CMainWindow()
{
	delete ui;
}

// -----------------------------------------------------------------------------

void CMainWindow::en_NewFile()
{
}

void CMainWindow::en_OpenFile()
{
}

void CMainWindow::en_SaveFile()
{
}

void CMainWindow::en_SaveFileAs()
{
}

void CMainWindow::en_PrintFile()
{
}

void CMainWindow::en_PrintSetup()
{
}

// -------------------------------------

void CMainWindow::en_Search()
{
}

void CMainWindow::en_SearchReplace()
{
}

// -------------------------------------

void CMainWindow::en_SetupMetric()
{
	assert(m_pMetricAction != nullptr);
	m_bMetric = !m_bMetric;
	m_pMetricAction->setChecked(m_bMetric);
	emit metricModeSelected(m_bMetric);
}

// -------------------------------------

void CMainWindow::TextOutputStart()
{
	if (m_bHaveTextOutput) {
		int nAppendResult = QMessageBox::question(this, tr("Machine Study Print File Output"), tr("Append to Print File?"));
		if (nAppendResult == QMessageBox::No) {
			ui->editMainText->clear();
			m_bHaveTextOutput = false;
		}
	}

	if (!m_bHaveTextOutput) {
		ui->editMainText->append("Machine Study Data Printout");
		QString strTemp = "Machine Study Software";
		if (!QApplication::applicationVersion().isEmpty()) {
			strTemp += QString(" v%1").arg(QApplication::applicationVersion());
		}
		ui->editMainText->append(strTemp);
		ui->editMainText->append("Copyright(c)2018 Multipli Machinery");
		ui->editMainText->append(QString());
		m_bHaveTextOutput = true;
	}

	CCustomerDialog dlgCustomer(this);
	dlgCustomer.m_DialogValues.m_strCustomer = m_strLastCustomer;
	int nResult = dlgCustomer.exec();
	if (nResult == QDialog::Accepted) {
		m_strLastCustomer = dlgCustomer.m_DialogValues.m_strCustomer;
		if (!dlgCustomer.m_DialogValues.m_strCustomer.isEmpty()) {
			ui->editMainText->append("Customer: " + dlgCustomer.m_DialogValues.m_strCustomer);
			ui->editMainText->append(QString());
		}
	}
}

// -------------------------------------

void CMainWindow::en_FormationSheetFormation()
{
	CFormationCalcDialog dlgForm(true, m_bMetric, this);
	int nResult = dlgForm.exec();
	if (nResult == QDialog::Accepted) {
		TextOutputStart();

		QString strTemp;
		ui->editMainText->append("Sheet Formation Calculation Window:");
		ui->editMainText->append("-----------------------------------");
		strTemp = "Number of Vats: ";
		strTemp += QString().setNum(dlgForm.m_DialogValues.m_nNumberOfVats);
		ui->editMainText->append(strTemp);
		ui->editMainText->append(QString());

		if (dlgForm.m_DialogValues.m_bAutoCalc) {
			strTemp = "Caliper (points): ";
			if (m_bMetric) {
				strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nCaliper, CaliperConv));
			} else {
				strTemp += QString().setNum(dlgForm.m_DialogValues.m_nCaliper);
			}
			ui->editMainText->append(strTemp);
			if (m_bMetric) {
				strTemp = "Sheet Density (grams/point): ";
				strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nDensity, DensityConv));
			} else {
				strTemp = "Sheet Density (lbs/point): ";
				strTemp += QString().setNum(dlgForm.m_DialogValues.m_nDensity);
			}
			ui->editMainText->append(strTemp);
			if (m_bMetric) {
				strTemp = "Basis Weight (grams/M Sq): ";
				strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nBasisWeight, BasisWeightConv));
			} else {
				strTemp = "Basis Weight (lbs/1K Sq. Ft): ";
				strTemp += QString().setNum(dlgForm.m_DialogValues.m_nBasisWeight);
			}
			ui->editMainText->append(strTemp);
			if (m_bMetric) {
				strTemp = "Sheet Trim Width (mm): ";
				strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nSheetWidth, SheetWidthConv));
			} else {
				strTemp = "Sheet Trim Width (in.): ";
				strTemp += QString().setNum(dlgForm.m_DialogValues.m_nSheetWidth);
			}
			ui->editMainText->append(strTemp);
			if (m_bMetric) {
				strTemp = "Formation Constant (kilograms/mm of Cylinder): ";
				strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nFormConst, FormConv));
			} else {
				strTemp = "Formation Constant (tons/in. of Cylinder): ";
				strTemp += QString().setNum(dlgForm.m_DialogValues.m_nFormConst);
			}
			ui->editMainText->append(strTemp);
			if (!dlgForm.m_DialogValues.m_bOtherFormConst) {
				ui->editMainText->append("    Based On:");
				strTemp = "        Cylinder Size: ";
				switch (dlgForm.m_DialogValues.m_nCylinderSize) {
					case CFormationCalcDialog::CSE_36in:
						strTemp += "36 in.";
						break;
					case CFormationCalcDialog::CSE_42in:
						strTemp += "42 in.";
						break;
					case CFormationCalcDialog::CSE_48in:
						strTemp += "48 in.";
						break;
					default:
						strTemp += "Unknown";
						break;
				}
				ui->editMainText->append(strTemp);
				strTemp = "        Board Type: ";
				switch(dlgForm.m_DialogValues.m_nBoardType) {
					case CFormationCalcDialog::BTE_FoldingBoxBoard:
						strTemp += "Folding Box Board";
						break;
					case CFormationCalcDialog::BTE_TubeBoard:
						strTemp += "Tube Board";
						break;
					default:
						strTemp += "Unknown";
						break;
				}
				ui->editMainText->append(strTemp);
			}
			ui->editMainText->append(QString());
			if (m_bMetric) {
				strTemp = "Maximum Kilograms Per Day: ";
				strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nYieldPerDay, YieldPerDayConv));
			} else {
				strTemp = "Maximum Tons Per Day: ";
				strTemp += QString().setNum(dlgForm.m_DialogValues.m_nYieldPerDay);
			}
			ui->editMainText->append(strTemp);
			ui->editMainText->append(QString());
			if (m_bMetric) {
				strTemp = "Required Machine Speed (Meters/Min): ";
				strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nMachineSpeed, MachineSpeedConv));
			} else {
				strTemp = "Required Machine Speed (Ft/Min): ";
				strTemp += QString().setNum(dlgForm.m_DialogValues.m_nMachineSpeed);
			}
			ui->editMainText->append(strTemp);
		} else {
			if (m_bMetric) {
				strTemp = "Kilograms Per Day: ";
				strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nYieldPerDay, YieldPerDayConv));
			} else {
				strTemp = "Tons Per Day: ";
				strTemp += QString().setNum(dlgForm.m_DialogValues.m_nYieldPerDay);
			}
			ui->editMainText->append(strTemp);
			ui->editMainText->append(QString());
			if (m_bMetric) {
				strTemp = "Machine Speed (Meters/Min): ";
				strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nMachineSpeed, MachineSpeedConv));
			} else {
				strTemp = "Machine Speed (Ft/Min): ";
				strTemp += QString().setNum(dlgForm.m_DialogValues.m_nMachineSpeed);
			}
			ui->editMainText->append(strTemp);
		}
		strTemp = "Machine Efficiency (%): ";
		strTemp += QString().setNum(dlgForm.m_DialogValues.m_nMachineEfficiency*100.0);
		ui->editMainText->append(strTemp);
		ui->editMainText->append(QString());
		strTemp = "Consistency (%): ";
		strTemp += QString().setNum(dlgForm.m_DialogValues.m_nConsistency*100.0);
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Liters Per Minute (total): ";
			strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nWaterNeeded, WaterNeededConv));
		} else {
			strTemp = "Gallons Per Minute (total): ";
			strTemp += QString().setNum(dlgForm.m_DialogValues.m_nWaterNeeded);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Liters Per Minute (per vat): ";
			strTemp += QString().setNum(toMetric(dlgForm.m_DialogValues.m_nWaterNeededPerVat, WaterNeededConv));
		} else {
			strTemp = "Gallons Per Minute (per vat): ";
			strTemp += QString().setNum(dlgForm.m_DialogValues.m_nWaterNeededPerVat);
		}
		ui->editMainText->append(strTemp);
		ui->editMainText->append(QString());
		ui->editMainText->append(QString());
	}
}

void CMainWindow::en_FormationProduction()
{
	CProductionCalcDialog dlgProd(true, m_bMetric, this);
	int nResult = dlgProd.exec();
	if (nResult == QDialog::Accepted) {
		TextOutputStart();

		QString strTemp;
		ui->editMainText->append("Production Calculation Window:");
		ui->editMainText->append("------------------------------");
		if (m_bMetric) {
			strTemp = "Sheet Trim Width (mm): ";
			strTemp += QString().setNum(toMetric(dlgProd.m_DialogValues.m_nSheetWidth, SheetWidthConv));
		} else {
			strTemp = "Sheet Trim Width (in.): ";
			strTemp += QString().setNum(dlgProd.m_DialogValues.m_nSheetWidth);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Machine Speed (Meters/Min): ";
			strTemp += QString().setNum(toMetric(dlgProd.m_DialogValues.m_nMachineSpeed, MachineSpeedConv));
		} else {
			strTemp = "Machine Speed (Ft/Min): ";
			strTemp += QString().setNum(dlgProd.m_DialogValues.m_nMachineSpeed);
		}
		ui->editMainText->append(strTemp);
		strTemp = "Machine Efficiency (%): ";
		strTemp += QString().setNum(dlgProd.m_DialogValues.m_nMachineEfficiency*100.0);
		ui->editMainText->append(strTemp);
		strTemp = "Caliper (points): ";
		if (m_bMetric) {
			strTemp += QString().setNum(toMetric(dlgProd.m_DialogValues.m_nCaliper, CaliperConv));
		} else {
			strTemp += QString().setNum(dlgProd.m_DialogValues.m_nCaliper);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Sheet Density (grams/point): ";
			strTemp += QString().setNum(toMetric(dlgProd.m_DialogValues.m_nDensity, DensityConv));
		} else {
			strTemp = "Sheet Density (lbs/point): ";
			strTemp += QString().setNum(dlgProd.m_DialogValues.m_nDensity);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Basis Weight (grams/M Sq): ";
			strTemp += QString().setNum(toMetric(dlgProd.m_DialogValues.m_nBasisWeight, BasisWeightConv));
		} else {
			strTemp = "Basis Weight (lbs/1K Sq. Ft): ";
			strTemp += QString().setNum(dlgProd.m_DialogValues.m_nBasisWeight);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Kilograms Per Day: ";
			strTemp += QString().setNum(toMetric(dlgProd.m_DialogValues.m_nYieldPerDay, YieldPerDayConv));
		} else {
			strTemp = "Tons Per Day: ";
			strTemp += QString().setNum(dlgProd.m_DialogValues.m_nYieldPerDay);
		}
		ui->editMainText->append(strTemp);
		ui->editMainText->append(QString());
		ui->editMainText->append(QString());
	}
}

void CMainWindow::en_FormationGrade()
{
	CGradeCalcDialog dlgGrade(true, m_bMetric, this);
	int nResult = dlgGrade.exec();
	if (nResult == QDialog::Accepted) {
		TextOutputStart();

		QString strTemp;
		ui->editMainText->append("Grade Calculation Window:");
		ui->editMainText->append("-------------------------");
		strTemp = "Caliper (points): ";
		if (m_bMetric) {
			strTemp += QString().setNum(toMetric(dlgGrade.m_DialogValues.m_nCaliper, CaliperConv));
		} else {
			strTemp += QString().setNum(dlgGrade.m_DialogValues.m_nCaliper);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Sheet Density (grams/point): ";
			strTemp += QString().setNum(toMetric(dlgGrade.m_DialogValues.m_nDensity, DensityConv));
		} else {
			strTemp = "Sheet Density (lbs/point): ";
			strTemp += QString().setNum(dlgGrade.m_DialogValues.m_nDensity);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Basis Weight (grams/M Sq): ";
			strTemp += QString().setNum(toMetric(dlgGrade.m_DialogValues.m_nBasisWeight, BasisWeightConv));
		} else {
			strTemp = "Basis Weight (lbs/1K Sq. Ft): ";
			strTemp += QString().setNum(dlgGrade.m_DialogValues.m_nBasisWeight);
		}
		ui->editMainText->append(strTemp);
		ui->editMainText->append(QString());
		ui->editMainText->append(QString());
	}
}

void CMainWindow::en_FormationHeadboxDilution()
{
	CHeadBoxDilutionDialog dlgHeadBox(true, m_bMetric, this);
	int nResult = dlgHeadBox.exec();
	if (nResult == QDialog::Accepted) {
		TextOutputStart();

		QString strTemp;
		ui->editMainText->append("HeadBox Calculation Window:");
		ui->editMainText->append("---------------------------");
		if (m_bMetric) {
			strTemp = "Kilograms Per Day: ";
			strTemp += QString().setNum(toMetric(dlgHeadBox.m_DialogValues.m_nYieldPerDay, YieldPerDayConv));
		} else {
			strTemp = "Tons Per Day: ";
			strTemp += QString().setNum(dlgHeadBox.m_DialogValues.m_nYieldPerDay);
		}
		ui->editMainText->append(strTemp);
		strTemp = "Present Consistency (%): ";
		strTemp += QString().setNum(dlgHeadBox.m_DialogValues.m_nPresentConsistency*100.0);
		ui->editMainText->append(strTemp);
		strTemp = "New Consistency (%): ";
		strTemp += QString().setNum(dlgHeadBox.m_DialogValues.m_nNewConsistency*100.0);
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Water Needed (LPM): ";
			strTemp += QString().setNum(toMetric(dlgHeadBox.m_DialogValues.m_nWaterNeeded, WaterNeededConv));
		} else {
			strTemp = "Water Needed (GPM): ";
			strTemp += QString().setNum(dlgHeadBox.m_DialogValues.m_nWaterNeeded);
		}
		ui->editMainText->append(strTemp);
		ui->editMainText->append(QString());
		ui->editMainText->append(QString());
	}
}

void CMainWindow::en_FormationFormationConst()
{
	CFormConstDialog dlgFormConst(true, m_bMetric, this);
	int nResult = dlgFormConst.exec();
	if (nResult == QDialog::Accepted) {
		TextOutputStart();

		QString strTemp;
		ui->editMainText->append("Formation Constant Calculation Window:");
		ui->editMainText->append("--------------------------------------");
		if (m_bMetric) {
			strTemp = "Kilograms Per Day: ";
			strTemp += QString().setNum(toMetric(dlgFormConst.m_DialogValues.m_nYieldPerDay, YieldPerDayConv));
		} else {
			strTemp = "Tons Per Day: ";
			strTemp += QString().setNum(dlgFormConst.m_DialogValues.m_nYieldPerDay);
		}
		ui->editMainText->append(strTemp);
		strTemp = "Number of Vats: ";
		strTemp += QString().setNum(dlgFormConst.m_DialogValues.m_nNumberOfVats);
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Sheet Width (mm): ";
			strTemp += QString().setNum(toMetric(dlgFormConst.m_DialogValues.m_nSheetWidth, SheetWidthConv));
		} else {
			strTemp = "Sheet Width (in.): ";
			strTemp += QString().setNum(dlgFormConst.m_DialogValues.m_nSheetWidth);
		}
		ui->editMainText->append(strTemp);
		strTemp = "Machine Efficiency (%): ";
		strTemp += QString().setNum(dlgFormConst.m_DialogValues.m_nMachineEfficiency*100.0);
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Formation Constant (kilograms/mm of Cylinder): ";
			strTemp += QString().setNum(toMetric(dlgFormConst.m_DialogValues.m_nFormConst, FormConv));
		} else {
			strTemp = "Formation Constant (tons/in. of Cylinder): ";
			strTemp += QString().setNum(dlgFormConst.m_DialogValues.m_nFormConst);
		}
		ui->editMainText->append(strTemp);
		ui->editMainText->append(QString());
		ui->editMainText->append(QString());
	}
}

// -------------------------------------

void CMainWindow::en_FlowPipe()
{
	CPipeFlowCalcDialog dlgPipeFlow(true, m_bMetric, this);
	int nResult = dlgPipeFlow.exec();
	if (nResult == QDialog::Accepted) {
		TextOutputStart();

		QString strTemp;
		ui->editMainText->append("Pipe Sizing Calculation Window:");
		ui->editMainText->append("-------------------------------");
		if (m_bMetric) {
			strTemp = "Velocity (meters/sec): ";
			strTemp += QString().setNum(toMetric(dlgPipeFlow.m_DialogValues.m_nVelocity, VelocityConv));
		} else {
			strTemp = "Velocity (ft/sec): ";
			strTemp += QString().setNum(dlgPipeFlow.m_DialogValues.m_nVelocity);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Liters Per Minute: ";
			strTemp += QString().setNum(toMetric(dlgPipeFlow.m_DialogValues.m_nWaterNeeded, WaterNeededConv));
		} else {
			strTemp = "Gallons Per Minute: ";
			strTemp += QString().setNum(dlgPipeFlow.m_DialogValues.m_nWaterNeeded);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Pipe Size I.D. (mm): ";
			strTemp += QString().setNum(toMetric(dlgPipeFlow.m_DialogValues.m_nPipeSizeID, PipeConv));
		} else {
			strTemp = "Pipe Size I.D. (in.): ";
			strTemp += QString().setNum(dlgPipeFlow.m_DialogValues.m_nPipeSizeID);
		}
		ui->editMainText->append(strTemp);
		ui->editMainText->append(QString());
		ui->editMainText->append(QString());
	}
}

void CMainWindow::en_FlowArea()
{
	CAreaFlowCalcDialog dlgAreaFlow(true, m_bMetric, this);
	int nResult = dlgAreaFlow.exec();
	if (nResult == QDialog::Accepted) {
		TextOutputStart();

		QString strTemp;
		ui->editMainText->append("Area Sizing Calculation Window:");
		ui->editMainText->append("-------------------------------");
		if (m_bMetric) {
			strTemp = "Velocity (meters/sec): ";
			strTemp += QString().setNum(toMetric(dlgAreaFlow.m_DialogValues.m_nVelocity, VelocityConv));
		} else {
			strTemp = "Velocity (ft/sec): ";
			strTemp += QString().setNum(dlgAreaFlow.m_DialogValues.m_nVelocity);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Liters Per Minute: ";
			strTemp += QString().setNum(toMetric(dlgAreaFlow.m_DialogValues.m_nWaterNeeded, WaterNeededConv));
		} else {
			strTemp = "Gallons Per Minute: ";
			strTemp += QString().setNum(dlgAreaFlow.m_DialogValues.m_nWaterNeeded);
		}
		ui->editMainText->append(strTemp);
		if (m_bMetric) {
			strTemp = "Cross Sectional Area (Sq. mm): ";
			strTemp += QString().setNum(toMetric(dlgAreaFlow.m_DialogValues.m_nCrossSectArea, AreaConv));
		} else {
			strTemp = "Cross Sectional Area (Sq. In.): ";
			strTemp += QString().setNum(dlgAreaFlow.m_DialogValues.m_nCrossSectArea);
		}
		ui->editMainText->append(strTemp);
		ui->editMainText->append(QString());
		ui->editMainText->append(QString());
	}
}

// -------------------------------------

void CMainWindow::en_HelpHelp()
{
}

void CMainWindow::en_HelpAbout()
{
}

// -------------------------------------

void CMainWindow::en_InsertText(const QString &strText)
{
	ui->editMainText->append(strText);
	m_bHaveTextOutput = true;
}

void CMainWindow::en_TextChanged()
{
	m_bDirty = true;
}

// =============================================================================
