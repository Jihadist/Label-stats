#pragma once

#include <QAbstractTableModel>

class QMusicModel : public QAbstractTableModel {
  Q_OBJECT
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;

  bool insertRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;
  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;
  bool insertColumns(int column, int count,
                     const QModelIndex &parent = QModelIndex()) override;
  bool removeColumns(int column, int count,
                     const QModelIndex &parent = QModelIndex()) override;

public:
  QMusicModel();
};
