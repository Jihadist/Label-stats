#include "logging.h"

void Logging::messageHandler(QtMsgType type, const QMessageLogContext &context,
                             const QString &msg) {
  QFile logFile("log.txt");
  logFile.open(QFile::Append | QFile::Text);
  // Open stream to file
  QTextStream out(&logFile);
  // Log datetime
  out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
  // Detect type of msg
  switch (type) {
  case QtInfoMsg:
    out << "INF ";
    break;
  case QtDebugMsg:
    out << "DBG ";
    break;
  case QtWarningMsg:
    out << "WRN ";
    break;
  case QtCriticalMsg:
    out << "CRT ";
    break;
  case QtFatalMsg:
    out << "FTL ";
    break;
  }
  // Write category and msg
  out << context.category << ": " << msg << endl;
  out.flush(); // Flush buffer
}

Q_LOGGING_CATEGORY(logDebug, "Debug")
Q_LOGGING_CATEGORY(logInfo, "Info")
Q_LOGGING_CATEGORY(logWarning, "Warning")
Q_LOGGING_CATEGORY(logCritical, "Critical")
