#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QToolBar>
#include <QLabel>
#include <QSpinBox>
#include <QThread>
#include <QFileDialog>
#include <QProcess>
#include <QCoreApplication>

#include "../ProfileEngine.h"
#include "MainWindow.h"
#include "TextEdit.h"
#include "VirtualJoystick.h"
#include "../otherFunctions.h"
#include "../COMPILER/CompilationWidget.h"
#include "../SETTINGS/SettingsDialog.h"
#include "../SETTINGS/MyFileDialog.h"
#include "../SETTINGS/ApplicationSettings.h"
#include "../SETTINGS/GeneralSettingsWidget.h"
#include "../SETTINGS/VJoySettingsWidget.h"
#include "../SETTINGS/VigemSettingsWidget.h"


///////////////////////////////////////////////////////////////////////////////
//  CONSTRUCTEUR ET DESTRUCTEUR
//
//  CREATE ACTIONS
//  CREATE MENUS
//  CREATE TOOLBAR
//  SETUP WIDGET
//  SET STATE
//  CLOSE EVENT
//  SLOT CLOSE
//
//  SLOT BROWSE BUTTON CLICKED
//  SLOT SETTINGS
//  SLOT COMPILATION
//  SLOT PLAY
//  SLOT STOP
//  SLOT UNLOAD
//
//  SLOT RUN CONTROLLERS INFO
//  SLOT RUN MONITORING
//  SLOT RUN AXES CURVES
///////////////////////////////////////////////////////////////////////////////


// CONSTRUCTEUR ET DESTRUCTEUR ////////////////////////////////////////////////
MainWindow::MainWindow(QString proFilePath, int dtms, bool bPlay, QWidget *parent) : QMainWindow{parent}
{
	// read settings
	ApplicationSettings& settings = ApplicationSettings::instance();
	settings.readFile();
	//bool bUseVJoyConfigBinary = settings.property("bUseVJoyConfigBinary").toBool();
	//QString vJoyConfigBinary = settings.property("vJoyConfigBinary").toString();
	//VirtualJoystick::setVJoyConfigOptions(bUseVJoyConfigBinary,vJoyConfigBinary);
	bool bUseStartingProfilePath = settings.property("bUseStartingProfilePath").toBool();
	QString startingProfile = settings.property("startingProfilePath").toString();
	bool bUseDefaultTimeStep = settings.property("bUseDefaultTimeStep").toBool();
	int defaultTimeStep = settings.property("defaultTimeStep").toInt();
	bool bWhiteList = settings.property("bWhiteListPid").toBool();
	
	// init
	m_proFilePath = "";
	m_dllFilePath = "";
	m_dllFileName = "";
	m_engine = new ProfileEngine{bWhiteList,this};
	
	m_compilWidget = new CompilationWidget{this};
	
	this->createActions();
	this->createMenus();
	this->createToolBar();
	this->setupWidget();
	this->setState(HmiState::WaitingForDll);
	
	// connections
	QObject::connect(actionSettings,          &QAction::triggered,     this,     &MainWindow::slotSettings);
	QObject::connect(actionQuit,              &QAction::triggered,     qApp,     &QCoreApplication::quit);
	QObject::connect(actionCompilation,       &QAction::triggered,     this,     &MainWindow::slotCompilation);
	QObject::connect(actionPlay,              &QAction::triggered,     this,     &MainWindow::slotPlay);
	QObject::connect(actionStop,              &QAction::triggered,     this,     &MainWindow::slotStop);
	QObject::connect(actionUnload,            &QAction::triggered,     this,     &MainWindow::slotUnload);
	QObject::connect(actionRunControllersInfo,&QAction::triggered,     this,     &MainWindow::slotRunControllersInfo);
	QObject::connect(actionRunMonitoring,     &QAction::triggered,     this,     &MainWindow::slotRunMonitoring);
	QObject::connect(actionRunAxesCurves,     &QAction::triggered,     this,     &MainWindow::slotRunAxesCurves);
	QObject::connect(m_engine,                &ProfileEngine::message, textEdit, &TextEdit::addMessage);
	QObject::connect(boutonBrowse,            &QPushButton::clicked,   this,     &MainWindow::slotBrowseButtonClicked);
	
	// arguments supplémentaires et valeurs par défaut
	if (bUseDefaultTimeStep) {boxRefreshRate->setValue(defaultTimeStep);}
	if (!proFilePath.isEmpty())
	{
		m_proFilePath = proFilePath;
		m_dllFilePath = "";
		m_dllFileName = "";
		
		lineDllPath->setText(m_proFilePath);
		textEdit->addMessage("Plugin path changed for " + m_proFilePath,Qt::black);
		this->setState(HmiState::ReadyToPlayNotLoaded);
		
		boxRefreshRate->setValue(dtms);
		
		if (bPlay) {this->slotPlay();}
	}
	else if (bUseStartingProfilePath && !startingProfile.isEmpty())
	{
		m_proFilePath = startingProfile;
		m_dllFilePath = "";
		m_dllFileName = "";
		
		lineDllPath->setText(m_proFilePath);
		textEdit->addMessage("Plugin path changed for " + m_proFilePath,Qt::black);
		this->setState(HmiState::ReadyToPlayNotLoaded);
	}
}

MainWindow::~MainWindow()
{
	// write settings
	ApplicationSettings& settings = ApplicationSettings::instance();
	if (!settings.isEmpty()) {settings.writeFile();}
}






// CREATE ACTIONS /////////////////////////////////////////////////////////////
void MainWindow::createActions()
{
	actionQuit = new QAction{"Quit",this};
	actionQuit->setStatusTip("Quit");
	actionQuit->setIcon(QIcon{":/RESOURCES/ICONES/croixRouge.png"});
	
	actionSettings = new QAction{"Settings",this};
	actionSettings->setStatusTip("Application settings");
	actionSettings->setIcon(QIcon{":/RESOURCES/ICONES/outils.png"});
	
	actionCompilation = new QAction{"Compilation",this};
	actionCompilation->setStatusTip("Compile profile");
	actionCompilation->setIcon(QIcon{":/RESOURCES/ICONES/compilation.png"});
	
	actionPlay = new QAction{"Run",this};
	actionPlay->setStatusTip("Load and run profile");
	actionPlay->setIcon(QIcon{":/RESOURCES/ICONES/play.png"});
	
	actionStop = new QAction{"Stop",this};
	actionStop->setStatusTip("Stop profile");
	actionStop->setIcon(QIcon{":/RESOURCES/ICONES/stop.png"});
	
	actionUnload = new QAction{"Unload",this};
	actionUnload->setStatusTip("Unload profile");
	actionUnload->setIcon(QIcon{":/RESOURCES/ICONES/eject.png"});
	
	actionRunControllersInfo = new QAction{"Start ControllersInfo.exe",this};
	actionRunControllersInfo->setStatusTip("Start ControllersInfo.exe");
	actionRunControllersInfo->setIcon(QIcon{":/RESOURCES/ICONES/info.png"});
	
	actionRunMonitoring = new QAction{"Start Monitoring.exe",this};
	actionRunMonitoring->setStatusTip("Start Monitoring.exe");
	actionRunMonitoring->setIcon(QIcon{":/RESOURCES/ICONES/eyes.png"});
	
	actionRunAxesCurves = new QAction{"Start AxesCurves.exe",this};
	actionRunAxesCurves->setStatusTip("Start AxesCurves.exe");
	actionRunAxesCurves->setIcon(QIcon{":/RESOURCES/ICONES/curve.png"});
}

// CREATE MENUS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::createMenus()
{
	// file menu
	fileMenu = this->menuBar()->addMenu("File");
	fileMenu->addAction(actionSettings);
	fileMenu->addSeparator();
	fileMenu->addAction(actionQuit);
	
	// profile menu
	profileMenu = this->menuBar()->addMenu("Profile");
	profileMenu->addAction(actionCompilation);
	profileMenu->addAction(actionPlay);
	profileMenu->addAction(actionStop);
	profileMenu->addAction(actionUnload);
	
	// tools menu
	toolsMenu = this->menuBar()->addMenu("Tools");
	toolsMenu->addAction(actionRunControllersInfo);
	toolsMenu->addAction(actionRunMonitoring);
	toolsMenu->addAction(actionRunAxesCurves);
}

// CREATE TOOLBAR //////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::createToolBar()
{
	toolbar = new QToolBar{"Session",this};
	toolbar->setFloatable(false);
	toolbar->setMovable(false);
	
	toolbar->addAction(actionSettings);
	toolbar->addAction(actionCompilation);
	toolbar->addAction(actionPlay);
	toolbar->addAction(actionStop);
	toolbar->addAction(actionUnload);
	
	this->addToolBar(Qt::TopToolBarArea,toolbar);
}

// SETUP WIDGET ///////////////////////////////////////////////////////////////
void MainWindow::setupWidget()
{
	// main widget and layout
	mainWidget = new QWidget{this};
	layout = new QVBoxLayout{mainWidget};
	mainWidget->setLayout(layout);
	this->setCentralWidget(mainWidget);
	layoutDllChoice = new QHBoxLayout{this};
	layoutRefreshRate = new QHBoxLayout{this};
	
	// dll choice
	boutonBrowse = new QPushButton{"Browse",this};
	lineDllPath = new QLineEdit{this};
	lineDllPath->setReadOnly(true);
	
	// refresh rate
	labelRefreshRate = new QLabel{"Refresh rate:",this};
	boxRefreshRate = new QSpinBox{this};
	boxRefreshRate->setMinimum(5);
	boxRefreshRate->setMaximum(500);
	boxRefreshRate->setValue(15);
	boxRefreshRate->setSingleStep(1);
	boxRefreshRate->setSuffix(" ms");
	
	// text edit
	textEdit = new TextEdit{this};
	
	// add widgets to laytouts
	layoutDllChoice->addWidget(boutonBrowse);
	layoutDllChoice->addWidget(lineDllPath);
	layoutRefreshRate->addWidget(labelRefreshRate);
	layoutRefreshRate->addWidget(boxRefreshRate);
	layoutRefreshRate->addStretch();
	
	layout->addLayout(layoutDllChoice);
	layout->addSpacing(5);
	layout->addLayout(layoutRefreshRate);
	layout->addSpacing(5);
	layout->addWidget(textEdit);
	
	// status bar
	statusbar = new QStatusBar{this};
	this->setStatusBar(statusbar);
	
	// end
	this->resize(500,500);
	this->setWindowIcon(QIcon{":/RESOURCES/ICONES/gamepad.png"});
}

// SET STATE //////////////////////////////////////////////////////////////////
void MainWindow::setState(HmiState s)
{
	if (s == HmiState::WaitingForDll)
	{
		actionSettings->setEnabled(true);
		actionCompilation->setEnabled(false);
		actionPlay->setEnabled(false);
		actionStop->setEnabled(false);
		actionUnload->setEnabled(false);
		boutonBrowse->setEnabled(true);
		boxRefreshRate->setEnabled(false);
	}
	else if (s == HmiState::ReadyToPlayNotLoaded)
	{
		actionSettings->setEnabled(true);
		actionCompilation->setEnabled(true);
		actionPlay->setEnabled(true);
		actionStop->setEnabled(false);
		actionUnload->setEnabled(false);
		boutonBrowse->setEnabled(true);
		boxRefreshRate->setEnabled(true);
	}
	else if (s == HmiState::ReadyToPlayLoaded)
	{
		actionSettings->setEnabled(false);
		actionCompilation->setEnabled(false);
		actionPlay->setEnabled(true);
		actionStop->setEnabled(false);
		actionUnload->setEnabled(true);
		boutonBrowse->setEnabled(true);
		boxRefreshRate->setEnabled(true);
	}
	else if (s == HmiState::Playing)
	{
		actionSettings->setEnabled(false);
		actionCompilation->setEnabled(false);
		actionPlay->setEnabled(false);
		actionStop->setEnabled(true);
		actionUnload->setEnabled(false);
		boutonBrowse->setEnabled(false);
		boxRefreshRate->setEnabled(false);
	}
	else if (s == HmiState::Quitting)
	{
		actionSettings->setEnabled(false);
		actionCompilation->setEnabled(false);
		actionPlay->setEnabled(false);
		actionStop->setEnabled(false);
		actionUnload->setEnabled(false);
		boutonBrowse->setEnabled(false);
		boxRefreshRate->setEnabled(false);
	}
}

// CLOSE EVENT ////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent *event)
{
	textEdit->addMessage("Closing",Qt::black);
	m_engine->stop();
	this->setState(HmiState::Quitting);
	
	if (m_engine->isLoaded())
	{
		m_engine->unloadProfile();
		QThread::sleep(1);
	}
	
	event->accept();
}

// SLOT CLOSE /////////////////////////////////////////////////////////////////
void MainWindow::slotClose()
{
	textEdit->addMessage("Closing",Qt::black);
	m_engine->stop();
	this->setState(HmiState::Quitting);
	
	if (m_engine->isLoaded())
	{
		m_engine->unloadProfile();
		QThread::sleep(1);
	}
	
	qApp->quit();
}






// SLOT BROWSE BUTTON CLICKED /////////////////////////////////////////////////
void MainWindow::slotBrowseButtonClicked()
{
	QString proFilePath = MyFileDialog::getOpenFileName(this, "Select profile plugin", "defaultDirectory", "*.pro");
	if (proFilePath == "" || proFilePath == m_proFilePath) {return;}
	
	m_engine->unloadProfile();
	m_proFilePath = proFilePath;
	m_dllFilePath = "";
	m_dllFileName = "";
	
	lineDllPath->setText(m_proFilePath);
	textEdit->addMessage("Plugin path changed for " + m_proFilePath,Qt::black);
	this->setState(HmiState::ReadyToPlayNotLoaded);
}

// SLOT SETTINGS //////////////////////////////////////////////////////////////
void MainWindow::slotSettings()
{
	SettingsDialog settingsDialog{this};
	settingsDialog.addSettingsWidget(new GeneralSettingsWidget{&settingsDialog});
	//settingsDialog.addSettingsWidget(new VJoySettingsWidget{&settingsDialog});
	settingsDialog.addSettingsWidget(new VigemSettingsWidget{&settingsDialog});
	
	int result = settingsDialog.exec();
	if (result == QDialog::Rejected) {return;}
}

// SLOT COMPILATION ///////////////////////////////////////////////////////////
void MainWindow::slotCompilation()
{
	m_compilWidget->setProjectDirectory(dirName(m_proFilePath));
	m_compilWidget->show();
}

// SLOT PLAY //////////////////////////////////////////////////////////////////
void MainWindow::slotPlay()
{
	if (m_engine->isActive()) {return;}
	
	// load the plugin only if necessary
	bool bConfigOk = true;
	if (m_dllFilePath == "" || !m_engine->isLoaded())
	{
		// search for one dll
		QString dllDir = dirName(m_proFilePath) + "/" + debugOrRelease();
		QStringList dllFiles = QDir{dllDir}.entryList({"*.dll"},QDir::Files);
		if (dllFiles.size() == 0)
		{
			textEdit->addMessage("No profile dll in directory " + dllDir,Qt::red);
			return;
		}
		else if (dllFiles.size() > 1)
		{
			textEdit->addMessage("Several profile dlls in directory " + dllDir,Qt::red);
			return;
		}
		m_dllFileName = dllFiles[0];
		m_dllFilePath = dllDir + "/" + m_dllFileName;
		
		// load the profile plugin
		bConfigOk = m_engine->loadProfile(m_dllFilePath);
	}
	
	// manages loading errors
	if (!bConfigOk)
	{
		this->setState(HmiState::ReadyToPlayNotLoaded);
		return;
	}
	
	// configuration and start
	this->setState(HmiState::Playing);
	if (!m_engine->start(boxRefreshRate->value())) {this->setState(HmiState::ReadyToPlayLoaded);}
}

// SLOT STOP //////////////////////////////////////////////////////////////////
void MainWindow::slotStop()
{
	this->setState(HmiState::ReadyToPlayLoaded);
	m_engine->stop();
}

// SLOT UNLOAD ////////////////////////////////////////////////////////////////
void MainWindow::slotUnload()
{
	m_engine->unloadProfile();
	this->setState(HmiState::ReadyToPlayNotLoaded);
}






// SLOT RUN CONTROLLERS INFO //////////////////////////////////////////////////
void MainWindow::slotRunControllersInfo()
{
	QString controllersInfoExe = QCoreApplication::applicationDirPath() + "/../../ControllersInfo/release/ControllersInfo.exe";
	if (!QProcess::startDetached(controllersInfoExe))
		textEdit->addMessage("Failed to start ControllersInfo.exe",Qt::red);
}

// SLOT RUN MONITORING ////////////////////////////////////////////////////////
void MainWindow::slotRunMonitoring()
{
	QString monitoringExe = QCoreApplication::applicationDirPath() + "/../../Monitoring/release/Monitoring.exe";
	if (!QProcess::startDetached(monitoringExe))
		textEdit->addMessage("Failed to start Monitoring.exe",Qt::red);
}

// SLOT RUN AXES CURVES ///////////////////////////////////////////////////////
void MainWindow::slotRunAxesCurves()
{
	QString axesCurvesExe = QCoreApplication::applicationDirPath() + "/../../AxesCurves/release/AxesCurves.exe";
	if (!QProcess::startDetached(axesCurvesExe))
		textEdit->addMessage("Failed to start AxesCurves.exe",Qt::red);
}

