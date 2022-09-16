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

    static void setDataRoot(QString _dataRoot){
        dataRoot=std::move(_dataRoot);
        treePath=dataRoot+"trees/";
        paperMetaPath=dataRoot+"meta_db/";
        create_dir_if_not_exist(dataRoot);
        create_dir_if_not_exist(treePath);
        create_dir_if_not_exist(paperMetaPath);
    }

    static const QString & getDataRoot(){
        return dataRoot;
    }

    static const QString & getTreePath(){
        return treePath;
    }

    static const QString & getPaperMetaPath(){
        return paperMetaPath;
    }

private:
    inline static QString dataRoot,treePath,paperMetaPath;
};

#endif // CONFIG_H
