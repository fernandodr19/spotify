#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScrollArea>
#include <QOAuth2AuthorizationCodeFlow>
#include "user.h"
#include "playlist.h"

class QGridLayout;
class QPlainTextEdit;

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
    void getTracks();

    QGridLayout *m_gridLayout;
    QPlainTextEdit *m_plainText;

    QOAuth2AuthorizationCodeFlow spotify;
    User m_user;
};

#endif // MAINWINDOW_H
