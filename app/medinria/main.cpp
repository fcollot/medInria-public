/* main.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Thu Sep 17 08:29:18 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Oct 14 14:33:19 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 139
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <QtGui>
#include <QtOpenGL>

#include "medMainWindow.h"

#include <dtkCore/dtkGlobal.h>

#include <dtkScript/dtkScriptManager.h>

#include <medCore/medPluginManager.h>
#include <medCore/medDataIndex.h>
#include <medSql/medDatabaseController.h>

int main(int argc, char *argv[])
{
    qRegisterMetaType<medDataIndex>("medDataIndex");

    QApplication application(argc, argv);
    application.setApplicationName("medinria");
    application.setApplicationVersion("0.0.1");
    application.setOrganizationName("inria");
    application.setOrganizationDomain("fr");
    application.setWindowIcon(QIcon(":/medinria.ico"));

    if (dtkApplicationArgumentsContain(&application, "-h") || dtkApplicationArgumentsContain(&application, "--help")) {
        qDebug() << "Usage: medinria [--no-fullscreen] [--stereo]";
        return 1;
    }

    QSplashScreen splash(QPixmap(":/pixmaps/medinria-splash.png"), Qt::WindowStaysOnTopHint);

    // Do not show the splash screen in debug builds because it hogs the 
    // foreground, hiding all other windows. This makes debugging the startup 
    // operations difficult.
#if ! defined( _DEBUG ) // && defined( WINDOWS )
    QObject::connect(medDatabaseController::instance().data(), SIGNAL(copyMessage(QString, int, QColor)), &splash, SLOT(showMessage(QString, int, QColor)) ); 
    splash.show();
#endif


    //if (!medDatabaseController::instance()->moveDatabase("c:/database"))
      //  qDebug() << "moving db failed";


    medPluginManager::instance()->initialize();
    dtkScriptManager::instance()->initialize();
    
 
    medMainWindow mainwindow;
    mainwindow.show();
   


    if(!dtkApplicationArgumentsContain(&application, "--no-fullscreen")
    && !dtkApplicationArgumentsContain(&application, "--wall"))
        mainwindow.setFullScreen(true);

    if(application.arguments().contains("--wall"))
        mainwindow.setWallScreen(true);

    if(application.arguments().contains("--stereo")) {
       QGLFormat format;
       format.setAlpha(true);
       format.setDoubleBuffer(true);
       format.setStereo(true);
       format.setDirectRendering(true);
       QGLFormat::setDefaultFormat(format);
    }

    splash.finish(&mainwindow);

    int status = application.exec();
    
    medPluginManager::instance()->uninitialize();
    dtkScriptManager::instance()->uninitialize();
    

    return status;
}
