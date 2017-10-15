#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QScrollArea>

class PlaylistView : public QScrollArea
{
    Q_OBJECT
public:
    explicit PlaylistView(QWidget *parent = 0);
};

#endif // PLAYLISTVIEW_H
