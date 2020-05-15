#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QSqlDatabase>
#include <QString>

#include "artisttemplate.h"

namespace Ui {
class Settings;
}

class Settings : public QMainWindow {
  Q_OBJECT

public:
  explicit Settings(QWidget *parent = nullptr);
  ~Settings();

private:
  Ui::Settings *ui;
  ArtistTemplate artistUI;
  QSettings settings;

  QString csvFilePath;
  QString projectFilePath;
  QString dbFilePath;
  QString bufferString; // костыль

  bool createDB();
  int insertQuery();
  int insertQuery(QString *query);
  QSqlDatabase db;

  int selectStats();
  int selectStats(const QString &Artist);
  int selectStats(const QString &Artist, const QString &Track);

  int selectArtist();
  int selectArtist(const QString &Artist);

  int selectTracks(const QString &Artist);

  int selectStreams(const QString &Artist);

  int selectIncome(const QString &Artist);
  int selectIncome(const QString &Artist, const QString &Track);

  int selectProfit(const QString &Artist);
  int selectProfit(const QString &Artist, const QString &Track);

public slots:
  void activate();

private slots:
  void on_create_triggered();
  void on_open_triggered();
  void on_openRecent_triggered();
  void on_saveAs_triggered();
  void on_save_triggered();

  bool openProject(QString &filePath);
  bool saveProject();

  void on_importCSV_triggered();

  void on_about_triggered();

signals:
  void isCreating();
  bool isCreated();
  void isInserting();
  bool isInserted();
  void isOpened();
  void isImporting();
  bool isImported();
};
