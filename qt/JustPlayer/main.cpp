#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <qsurfaceformat.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, true);
    // QSurfaceFormat fmt;
    // fmt.setVersion(3, 3);
    // fmt.setProfile(QSurfaceFormat::CoreProfile);
    // QSurfaceFormat::setDefaultFormat(fmt);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "JustPlayer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
