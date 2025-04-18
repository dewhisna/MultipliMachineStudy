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

#include "HelpBrowser.h"

#include <QDesktopServices>

#include <assert.h>

// =============================================================================

CHelpBrowser::CHelpBrowser(QWidget *parent)
	:	QTextBrowser(parent)
{
	setOpenLinks(false);		// Needed so we can control navigation on links
	connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(en_anchorClicked(QUrl)));
}

void CHelpBrowser::setHelpEngine(QHelpEngine* pHelpEngine)
{
	m_pHelpEngine = pHelpEngine;
}

QVariant CHelpBrowser::loadResource(int type, const QUrl &name)
{
	if ((!m_pHelpEngine.isNull()) && (name.scheme() == "qthelp")) {
		return QVariant(m_pHelpEngine->fileData(name));
	} else {
		assert(false);		// Should always have a HelpEngine and en_anchorClicked() should navigate external links
		return QTextBrowser::loadResource(type, name);
	}
}

void CHelpBrowser::en_anchorClicked(const QUrl &link)
{
	if (link.scheme() == "qthelp") {
		if (!m_pHelpEngine.isNull()) setSource(link);
	} else {
		QDesktopServices::openUrl(link);
	}
}

// =============================================================================
