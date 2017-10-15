#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScrollArea>
#include <QOAuth2AuthorizationCodeFlow>
#include <QtNetworkAuth>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGridLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include "user.h"
#include "playlist.h"
#include "spotifyview.h"

class QGridLayout;
class QPlainTextEdit;

class MainWindow : public QScrollArea
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void granted();

private:
    void getUserInfo();
    void getPlayLists();
    void getTracks();
    void showInterface();
    void playTrack(const QString& trackName);

    QGridLayout *m_gridLayout;
    QPlainTextEdit *m_plainText;
    SpotifyView *m_spotifyView;

    QOAuth2AuthorizationCodeFlow spotify;
    User m_user;
};

#endif // MAINWINDOW_H
