#include "currency.h"

#include <QMessageBox>
#include <QtWidgets>

#include <QJsonDocument>
#include <QJsonObject>

#if QT_CONFIG(ssl)
QString defaultUrl =
    "https://api.exchangeratesapi.io/latest?base=sell&symbols=buy";
#else
QString defaultUrl =
    "http://api.exchangeratesapi.io/latest?base=sell&symbols=buy";
#endif

Currency::Currency(QString currencyPair) {
  exchange_rate = 0.0;
  QStringList buffer = currencyPair.split("/");
  exchange_pair = QPair<QString, QString>(buffer.first(), buffer.last());
  defaultUrl.replace("sell", exchange_pair.first);
  defaultUrl.replace("buy", exchange_pair.second);
  startRequest(defaultUrl);
}

Currency::Currency(QPair<QString, QString> pair) {
  exchange_pair = pair;
  exchange_rate = 0.0;
  defaultUrl.replace("sell", exchange_pair.first);
  defaultUrl.replace("buy", exchange_pair.second);
  startRequest(defaultUrl);
}

void Currency::startRequest(const QUrl &requestedUrl) {
  reply = qnam.get(QNetworkRequest(requestedUrl));
  QObject::connect(reply, &QNetworkReply::finished, this,
                   &Currency::httpFinished);
  QObject::connect(reply, &QIODevice::readyRead, this,
                   &Currency::httpReadyRead);
}

void Currency::httpFinished() {

  if (reply->error()) {
    qDebug() << "Request failed: " << reply->error();
    reply->deleteLater();
    reply = nullptr;
    return;
  }
}

void Currency::httpReadyRead() {

  QJsonDocument doc(QJsonDocument::fromJson(reply->readAll()));

  if (!doc.isNull())
    if (doc.isObject()) {
      auto obj = doc.object();
      qDebug() << obj;
      auto obj_rates = obj.value("rates").toObject();
      qDebug() << obj_rates;
      exchange_rate = obj_rates.value(exchange_pair.second).toDouble();
      qDebug() << exchange_rate;

    } else

      qDebug() << "Document is not an object" << endl;
  else

    qDebug() << "Invalid JSON...\n" << doc << endl;
}

#ifndef QT_NO_SSL
void Currency::sslErrors(QNetworkReply *, const QList<QSslError> &errors) {
  QMessageBox msgBox;
  QString errorString;
  for (const QSslError &error : errors) {
    if (!errorString.isEmpty())
      errorString += '\n';
    errorString += error.errorString();
  }

  if (QMessageBox::warning(
          &msgBox, QObject::tr("SSL Errors"),
          QObject::tr("One or more SSL errors has occurred:\n%1")
              .arg(errorString),
          QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {

    reply->ignoreSslErrors();
  }
}
#endif
