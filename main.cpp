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
  //  static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] =
  //  "QT_DEVICE_PIXEL_RATIO"; if
  //  (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO) &&
  //      !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR") &&
  //      !qEnvironmentVariableIsSet("QT_SCALE_FACTOR") &&
  //      !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
  //    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  //  }
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("FromX Soft Company");
  QCoreApplication::setOrganizationDomain("imfromx.com");
  QCoreApplication::setApplicationName("Label Stats");
  QFontDatabase::addApplicationFont(":/main/res/Gora-Free.ttf");
  QFile::remove("log.txt");
  qInstallMessageHandler(Logging::messageHandler);
  MainWindow mainWin;
  Settings setWin;
  // ArtistTemplate artWin(&setWin);
  QObject::connect(&mainWin, &MainWindow::startButtonClicked, &setWin,
                   &Settings::activate);
  //  QObject::connect(&setWin, &Settings::isOpened, &artWin,
  //                   &ArtistTemplate::activate);
  mainWin.show();
  return a.exec();
}
