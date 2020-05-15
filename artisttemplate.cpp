#include "artisttemplate.h"
#include "ui_artisttemplate.h"

ArtistTemplate::ArtistTemplate(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ArtistTemplate) {
  ui->setupUi(this);
  ui->artistBox->setEditText("Выберите артиста");
  ui->statsView->show();
  // cash=Currency(QPair<QString,QString>("USD,RUB"));
}

ArtistTemplate::~ArtistTemplate() { delete ui; }

QComboBox *ArtistTemplate::getArtistsBox() { return ui->artistBox; }

QString ArtistTemplate::getArtistName() { return ui->artistBox->currentText(); }

int ArtistTemplate::getArtistIndex() { return ui->artistBox->currentIndex(); }

QString ArtistTemplate::getTrackName() { return ui->tracksBox->currentText(); }

int ArtistTemplate::getTrackIndex() { return ui->tracksBox->currentIndex(); }

QComboBox *ArtistTemplate::getTracksBox() { return ui->tracksBox; }

void ArtistTemplate::setArtists(QStringList &list) {
  ui->artistBox->clear();
  ui->artistBox->addItems(list);
}

void ArtistTemplate::setTracks(QStringList &list) {
  ui->tracksBox->clear();
  ui->tracksBox->addItems(list);
}

void ArtistTemplate::setStats(QAbstractTableModel &model) {
  ui->statsView->setModel(&model);
}

void ArtistTemplate::setGlobalStreamsPerArtist(int quantity) {
  ui->globalStreamsCounter->setText(QString::number(quantity));
}

void ArtistTemplate::setIncomePerTrack(double income) {
  setValue(this, ui->incomePerTrackValue, income);
}

void ArtistTemplate::setIncomePerArtist(double income) {
  setValue(this, ui->incomePerArtistValue, income);
}

void ArtistTemplate::setFeeForAggregator(double fee) {
  ui->feeForAggregatorValue->setText(QString::number(fee));
}

void ArtistTemplate::setFeeForDistributor(double fee) {
  ui->feeForDistributorValue->setText(QString::number(fee));
}

void ArtistTemplate::setProfitPerArtist(double profit) {
  setValue(this, ui->profitPerArtistValue, profit);
}

void ArtistTemplate::setValue(ArtistTemplate *T, QLabel *label, double value) {
  QString buffer;
  buffer.append(QString::number(value))
      .append(" ")
      .append(T->cash.getPair().first)
      .append(" | ")
      .append(QString::number(value * T->cash.getRate()))
      .append(" ")
      .append(T->cash.getPair().second);
  label->setText(buffer);
}

void ArtistTemplate::activate() { this->show(); }
