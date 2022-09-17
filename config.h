#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QDir>

class Config
{
public:
    Config()=default;

    //if linux, maybe use QDir::toNativeSeparators(dirName)
    static bool create_dir_if_not_exist(const QString &dirName){
        QDir dir(dirName);
        if(!dir.exists()){
            bool succ=dir.mkdir(dirName);
            if(!succ)
                return false;
        }
        return true;
    }

    static void setDataRoot(QString _dataRoot);

    static const QString & getDataRoot(){
        return dataRoot;
    }

    static const QString & getTreePath(){
        return treePath;
    }

    static const QString & getPaperMetaPath(){
        return paperMetaPath;
    }

    static const QString & getNotePath(){
        return notePath;
    }

private:
    inline static QString dataRoot,treePath,paperMetaPath,notePath;
};

#endif // CONFIG_H
