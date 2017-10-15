#ifndef TRACKSVIEW_H
#define TRACKSVIEW_H

#include <QScrollArea>
#include "user.h"

class QTableWidget;

class TracksView : public QScrollArea
{
    Q_OBJECT
public:
    explicit TracksView(User user, QWidget *parent = 0);

private:
    User m_user;
    QTableWidget *m_tracks;
};

#endif // TRACKSVIEW_H
