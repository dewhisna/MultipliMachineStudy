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

#ifndef YIELDPERDAYDIALOG_H
#define YIELDPERDAYDIALOG_H

#include <QDialog>
#include <QPushButton>

// =============================================================================

namespace Ui {
	class CYieldPerDayDialog;
}

class CYieldPerDayDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CYieldPerDayDialog(bool bMetric, QWidget *parent = nullptr);
	~CYieldPerDayDialog();

	struct TDialogValues {
		TDialogValues()
			:	m_nYieldPerDay(0.0)
		{ }
		double m_nYieldPerDay;			// In Imperial Units (Tons Per Day), (calculated output only)
	} m_DialogValues;			// Values to set coming in or final values (after validation) going out

public slots:
	void setMetric(bool bMetric);

	virtual int exec() override;

protected slots:
	void en_changeYieldPerDay();

	virtual void accept() override;

protected:
	void setUIText();

	bool validateYieldPerDay(double *pnYieldPerDay, bool bPrompt);

private:
	bool m_bMetric;
	QPushButton *m_pOKButton;
	Ui::CYieldPerDayDialog *ui;
};

// =============================================================================

#endif // YIELDPERDAYDIALOG_H
