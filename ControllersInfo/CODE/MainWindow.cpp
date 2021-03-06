#include "MainWindow.h"
#include "GameController.h"
#include <QCoreApplication>


///////////////////////////////////////////////////////////////////////////////
//  CONSTRUCTEUR
//
//  CREATE ACTIONS
//  CREATE MENUS
//  SETUP WIDGET
//  CREATE HEADERS
//  CLEAR LAYOUT
//
//  SLOT UPDATE
///////////////////////////////////////////////////////////////////////////////


// CONSTRUCTEUR ///////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}
{
	this->createActions();
	this->createMenus();
	this->setupWidget();
	this->slotUpdate();
	
	// connections
	QObject::connect(actionUpdate, &QAction::triggered, this, &MainWindow::slotUpdate);
	QObject::connect(actionQuit,   &QAction::triggered, qApp, &QCoreApplication::quit);
}






// CREATE ACTIONS /////////////////////////////////////////////////////////////
void MainWindow::createActions()
{
	actionUpdate = new QAction{"Update",this};
	actionUpdate->setStatusTip("Update controllers info");
	actionUpdate->setShortcut(QKeySequence{"F5"});
	actionUpdate->setShortcutContext(Qt::WindowShortcut);
	actionUpdate->setIcon(QIcon{":/RESOURCES/ICONES/update.png"});
	
	actionQuit = new QAction{"Quit",this};
	actionQuit->setStatusTip("Quit");
	actionQuit->setShortcut(QKeySequence{"Ctrl+Q"});
	actionQuit->setShortcutContext(Qt::WindowShortcut);
	actionQuit->setIcon(QIcon{":/RESOURCES/ICONES/croixRouge.png"});
}

// CREATE MENUS ///////////////////////////////////////////////////////////////
void MainWindow::createMenus()
{
	fileMenu = this->menuBar()->addMenu("File");
	fileMenu->addAction(actionUpdate);
	fileMenu->addSeparator();
	fileMenu->addAction(actionQuit);
}

// SETUP WIDGET ///////////////////////////////////////////////////////////////
void MainWindow::setupWidget()
{
	w = new QWidget{this};
	layout = new QGridLayout{w};
	w->setLayout(layout);
	layout->setContentsMargins(10,20,10,20);
	layout->setVerticalSpacing(15);
	
	this->setCentralWidget(w);
	this->resize(600,250);
	this->setWindowIcon(QIcon{":/RESOURCES/ICONES/info.png"});
	this->setWindowTitle("Controllers info");
}

// CREATE HEADERS /////////////////////////////////////////////////////////////
void MainWindow::createHeaders()
{
	QLabel *labelDescription = new QLabel{"Description",this};
	QLabel *labelId          = new QLabel{"ID",this};
	QLabel *labelButtons     = new QLabel{"Buttons",this};
	QLabel *labelAxes        = new QLabel{"Axes",this};
	QLabel *labelPovs        = new QLabel{"Povs",this};
	QLabel *labelHardwareId  = new QLabel{"Hardware ID",this};
	
	labelDescription->setAlignment(Qt::AlignCenter);
	labelId->setAlignment(Qt::AlignCenter);
	labelButtons->setAlignment(Qt::AlignCenter);
	labelAxes->setAlignment(Qt::AlignCenter);
	labelPovs->setAlignment(Qt::AlignCenter);
	labelHardwareId->setAlignment(Qt::AlignCenter);
	
	layout->addWidget(labelDescription,0,0,1,1);
	layout->addWidget(labelId,0,1,1,1);
	layout->addWidget(labelButtons,0,2,1,1);
	layout->addWidget(labelAxes,0,3,1,1);
	layout->addWidget(labelPovs,0,4,1,1);
	layout->addWidget(labelHardwareId,0,5,1,1);
}

// CLEAR LAYOUT ///////////////////////////////////////////////////////////////
void MainWindow::clearLayout()
{
	QLayoutItem *child = nullptr;
	while ((child = layout->takeAt(0))) {delete child->widget();}
}






// SLOT UPDATE ////////////////////////////////////////////////////////////////
void MainWindow::slotUpdate()
{
	// search for DirectInput and XInput controllers
	QVector<GameController*> joysticks = GameController::enumerateControllers(this);
	
	// update the table
	this->clearLayout();
	this->createHeaders();
	int iLine = 1;
	
	for (GameController *gc : joysticks)
	{
		uint id = gc->id();
		QString idString;
		if (id < 100) {idString = QString::number(id) + " (DirectInput)";}
		else {idString = QString::number(id-100) + " (XInput)";}
		
		QLabel *item0 = new QLabel{gc->description(),this};
		QLabel *item1 = new QLabel{idString,this};
		QLabel *item2 = new QLabel{QString::number(gc->buttonsCount()),this};
		QLabel *item3 = new QLabel{QString::number(gc->axesCount()),this};
		QLabel *item4 = new QLabel{QString::number(gc->povsCount()),this};
		QLabel *item5 = new QLabel{gc->hardwareId(),this};
		
		item0->setAlignment(Qt::AlignCenter);
		item1->setAlignment(Qt::AlignCenter);
		item2->setAlignment(Qt::AlignCenter);
		item3->setAlignment(Qt::AlignCenter);
		item4->setAlignment(Qt::AlignCenter);
		item5->setAlignment(Qt::AlignCenter);
		
		layout->addWidget(item0,iLine,0,1,1);
		layout->addWidget(item1,iLine,1,1,1);
		layout->addWidget(item2,iLine,2,1,1);
		layout->addWidget(item3,iLine,3,1,1);
		layout->addWidget(item4,iLine,4,1,1);
		layout->addWidget(item5,iLine,5,1,1);
		
		++iLine;
	}
	
	layout->addItem(new QSpacerItem{0,0,QSizePolicy::Minimum,QSizePolicy::Expanding},iLine,0,1,1);
}

