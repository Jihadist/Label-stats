#include <QFile>
#include <QFontDatabase>

#include "artisttemplate.h"
#include "logging.h"
#include "mainwindow.h"
#include "settings.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  qSetMessagePattern("%{file}(%{line}): %{message}");
  QGuiApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("FromX Soft Company");
  QCoreApplication::setOrganizationDomain("imfromx.com");
  QCoreApplication::setApplicationName("Label Stats");
  QFontDatabase::addApplicationFont(":/main/res/Gora-Free.ttf");

#ifndef QT_DEBUG
  QFile::remove("log.txt");
  qInstallMessageHandler(Logging::messageHandler);
#endif
  MainWindow mainWin;
  Settings setWin;
  QObject::connect(&mainWin, &MainWindow::startButtonClicked, &setWin,
                   &Settings::activate);
  mainWin.show();
  return a.exec();
}
