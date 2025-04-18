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

#include "MainWindow.h"
#include "version.h"

#include <QApplication>

#ifdef Q_OS_WIN
// Needed to call CreateMutex to lockout installer running while we are:
#include <windows.h>
#endif

QPointer<CMainWindow> g_pMyMainWindow = nullptr;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationVersion(VER_QT);
	a.setApplicationName(VER_APPNAME_STR_QT);
	a.setOrganizationName(VER_ORGNAME_STR_QT);
	a.setOrganizationDomain(VER_ORGDOMAIN_STR_QT);

	Q_INIT_RESOURCE(MachineStudy);

#ifdef Q_OS_WIN32
	a.setWindowIcon(QIcon(":/res/mmcms4.ico"));
#elif !defined(Q_OS_MAC)	// Normally, this would also include Mac, but Mac has its icon set in the project file.  Loading this one makes it fuzzy.
	a.setWindowIcon(QIcon(":/res/mmc4.png"));
#endif

#ifdef Q_OS_WIN
	HANDLE hMutex = CreateMutexW(NULL, false, L"MachineStudyMutex");
	assert(hMutex != NULL);
	// Note: System will automatically close the mutex object when we
	//	exit and InnoSetup actually suggest we leave it open
#endif

	CMainWindow w;
	g_pMyMainWindow = &w;
	w.show();

	return a.exec();
}
