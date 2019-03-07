#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QWidget>

namespace Ui {
class DataConfig;
}

class DataConfig : public QWidget
{
    Q_OBJECT

public:
    explicit DataConfig(QWidget *parent = 0);
    ~DataConfig();

private:
    Ui::DataConfig *ui;
};

#endif // DATACONFIG_H
