#include "mainmenu.h"

MainMenu::MainMenu( QWidget * parent)
	: QStackedWidget(parent)
{
	setupUi(this);
	this->setWindowFlags(Qt::Dialog);
	connect(optionsButton,SIGNAL(clicked()),this,SLOT(setOptionsMenu()));
	connect(mainMenuButton,SIGNAL(clicked()),this,SLOT(setMainMenu()));
}

void MainMenu::setMainMenu()
{
	this->setCurrentIndex(0);
}


void MainMenu::setOptionsMenu()
{
	this->setCurrentIndex(1);
}

void MainMenu::closeEvent(QCloseEvent *event)
{
	this->setCurrentIndex(0);
	QStackedWidget::closeEvent(event);
}

