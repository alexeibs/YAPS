#include "View.h"
#include <QListView>

QListView* makeView(QAbstractItemModel* model, int column)
{
    auto view = new QListView;
    view->setViewMode(QListView::ListMode);
    view->setModel(model);
    view->setModelColumn(column);
    return view;
}
