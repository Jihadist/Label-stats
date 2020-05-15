#include "logging.h"

void Logging::messageHandler(QtMsgType type, const QMessageLogContext &context,
                             const QString &msg) {
  QFile logFile("log.txt");
  logFile.open(QFile::Append | QFile::Text);
  // Открываем поток записи в файл
  QTextStream out(&logFile);
  // Записываем дату записи
  out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
  // По типу определяем, к какому уровню относится сообщение
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
  // Записываем в вывод категорию сообщения и само сообщение
  out << context.category << ": " << msg << endl;
  out.flush(); // Очищаем буферизированные данные
}

Q_LOGGING_CATEGORY(logDebug, "Debug")
Q_LOGGING_CATEGORY(logInfo, "Info")
Q_LOGGING_CATEGORY(logWarning, "Warning")
Q_LOGGING_CATEGORY(logCritical, "Critical")

// Logging::Logging()
//{

//}
