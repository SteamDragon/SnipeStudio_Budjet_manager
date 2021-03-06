#include "ssbmdatabasemodel.h"

ssbmDatabaseModel::ssbmDatabaseModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db) {}

int ssbmDatabaseModel::rowCount(const QModelIndex &parent) const {
  return QSqlTableModel::rowCount(parent);
}

int ssbmDatabaseModel::columnCount(const QModelIndex &parent) const {
  return QSqlTableModel::columnCount(parent);
}

QVariant ssbmDatabaseModel::data(const QModelIndex &index, int role) const {
  switch (role) {
  case Qt::BackgroundRole: {
    int row = index.row();
    if (this->index(row, 2).data().toDouble() < 0) {
      return QBrush(QColor(Qt::red));
    } else if (this->index(row, 2).data().toDouble() > 0) {
      return QBrush(QColor(Qt::green));
    } else {
      return QBrush(QColor(Qt::white));
    }
  }
  default:
    return QSqlTableModel::data(index, role);
  }
}

QColor ssbmDatabaseModel::calculateColorForRow(int row) {}
