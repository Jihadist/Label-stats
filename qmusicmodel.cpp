#include "qmusicmodel.h"

int QMusicModel::rowCount(const QModelIndex &parent) const { Q_UNUSED(parent) }

int QMusicModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
}

QVariant QMusicModel::data(const QModelIndex &index, int role) const {}

QVariant QMusicModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const {}

bool QMusicModel::insertRows(int row, int count, const QModelIndex &parent) {}

bool QMusicModel::removeRows(int row, int count, const QModelIndex &parent) {}

bool QMusicModel::insertColumns(int column, int count,
                                const QModelIndex &parent) {}

bool QMusicModel::removeColumns(int column, int count,
                                const QModelIndex &parent) {}

QMusicModel::QMusicModel() {}
