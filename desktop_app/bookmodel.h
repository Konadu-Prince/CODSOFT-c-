#ifndef BOOKMODEL_H
#define BOOKMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "database.h"

class BookModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit BookModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Custom methods
    void refreshData();
    void searchBooks(const QString &query);
    Book getBookAt(int row) const;

private:
    QVector<Book> m_books;
    QString m_searchQuery;
};

#endif // BOOKMODEL_H
