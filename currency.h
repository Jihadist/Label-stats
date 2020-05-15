#pragma once

#include <QObject>
#include <QUrl>
#include <QtNetwork>

class Currency : public QObject {
  Q_OBJECT
public:
  Currency(QString currencyPair = "USD/RUB");
  Currency(QPair<QString, QString> pair);
  double getRate() { return exchange_rate; }
  QPair<QString, QString> getPair() { return exchange_pair; }
  void refresh();

private:
  double exchange_rate;
  QPair<QString, QString> exchange_pair;
  QNetworkAccessManager qnam;
  QNetworkReply *reply;

  void startRequest(const QUrl &requestedUrl);

  void httpFinished();
  void httpReadyRead();
  void slotAuthenticationRequired(QNetworkReply *,
                                  QAuthenticator *authenticator);
#ifndef QT_NO_SSL
  void sslErrors(QNetworkReply *, const QList<QSslError> &errors);
#endif
};
