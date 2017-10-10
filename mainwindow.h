#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScrollArea>
#include <QOAuth2AuthorizationCodeFlow>
#include "playlist.h"

class QGridLayout;

namespace Ui {
class MainWindow;
}

class MainWindow : public QScrollArea
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void granted();

private:
    Ui::MainWindow *ui;

    void getUserInfo();
    void getPlayLists();

    QGridLayout *m_gridLayout;
    QOAuth2AuthorizationCodeFlow spotify;
    bool isGranted;
    QString userName;
    QVector<Playlist> m_playlists;
};

#endif // MAINWINDOW_H
