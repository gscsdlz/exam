#include "localcache.h"

LocalCache::LocalCache(QObject *parent)
    :QObject(parent)
{
    ORM::dbSrc = "/client.db";
    db = ORM::getInstance();
    stop = false;
}

QString LocalCache::getExam()
{
    auto res = db->getRow("SELECT * FROM exam_cache ORDER BY id DESC LIMIT 1");
    if (res.empty()) {
        return "";
    }
    return res.at(1).toString();
}

bool LocalCache::setExam(QString str)
{
    int id = db->execute("INSERT INTO exam_cache (exam_str) VALUES (?)", QVector<QVariant>(1, str));
    return id > 0;
}

void LocalCache::clearCache()
{
    db->execute("DELETE FROM exam_cache");
    db->execute("DELETE FROM answer_cache");
}

void LocalCache::setAnswer(int pro_id, int ans)
{
    QVariant num = db->getOne("SELECT id FROM answer_cache WHERE pro_id = ?", QVector<QVariant>(1, pro_id));
    QVector<QVariant> args;
    args.push_back(ans);
    args.push_back(pro_id);
    if (num == 0) {
        db->execute("INSERT INTO answer_cache (ans, pro_id) VALUES (?,?)", args);
    } else {
        db->execute("UPDATE answer_cache SET ans = ? WHERE pro_id = ?", args);
    }
}

QVector<AnswerInfo> LocalCache::getAnswers()
{
    auto res = db->getAll("SELECT * FROM answer_cache");
    QVector<AnswerInfo> result;
    for(auto row : res) {
        AnswerInfo info;
        info._set("pro_id", row.at(1));
        info._set("ans", row.at(2));
        result.append(info);
    }
    return result;
}
