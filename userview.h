#ifndef USERVIEW_H
#define USERVIEW_H

#include <QScrollArea>
#include "user.h"

class QTableWidget;
class QLabel;

class UserView : public QScrollArea
{
    Q_OBJECT
public:
    explicit UserView(User user, QWidget *parent = 0);

private:
    void loadPlaylistTracks(const QString& playlistId);

    User m_user;
    QTableWidget *m_playlists;
    QLabel *m_playlistName;
    QTableWidget *m_playlistTracks;
};

#endif // USERVIEW_H
