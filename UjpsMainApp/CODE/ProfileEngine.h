#ifndef PROFILE_ENGINE
#define PROFILE_ENGINE


#include <QObject>
#include <QColor>
#include "VigemPidWhiteLister.h"
class QTimer;
class QPluginLoader;
class AbstractProfile;


class ProfileEngine : public QObject
{
	Q_OBJECT
	
	public:
		ProfileEngine(bool bWhiteList, QObject *parent = nullptr);
		ProfileEngine(const ProfileEngine &other) = delete;
		ProfileEngine(ProfileEngine &&other) = delete;
		ProfileEngine& operator=(const ProfileEngine &other) = delete;
		ProfileEngine& operator=(ProfileEngine &&other) = delete;
		virtual ~ProfileEngine();
		
		bool loadProfile(const QString &dllFilePath);
		bool unloadProfile();
		bool isLoaded() const;
		
		bool start(int dtms);
		void stop();
		bool isActive() const;
		
		
	private slots:
		void slotOneLoop();
		
		
	signals:
		void message(const QString &message, QColor color);
		
		
	private:
		QTimer *m_timer;
		QString m_dllFileName;
		AbstractProfile *m_profile;
		QPluginLoader *m_loader;
		
		VigemPidWhiteLister m_vigemInterface;
};


#endif

