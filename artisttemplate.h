#pragma once

#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QStringList>
#include <QWidget>

#include "currency.h"

namespace Ui {
class ArtistTemplate;
}

class ArtistTemplate : public QMainWindow {
  Q_OBJECT

public:
  explicit ArtistTemplate(QWidget *parent = nullptr);
  ~ArtistTemplate();

  QComboBox *getArtistsBox();
  QString getArtistName();
  int getArtistIndex();
  QString getTrackName();
  int getTrackIndex();
  QComboBox *getTracksBox();

  void setArtists(QStringList &list);
  void setTracks(QStringList &list);
  void setStats(QAbstractTableModel &model);

  void setGlobalStreamsPerArtist(int quantity);
  void setIncomePerTrack(double income);
  void setIncomePerArtist(double income);
  void setFeeForAggregator(double fee);
  void setFeeForDistributor(double fee);
  void setProfitPerArtist(double profit);

  void flush();

private:
  static void setValue(ArtistTemplate *T, QLabel *label, double value);
  Currency cash; //(QPair<QString,QString>("USD","RUB"));

  Ui::ArtistTemplate *ui;
public slots:
  void activate();
};
