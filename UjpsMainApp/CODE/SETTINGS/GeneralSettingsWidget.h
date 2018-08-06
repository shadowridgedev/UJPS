#ifndef GENERAL_SETTINGS_WIDGET
#define GENERAL_SETTINGS_WIDGET


#include "AbstractSettingsWidget.h"
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QRadioButton;
class QButtonGroup;
class QLineEdit;
class QCheckBox;
class QPushButton;


class GeneralSettingsWidget : public AbstractSettingsWidget
{
	Q_OBJECT
	
	public:
		explicit GeneralSettingsWidget(QWidget *parent = nullptr);
		GeneralSettingsWidget(const GeneralSettingsWidget &s) = delete;
		GeneralSettingsWidget(GeneralSettingsWidget &&s) = delete;
		GeneralSettingsWidget& operator=(const GeneralSettingsWidget &s) = delete;
		GeneralSettingsWidget& operator=(GeneralSettingsWidget &&s) = delete;
		virtual ~GeneralSettingsWidget();
		
		virtual QString tabName() const override;
		virtual void buttonOkClicked() override;
		virtual void buttonCancelClicked() override;
		
		
	private slots:
		void slotStartingProfileStateChanged(int checkState);
		void slotStartingProfileBrowse();
		
		
	private:
		void addDefaultDirWidgets();
		void addStartingProfileWidgets();
		
		QVBoxLayout *layout1;
		
		// default directory
		QHBoxLayout *layoutDefDir;
		QLabel *labelDefDir;
		QRadioButton *radioDefDirPrevious, *radioDefDirFixed;
		QButtonGroup *radioDefDirGroup;
		QLineEdit *lineDefDir;
		
		// starting profile
		QHBoxLayout *layoutStartingProfile;
		QCheckBox *boxUseStartingProfile;
		QLineEdit *lineStartingProfile;
		QPushButton *buttonStartingProfile;
};


#endif





