#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QApplication>

class GlobalSettings : public QObject
{
    Q_OBJECT
public:
    explicit GlobalSettings(QObject *parent = nullptr);

    static QSettings* getInstance(){
        if(settings==nullptr)
        {
            auto path=qApp->applicationDirPath()+"/config.ini";
            settings=new QSettings(path,QSettings::IniFormat,qApp);
            settings->setIniCodec("UTF-8");
        }
        return settings;
    }

signals:
protected:
    inline static QSettings *settings=nullptr;
};

#endif // GLOBALSETTINGS_H
