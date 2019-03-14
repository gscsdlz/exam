#include "localcache.h"

LocalCache::LocalCache()
{
    ORM::dbSrc = "/client.db";
    db = ORM::getInstance();
}

QString LocalCache::getExam()
{
    auto res = db->getRow("SELECT * FROM exam_cache ORDER BY id DESC LIMIT 1");
    return res.at(1).toString();
}

bool LocalCache::setExam(QString str)
{
    int id = db->execute("INSERT INTO exam_cache (str) VALUES (?)", QVector<QVariant>(1, str));
    return id > 0;
}

void LocalCache::clearCache()
{
    db->execute("DELETE FROM exam_cache");
    db->execute("DELETE FROM answer_cahce");
}

bool LocalCache::setAnswer(int pro_id, QVector<int> ans)
{
    QVariant num = db->getOne("SELECT COUNT(*) as num FROM answer_cache WHERE pro_id = ?", QVector<QVariant>(1, pro_id));
    QVector<QVariant> args;
    QString ansStr = "";
    for(int i = 0; i < ans.length(); i++) {
        ansStr += ans[i] + ",";
    }
    if (ansStr.length() != 0) {
        ansStr.remove(ansStr.length() - 1, 1);
    }
    args.push_back(ansStr);
    args.push_back(pro_id);

    int row;
    if (num.toInt() == 0) {
        row = db->execute("INSERT INTO answer_cache (ans, pro_id) VALUES(?,?)", args);
    } else {
        row = db->execute("UDPATE answer_cache SET ans = ? WHERE pro_id = ?", args);
    }
    return row > 0;
}

QVector<AnswerInfo> LocalCache::getAnswers()
{
    auto res = db->getAll("SELECT * FROM answer_cache");
    QVector<AnswerInfo> result;
    for(auto row : res) {
        AnswerInfo info;
        info._set("pro_id", row.at(0));
        info._set("ans", row.at(1));
        result.append(info);
    }
    return result;
}
