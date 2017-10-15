#include "fancytabwidget.h"
#include <QButtonGroup>
#include <QBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QVariant>

TabButton::TabButton(const QString& text, QWidget *parent) :
    QPushButton(text, parent)
{
    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);
    setCheckable(true);
    setFocusPolicy(Qt::NoFocus);
}

void TabButton::addButton(TabButton *button)
{
    m_buttonGroup->addButton(button);
}

void TabButton::hideChildren()
{
    for(QAbstractButton *button : m_buttonGroup->buttons())
        button->setVisible(false);
}


FancyTabWidget::FancyTabWidget(QWidget *parent) :
    QWidget(parent)
{
    m_buttonsWidget = new QWidget;
    m_buttonsWidget->setObjectName("fancyTabButtonsWidget");

    m_buttonsLayout = new QVBoxLayout;
    m_buttonsLayout->setSpacing(0);
    m_buttonsLayout->setMargin(0);

    QBoxLayout *buttonsLayout;
    buttonsLayout = new QVBoxLayout;
    buttonsLayout->setSpacing(0);
    buttonsLayout->setMargin(0);
    buttonsLayout->addLayout(m_buttonsLayout);
    buttonsLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_buttonsWidget->setLayout(buttonsLayout);

    m_pagesWidget = new QStackedWidget;

    QBoxLayout *mainLayout;
    mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(m_buttonsWidget);
    mainLayout->addWidget(m_pagesWidget, 1);
    setLayout(mainLayout);

    m_buttons = new QButtonGroup(m_buttonsWidget);
    m_buttons->setExclusive(true);
}

TabButton *FancyTabWidget::insertTab(QWidget *tab, const QString &label, const QString &objectName, TabButton *parent)
{
    TabButton *button = new TabButton(label, m_buttonsWidget);
    button->setObjectName(objectName);
    button->setParentButton(parent);
    connect(button, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));

    m_buttonsLayout->addWidget(button);

    if(!parent) {
        m_buttons->addButton(button);
        TabButton *subButton = insertTab(tab, tr("General"), objectName + "_general", button);
        subButton->setVisible(false);
        button->setProperty("root", true);
        if(m_buttons->buttons().size() == 1)
            button->setChecked(true);
    }
    else {
        m_pagesWidget->addWidget(tab);

        button->setText("- " + label);
        button->setTab(tab);
        parent->addButton(button);
        int nButtons = parent->getButtons()->buttons().size();
        if(nButtons == 1) {
            button->blockSignals(true);
            button->setChecked(true);
            button->blockSignals(false);
        }
        else if(nButtons == 2 && parent->isChecked())
            parent->getButtons()->buttons().first()->setVisible(true);
        button->setVisible(parent->isChecked());
        button->setProperty("root", false);
    }

    return button;
}

void FancyTabWidget::changePage(TabButton *button)
{
    TabButton *parent = button->getParentButton();
    while(parent) {
        parent->setChecked(true);
        parent = parent->getParentButton();
    }

    button->setChecked(true);
    if(button->getTab())
        m_pagesWidget->setCurrentWidget(button->getTab());
    else {
        TabButton *child = qobject_cast<TabButton*>(button->getButtons()->buttons().first());
        child->setChecked(true);
        m_pagesWidget->setCurrentWidget(child->getTab());
    }
}

void FancyTabWidget::changePage(const QString& objectName)
{
    for(QObject *object : m_buttonsWidget->children()) {
        if(object->objectName() == objectName) {
            changePage((TabButton*)object);
            break;
        }
    }
}

void FancyTabWidget::onToggled(bool checked)
{
    TabButton *button = qobject_cast<TabButton*>(sender());
    if(button) {
        int size = button->getButtons()->buttons().size();
        if(size > 1) {
            for(QAbstractButton *b : button->getButtons()->buttons())
                b->setVisible(true);
            TabButton *child = qobject_cast<TabButton*>(button->getButtons()->checkedButton());
            if(!child)
                child = qobject_cast<TabButton*>(button->getButtons()->buttons().first());
            m_pagesWidget->setCurrentWidget(child->getTab());
        }
        else if(size == 1) {
            TabButton *child = qobject_cast<TabButton*>(button->getButtons()->buttons().first());
            m_pagesWidget->setCurrentWidget(child->getTab());
        }
        else {
            m_pagesWidget->setCurrentWidget(button->getTab());
        }

        if(!checked)
            button->hideChildren();
    }
}
