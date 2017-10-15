#ifndef FANCYTABWIDGET_H
#define FANCYTABWIDGET_H

#include <QWidget>
#include <QPushButton>

class QButtonGroup;
class QBoxLayout;
class QStackedWidget;
class QPushButton;

class TabButton : public QPushButton
{
    Q_OBJECT

public:
    TabButton(const QString& text, QWidget *parent = 0);
    void addButton(TabButton *button);
    QButtonGroup *getButtons() { return m_buttonGroup; }
    void setTab(QWidget *tab) { m_tab = tab; }
    QWidget *getTab() { return m_tab; }
    void hideChildren();

    void setParentButton(TabButton *parent) { m_parent = parent; }
    TabButton *getParentButton() { return m_parent; }

private:
    QButtonGroup *m_buttonGroup = nullptr;
    QWidget *m_tab = nullptr;
    TabButton *m_parent = nullptr;
};

class FancyTabWidget : public QWidget
{
    Q_OBJECT

public:
    FancyTabWidget(QWidget *parent = 0);

    TabButton *insertTab(QWidget *tab, const QString &label, const QString &objectName = "", TabButton *parent = nullptr);

    void changePage(TabButton *button);
    void changePage(const QString& objectName);

private slots:
    void onToggled(bool checked);

private:
    QButtonGroup *m_buttons;
    QWidget *m_buttonsWidget;
    QBoxLayout *m_buttonsLayout;
    QStackedWidget *m_pagesWidget;
};

#endif // FANCYTABWIDGET_H
