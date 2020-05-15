#include "settings.h"
#include "ui_settings.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QStringList>

#include <QMessageBox>

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "qtcsv/reader.h"
#include "qtcsv/stringdata.h"
#include "qtcsv/variantdata.h"
#include "qtcsv/writer.h"

Settings::Settings(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Settings) {
  ui->setupUi(this);
  db = QSqlDatabase::addDatabase("QSQLITE");
  QObject::connect(this, &Settings::isOpened, &artistUI,
                   &ArtistTemplate::activate);
  QObject::connect(
      artistUI.getArtistsBox(),
      QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
      [=](const QString &Artist) {
        qDebug() << Artist;
        if ((artistUI.getArtistIndex() == 0) | (Artist == "Все артисты"))
          qDebug() << "Error code:" << selectStats();
        else {
          qDebug() << "Error code:" << selectArtist(Artist);
        }
      });
  QObject::connect(
      artistUI.getTracksBox(),
      QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
      [=](const QString &Track) {
        if (artistUI.getTrackIndex() == -1)
          return;
        QString Artist = artistUI.getArtistsBox()->currentText();

        int ArtistCounter = artistUI.getArtistsBox()->count();
        int TrackCounter = artistUI.getTracksBox()->count();
        qDebug() << TrackCounter;
        qDebug() << ArtistCounter;
        qDebug() << Track;

        if (Track == "Все треки" && ArtistCounter > 0 && TrackCounter > 0) {
          qDebug() << "Выбраны все треки";
          // emit artistUI.getArtistsBox()->currentIndexChanged(Artist);
          bufferString = Artist;
          qDebug() << "Error code:" << selectArtist(Artist);
        } else {
          qDebug() << "Error code:" << selectStats(Artist, Track);
          qDebug() << "Error code:" << selectIncome(Artist, Track);
          qDebug() << "Error code:" << selectProfit(Artist, Track);
        }
      });
}

Settings::~Settings() { delete ui; }

bool Settings::createDB() {
  emit isCreating();
  if (csvFilePath.isEmpty() && !dbFilePath.isEmpty())
    return false;
  if (!db.open()) {
    qDebug() << "Couldn't open db";
  }
  QSqlQuery query;

  // db.setDatabaseName("test.dev.db");
  dbFilePath = "test.dev.db";
  db.setDatabaseName(dbFilePath);
  if (!db.open()) {
    qDebug() << "Database cannot be opened";
    return false;
  }
  // Suppression + creation of the table
  query.exec("DROP TABLE `tracks`;");
  if (!(query.exec(
          "CREATE TABLE `tracks` ( `UPC` int, `Cat#` null, `ISRC` text, "
          "`Quantity` int, `UnitGross` real, `TotalGross` real, `Fees` real, "
          "`%Share` int, `Subtotal` real, `Net(Local)` null, `Currency(Local)` "
          "null, `ExchangeRate` null, `Net` real, `Currency` text, `Label` "
          "text, `Artist` text, `AlbumTitle` text, `SongTitle` text, `SongMix` "
          "text, `RetailerTerritory` null, `CustomerTerritory` null, "
          "`3rdPartyRetailer` text, `Stream` int, `ProductType` text, "
          "`SaleType` text, `CustomerID` null, `TransactionMonth` text, "
          "`AccountedDate` text, `Shared` null, `ISWC` null, `CompShare` null, "
          "`Writers` null, `custom_id` null,primary "
          "key(`ISRC`,`3rdPartyRetailer`,`TransactionMonth`), FOREIGN "
          "KEY(`Artist`) REFERENCES `Artists`(`Artist`) ON UPDATE CASCADE)"))) {
    qDebug() << query.lastQuery();
    qDebug() << query.lastError();
    return false;
  }
  db.close();
  emit isCreated();
  return true;
}

int Settings::insertQuery() {
  emit isInserting();

  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QFile file(csvFilePath);
  // QFile file(":/accounted.csv");

  // Open the "excel.csv" file
  if (file.open(QIODevice::ReadOnly)) {
    file.readLine();
    QList<QStringList> readData;
    readData = QtCSV::Reader::readToList(file);

    for (const auto &row : readData) {
      QSqlQuery insertQuery;
      insertQuery.prepare(
          "INSERT into `tracks` (`UPC`, `Cat#`, "
          "`ISRC`,`Quantity`,`UnitGross` , `TotalGross` , `Fees` , `%Share` , "
          "`Subtotal` , `Net(Local)` , `Currency(Local)` , `ExchangeRate` , "
          "`Net` , `Currency` , `Label` , `Artist` , `AlbumTitle` , "
          "`SongTitle` "
          ", `SongMix` , `RetailerTerritory` , `CustomerTerritory` , "
          "`3rdPartyRetailer` , `Stream` , `ProductType` , `SaleType` , "
          "`CustomerID` , `TransactionMonth` , `AccountedDate` , `Shared`) "
          "VALUES "
          "(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
      for (const auto &column : row) {
        insertQuery.addBindValue(column);
      }
      if (!insertQuery.exec()) {
        qDebug() << insertQuery.lastQuery();
        qDebug() << "error";
        qDebug() << insertQuery.lastError();
        return -2;
      } /*else
  qDebug() << "Success";*/
    }

    file.close();
  }
  db.close();
  emit isInserted();
  return 1;
}

void Settings::activate() { this->show(); }

int Settings::selectStats() {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery *selectQuery = new QSqlQuery;
  QSqlQueryModel *model = new QSqlQueryModel;
  selectQuery->prepare(
      "select `3rdPartyRetailer` as `Retailer`, sum(`TotalGross`) as `Income`, "
      "`%Share` from `tracks` group by `Retailer` order by "
      "`Income` desc");
  if (selectQuery->exec()) {
    model->setQuery(*selectQuery);
    artistUI.setStats(*model);
  } else {
    qDebug() << selectQuery->lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery->lastError();
    return -2;
  }
  if (!selectStreams("*"))
    return -3;
  if (!selectIncome("*"))
    return -4;
  if (!selectProfit("*"))
    return -5;
  db.close();
  return 1;
}

int Settings::selectArtist() {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery selectQuery;
  QStringList artistsList;
  artistsList.push_back("Все артисты");
  selectQuery.prepare(
      "select distinct `Artist` from `tracks` order by `Artist`");
  if (selectQuery.exec()) {
    while (selectQuery.next()) {
      artistsList.push_back(selectQuery.value(0).toString());
      qDebug() << selectQuery.value(0).toString();
    }
  } else {
    qDebug() << selectQuery.lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery.lastError();
    return -2;
  }
  db.close();
  artistUI.setArtists(artistsList);
  return 1;
}

int Settings::selectArtist(const QString &Artist) {
  if (!selectTracks(Artist))
    return -1;
  if (!selectStats(Artist))
    return -2;
  if (!selectStreams(Artist))
    return -3;
  if (!selectIncome(Artist))
    return -4;
  if (!selectProfit(Artist))
    return -5;
  return 1;
}

int Settings::selectTracks(const QString &Artist) {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery selectQuery;
  QStringList tracksList;
  tracksList.push_back("Все треки");
  selectQuery.prepare("select distinct `SongTitle` from `tracks` where "
                      "`Artist`=? order by `SongTitle`");
  selectQuery.addBindValue(Artist);
  if (selectQuery.exec()) {
    while (selectQuery.next()) {
      tracksList.push_back(selectQuery.value(0).toString());
      qDebug() << selectQuery.value(0).toString();
    }
  } else {
    qDebug() << selectQuery.lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery.lastError();
    return -2;
  }
  if (Artist != bufferString)
    artistUI.setTracks(tracksList);
  db.close();
  return 1;
}

int Settings::selectStats(const QString &Artist) {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery *selectQuery = new QSqlQuery;
  QSqlQueryModel *model = new QSqlQueryModel;
  selectQuery->prepare(
      "select `3rdPartyRetailer` as `Retailer`, sum(`TotalGross`) as `Income`, "
      "`%Share` from `tracks` where `Artist`=? group by `Retailer` order by "
      "`Income` desc");
  selectQuery->addBindValue(Artist);
  if (selectQuery->exec()) {
    model->setQuery(*selectQuery);
    artistUI.setStats(*model);
  } else {
    qDebug() << selectQuery->lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery->lastError();
    return -2;
  }
  db.close();
  return 1;
}

int Settings::selectStreams(const QString &Artist) {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery *selectQuery = new QSqlQuery;
  selectQuery->prepare("select sum(`Quantity`) from `tracks` where `Artist`=?");
  selectQuery->addBindValue(Artist);
  if (selectQuery->exec() && selectQuery->next()) {

    artistUI.setGlobalStreamsPerArtist(selectQuery->value(0).toInt());
  } else {
    qDebug() << selectQuery->lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery->lastError();
    return -2;
  }
  db.close();
  return 1;
}

int Settings::selectIncome(const QString &Artist) {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery *selectQuery = new QSqlQuery;
  selectQuery->prepare(
      "select sum(`TotalGross`) from `tracks` where `Artist`=?");
  selectQuery->addBindValue(Artist);
  if (selectQuery->exec() && selectQuery->next()) {
    artistUI.setIncomePerArtist(selectQuery->value(0).toDouble());
    // artistUI.setGlobalStreamsPerArtist(selectQuery->value(0).toInt());
  } else {
    qDebug() << selectQuery->lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery->lastError();
    return -2;
  }
  db.close();
  return 1;
}

int Settings::selectProfit(const QString &Artist) {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery *selectQuery = new QSqlQuery;
  selectQuery->prepare("select sum(`Net`) from `tracks` where `Artist`=?");
  selectQuery->addBindValue(Artist);
  if (selectQuery->exec() && selectQuery->next()) {
    artistUI.setProfitPerArtist(selectQuery->value(0).toDouble());
  } else {
    qDebug() << selectQuery->lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery->lastError();
    return -2;
  }
  db.close();
  return 1;
}

int Settings::selectStats(const QString &Artist, const QString &Track) {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery *selectQuery = new QSqlQuery;
  QSqlQueryModel *model = new QSqlQueryModel;
  selectQuery->prepare(
      "select `3rdPartyRetailer` as `Retailer`, sum(`TotalGross`) as `Income`, "
      "`%Share` from `tracks` where `Artist`=? and `SongTitle`=?  group by "
      "`Retailer` order by "
      "`Income` desc");
  selectQuery->addBindValue(Artist);
  selectQuery->addBindValue(Track);
  if (selectQuery->exec()) {
    model->setQuery(*selectQuery);
    artistUI.setStats(*model);
  } else {
    qDebug() << selectQuery->lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery->lastError();
    return -2;
  }
  db.close();
  return 1;
}

int Settings::selectIncome(const QString &Artist, const QString &Track) {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery *selectQuery = new QSqlQuery;
  selectQuery->prepare("select sum(`TotalGross`) from `tracks` where "
                       "`Artist`=? and `SongTitle`=?");
  selectQuery->addBindValue(Artist);
  selectQuery->addBindValue(Track);
  if (selectQuery->exec() && selectQuery->next()) {
    artistUI.setIncomePerTrack(selectQuery->value(0).toDouble());
    // artistUI.setIncomePerArtist(selectQuery->value(0).toDouble());
    // artistUI.setGlobalStreamsPerArtist(selectQuery->value(0).toInt());
  } else {
    qDebug() << selectQuery->lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery->lastError();
    return -2;
  }
  db.close();
  return 1;
}

int Settings::selectProfit(const QString &Artist, const QString &Track) {
  if (!db.open()) {
    qDebug() << "Couldn't open db";
    return -1;
  }
  QSqlQuery *selectQuery = new QSqlQuery;
  selectQuery->prepare("select sum(`TotalGross`) from `tracks` where "
                       "`Artist`=? and `SongTitle`=?");
  selectQuery->addBindValue(Artist);
  selectQuery->addBindValue(Track);
  if (selectQuery->exec() && selectQuery->next()) {
    qDebug() << selectQuery->value(0).toDouble();
    // artistUI.setIncomePerArtist(selectQuery->value(0).toDouble());
    // artistUI.setGlobalStreamsPerArtist(selectQuery->value(0).toInt());
  } else {
    qDebug() << selectQuery->lastQuery();
    qDebug() << "error";
    qDebug() << selectQuery->lastError();
    return -2;
  }
  db.close();
  return 1;
}

void Settings::on_create_triggered() {
  csvFilePath = QFileDialog::getOpenFileName(
      this, tr("Open OneRPM stats"), "",
      tr("OneRPM statistics (*.csv);;All Files (*)"));
  if (!csvFilePath.isEmpty())
    if (createDB())
      if (insertQuery()) {
        emit isOpened();
        selectArtist();
      }
}

void Settings::on_open_triggered() {
  projectFilePath = QFileDialog::getOpenFileName(
      this, tr("Open label statistics project"), "",
      tr("Label statistics (*.json);;All Files (*)"));
  if (!projectFilePath.isEmpty())
    openProject(projectFilePath);
}

void Settings::on_openRecent_triggered() {
  QString path = settings.value("LatestProjectPath").toString();
  if (!path.isEmpty())
    openProject(path);
  else
    qDebug() << "No recent project";
}

bool Settings::openProject(QString &filePath) {
  qDebug() << filePath;
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Cannot read the file %s", file.fileName().toStdString().c_str());
  } else {
    QJsonDocument project(QJsonDocument::fromJson(file.readAll()));
    if (project.isObject()) {
      dbFilePath = project.object().value("DBFilePath").toString();
      qDebug() << dbFilePath;
      db.setDatabaseName(dbFilePath);
      emit isOpened();
      selectArtist();
    }
    file.close();
    return true;
  }
  return false;
}

void Settings::on_saveAs_triggered() {
  projectFilePath = QFileDialog::getSaveFileName(
      this, tr("Save label statistics project"), "",
      tr("Label statistics (*.json);;All Files (*)"));
  if (!projectFilePath.isEmpty() && !dbFilePath.isEmpty())
    saveProject();
}

bool Settings::saveProject() {
  QDir currentProjectDir(QDir::current());
  // QString path = currentProjectDir.absoluteFilePath(projectFilePath);
  settings.setValue("LatestProjectPath",
                    currentProjectDir.absoluteFilePath(projectFilePath));
  QJsonObject object{{"DBFilePath", dbFilePath}};
  QJsonDocument project(object);

  QFile file(projectFilePath);
  if (!file.open(QIODevice::ReadWrite)) {
    qWarning("Cannot create the file %s",
             file.fileName().toStdString().c_str());
    return false;
  }
  file.write(project.toJson());
  file.close();
  qDebug() << "Saved";
  return true;
}

void Settings::on_save_triggered() {
  if (projectFilePath.isEmpty())
    on_saveAs_triggered();
  else {
    settings.setValue("LatestProjectPath", projectFilePath);
  }
}

void Settings::on_importCSV_triggered() {
  csvFilePath = QFileDialog::getOpenFileName(
      this, tr("Import OneRPM stats"), "",
      tr("OneRPM statistics (*.csv);;All Files (*)"));
  if (!csvFilePath.isEmpty())
    if (insertQuery()) {
      emit isOpened();
      selectArtist();
    }
}

void Settings::on_about_triggered() {

  QString translatedTextAboutQtCaption;
  translatedTextAboutQtCaption =
      QMessageBox::tr("<h3>About Label Statistics</h3>"
                      "<p>This program uses Qt version %1.</p>")
          .arg(QLatin1String(QT_VERSION_STR));
  //: Leave this text untranslated or include a verbatim copy of it below
  //: and note that it is the authoritative version in case of doubt.
  const QString translatedTextAboutQtText =
      QMessageBox::tr(
          "<p>Версия: 1.0</p>"
          "<p></p>"
          "<p>Описание: программа разработана для анализа активности "
          "пользователей музыкальными сервисами, а также для удобного подсчета "
          "доходов артистов. Эта программа предназначена для продюсерских "
          "центров, а также для независимых артистов.</p>"
          "<p></p>"
          "<p>По всем вопросам: ifxbooking@gmail.com</p>"
          "<p>Сайт нашего агентства: <a href=\"http://%2/\">%2</a></p>"
          "<p>Copyright (C) %1 The FromX Ltd and other "
          "contributors.</p>"
          "<p>Label Statistics and the Label Statistics logo are trademarks of "
          "The FromX Ltd.</p>")
          .arg(QStringLiteral("2020"), QStringLiteral("imfromx.com"));
  QMessageBox *msgBox = new QMessageBox(this);
  msgBox->setAttribute(Qt::WA_DeleteOnClose);
  msgBox->setWindowTitle("About Label Statistics");
  msgBox->setText(translatedTextAboutQtCaption);
  msgBox->setInformativeText(translatedTextAboutQtText);
  QPixmap pm(QLatin1String(":/main/res/icon.png"));
  if (!pm.isNull())
    msgBox->setIconPixmap(pm);
  msgBox->show();
}
