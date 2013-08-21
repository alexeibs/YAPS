#ifndef VIEW_H
#define VIEW_H

class QWidget;
class QListView;
class QAbstractItemModel;

QListView* makeView(QAbstractItemModel*, int column);

#endif // VIEW_H
