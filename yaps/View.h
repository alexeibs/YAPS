#ifndef VIEW_H
#define VIEW_H

class QWidget;
class QListView;
class QAbstractItemModel;

QListView* makeView(QWidget* parent, QAbstractItemModel*, int column);

#endif // VIEW_H
