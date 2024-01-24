/*
 * main.cpp - startup routine for Veyon Master Application
 *
 * Copyright (c) 2004-2023 Tobias Junghans <tobydox@veyon.io>
 *
 * This file is part of Veyon - https://veyon.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include <QApplication>
#include <QGuiApplication>
#include <QSplashScreen>

#include <QLocalSocket>
#include <QLocalServer>
#include <QString>
#include <stdio.h>

#include "DocumentationFigureCreator.h"
#include "VeyonMaster.h"
#include "MainWindow.h"


int main( int argc, char** argv )
{
	VeyonCore::setupApplicationParameters();

	QApplication app( argc, argv );
	app.connect( &app, &QApplication::lastWindowClosed, &QApplication::quit );

	VeyonCore core( &app, VeyonCore::Component::Master, QStringLiteral("Master") );

	QString appKey = QStringLiteral("classroom-management-01");

	VeyonMaster masterCore( &core );

	QLocalSocket* socket = new QLocalSocket();
    socket->connectToServer(appKey);

    if (socket->isOpen()) {
        socket->close();
        socket->deleteLater();

        masterCore.mainWindow()->activateWindow();
        masterCore.mainWindow()->show();
        masterCore.mainWindow()->raise();
        masterCore.mainWindow()->setFocus();

        return 0;
    }

	 socket->deleteLater();
	 QLocalServer server;

#ifdef VEYON_DEBUG
	if( qEnvironmentVariableIsSet( "VEYON_MASTER_CREATE_DOC_FIGURES") )
	{
		DocumentationFigureCreator().run();
		return 0;
	}
#endif

	QSplashScreen splashScreen( QPixmap( QStringLiteral(":/master/splash.png") ) );
	splashScreen.show();

	if( MainWindow::initAuthentication() == false ||
		MainWindow::initAccessControl() == false )
	{
		return -1;
	}

    server.listen(appKey);

    masterCore.mainWindow()->activateWindow();

    if( masterCore.mainWindow() ){
        splashScreen.finish( masterCore.mainWindow() );
        masterCore.mainWindow()->show();
        masterCore.mainWindow()->raise();
        masterCore.mainWindow()->setFocus();
    }

	return core.exec();
}
