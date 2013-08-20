#include "View.h"

#include <QListView>

QListView* makeView(QWidget* parent, QAbstractItemModel* model, int column)
{
    QListView* view = new QListView(parent);
    view->setViewMode(QListView::ListMode);
    view->setModel(model);
    view->setModelColumn(column);
    return view;
}
