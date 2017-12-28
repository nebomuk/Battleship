#ifndef MAINMENU_H
#define MAINMENU_H
//
#include <QStackedWidget>
#include "ui_mainmenu.h"
//
class MainMenu : public QStackedWidget, public Ui::Form
{
Q_OBJECT
public:
	MainMenu( QWidget * parent = 0);
private slots:
	void setMainMenu();
	void setOptionsMenu();
	void closeEvent(QCloseEvent *event);
};
#endif




