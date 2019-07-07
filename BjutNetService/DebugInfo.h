#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <QObject>
#include <QString>
#include <QFile>

namespace bna {

class DebugTool : public QObject{

    Q_OBJECT

public:
    enum DebugStatus{
        STATUS_NONE,
        STATUS_INFO,
        STATUS_SUCCESS,
        STATUS_FAIL,
        STATUS_DATA
    };

    void init(const QString &name);

    void release();

    QString fileName() const;

    //void writeInfo(const QString &content, bool with_time = true, bool end_line = true);

public slots:

    void writeInfo(DebugStatus status, const QString &content, bool with_time = true, bool end_line = true);

private:
    QFile m_file;
};

inline QString DebugTool::fileName() const {
    return m_file.fileName();
}

}


#endif // DEBUGINFO_H
