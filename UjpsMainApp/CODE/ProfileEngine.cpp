#include "ProfileEngine.h"
#include "otherFunctions.h"
#include <QCoreApplication>
#include <QTimer>
#include <QPluginLoader>
#include "AbstractProfile.h"


///////////////////////////////////////////////////////////////////////////////
//  CONSTRUCTEUR ET DESTRUCTEUR
//
//  LOAD PROFILE
//  UNLOAD PROFILE
//  IS LOADED
//
//  START
//  STOP
//  IS ACTIVE
//
//  SLOT ONE LOOP
///////////////////////////////////////////////////////////////////////////////


// CONSTRUCTEUR ET DESTRUCTEUR ////////////////////////////////////////////////
ProfileEngine::ProfileEngine(bool bWhiteList, QObject *parent) : QObject{parent}
{
	m_timer = new QTimer{this};
	QObject::connect(m_timer, &QTimer::timeout, this, &ProfileEngine::slotOneLoop);
	
	m_dllFileName = "";
	m_profile = nullptr;
	m_loader = nullptr;
	
	if (bWhiteList && m_vigemInterface.vigemIsReady())
		m_vigemInterface.whiteList(QCoreApplication::applicationPid());
}

ProfileEngine::~ProfileEngine()
{
	m_timer->stop();
	this->unloadProfile();
	
	// remove this application from the white list
	// (to avoid to pollute the white list with many invalid pids)
	if (m_vigemInterface.vigemIsReady())
		m_vigemInterface.blackList(QCoreApplication::applicationPid());
}






// LOAD PROFILE ///////////////////////////////////////////////////////////////
bool ProfileEngine::loadProfile(const QString &dllFilePath)
{
	if (m_loader) {return false;}
	
	emit message("Loading profile " + m_dllFileName,Qt::black);
	m_loader = new QPluginLoader{dllFilePath};
	if (QObject *plugin = m_loader->instance())
	{
		if (AbstractProfile* profile = qobject_cast<AbstractProfile*>(plugin))
		{
			m_dllFileName = shortName(dllFilePath);
			m_profile = profile;
			QObject::connect(m_profile, SIGNAL(message(QString,QColor)), this, SIGNAL(message(QString,QColor)));
			return true;
		}
	}
	
	emit message("Plugin loading failed",Qt::red);
	delete m_loader;
	m_loader = nullptr;
	return false;
}

// UNLOAD PROFILE /////////////////////////////////////////////////////////////
bool ProfileEngine::unloadProfile()
{
	if (!m_loader) {return true;}
	
	this->stop();
	bool b = m_loader->unload(); // it deletes m_profile, but on Qt5.11 it does not unleash the dll
	delete m_loader;
	
	emit message("Unload profile " + m_dllFileName,Qt::black);
	m_dllFileName = "";
	m_loader = nullptr;
	m_profile = nullptr;
	return b;
}

// IS LOADED //////////////////////////////////////////////////////////////////
bool ProfileEngine::isLoaded() const
{
	return (m_loader != nullptr);
}






// START //////////////////////////////////////////////////////////////////////
bool ProfileEngine::start(int dtms)
{
	if (!m_profile || m_timer->isActive()) {return false;}
	
	// initialize profile
	emit message("Starting " + m_dllFileName,Qt::black);
	m_profile->setTimeStep(dtms);
	m_timer->setInterval(dtms);
	try
	{
		if (!m_profile->play())
		{
			emit message("Failed to initialize profile",Qt::red);
			return false;
		}
	}
	catch (std::exception &e)
	{
		QString msg = "Failed to initialize profile: " + QString{e.what()};
		emit message(msg,Qt::red);
		return false;
	}
	
	// start the loop!
	m_timer->start();
	emit message("Running",Qt::black);
	return true;
}

// STOP ////////////////////////////////////////////////////////////////////////
void ProfileEngine::stop()
{
	if (!m_profile || !m_timer->isActive()) {return;}
	
	m_timer->stop();
	m_profile->stop();
	emit message("Stop profile " + m_dllFileName,Qt::black);
}

// IS ACTIVE //////////////////////////////////////////////////////////////////
bool ProfileEngine::isActive() const
{
	return m_timer->isActive();
}






// SLOT ONE LOOP //////////////////////////////////////////////////////////////
void ProfileEngine::slotOneLoop()
{
	if (!m_profile) {return;}
	
	try {m_profile->run();}
	catch (std::exception &e) {emit message(e.what(),Qt::red);}
}

