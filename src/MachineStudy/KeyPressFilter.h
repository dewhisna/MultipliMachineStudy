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

//
// KeyPress Event Filter for Enter Key
//

#ifndef KEY_PRESS_FILTER_H_
#define KEY_PRESS_FILTER_H_

#include <QObject>
#include <QEvent>
#include <QKeyEvent>

class CKeyPressEnterEater : public QObject
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event)
	{
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			bool res = QObject::eventFilter(obj, event);

			if ((keyEvent->key() == Qt::Key_Return) ||
				(keyEvent->key() == Qt::Key_Enter)) {
				return true;
			} else {
				return res;
			}
		} else {
			// standard event processing
			return QObject::eventFilter(obj, event);
		}
	}
};

#endif	// KEY_PRESS_FILTER_H_
