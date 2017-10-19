#ifndef TRACKSVIEW_H
#define TRACKSVIEW_H

#include <QScrollArea>
#include "user.h"

class QTableWidget;
class QPlainTextEdit;
class QLineEdit;

class TracksView : public QScrollArea
{
    Q_OBJECT
public:
    explicit TracksView(User user, QWidget *parent = 0);

private:
    void searchTracks(const QString& trackName);

    User m_user;
    QTableWidget *m_localTracks;
    QPlainTextEdit *m_plainText;
    QLineEdit *m_searchForTracks;
    QTableWidget *m_searchedTracks;

};

#endif // TRACKSVIEW_H
