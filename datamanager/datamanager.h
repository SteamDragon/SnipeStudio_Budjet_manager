#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QtCore/QFile>
#include <QTextCodec>
#include <sqlman/sqlman.h>


class dataManager : public QObject
{
    Q_OBJECT
public:
    dataManager();
    ~dataManager();

    QString getPath();
    QString GetCurrency();
    QString getTranslation();
    QString getMenuTranslation();
    QString getDefaultUser();
    int getVerbosity();

signals:
public slots:
    void setPath(QString data);
    void setCurrency(QString currency);
    void setTranslation(QString translation);
    void setDefaultUser(QString DefaultUser);
    void setVerbosity(int verb);


private:
    QString dataPath;
    QString configPath;
    QString Currency;
    QString Translation;
    QString DefUser;
    int verbosity;
};

#endif // DATAMANAGER_H
