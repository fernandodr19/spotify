#ifndef USERVIEW_H
#define USERVIEW_H

#include <QScrollArea>

class UserView : public QScrollArea
{
    Q_OBJECT
public:
    explicit UserView(QWidget *parent = 0);
};

#endif // USERVIEW_H
