#include "daelalldata.h"

DaelAllData::DaelAllData(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");      //参数说明库类型为SQLite
    db.setDatabaseName("AlarmDataBase.db");            //设置库名
    if (!db.open())									//打开库，如果不存在将会自动创建
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
    }
    else
    {
        qDebug() << "Succeed to connect database." ;
        sql_q = new QSqlQuery(db);                  //实例化数据库操作类，参数很关键，必须是之前打开过的数据库，此处为db
    }

    //建表or打开

    QString cmd;                                    //SQL指令，下面进行组指令
    cmd += "create table if not exists ";           //组入“如果不存在，就创建表”的指令
    cmd += "AlarmData";                    //组入表名
    cmd += "(日期 INTEGER PRIMARY KEY TEXT,设备ID TEXT,防区 TEXT,)";//组入表头
    qDebug()<<cmd;
    if(!sql_q->exec(cmd))                           //执行
    {
        qDebug() << "Error: Fail to create table."<< sql_q->lastError();
    }
    else
    {
        qDebug() << "Table opened!";
    }
}

//将数据加上包头
void DaelAllData::dealSendData(QString sendData,int type)
{
    int len=sendData.size();
    QString str=QString("%1").arg(len,13,10,QLatin1Char('0'));
    QString datah="IALARM:"+str;
    if(type==0)
    {
        datah="IALARM:0000000000000";
    }
    QString dataAll=datah+sendData;
    //处理完传出去
    if(type==0){
        emit dealSendDataToUdp(dataAll);
    }else if(type==1){
        emit dealSendDataToTcp(dataAll);
    }else{
        emit dealSendDataToUi(dataAll);
    }
}

//处理收到数据
void DaelAllData::dealReadData(QString readData, int len,int type)
{
    emit TcpReadDataToUi(readData);//原始数据传给界面
    readData=readData.mid(20,len);
    QDomDocument dom;
    QString errorMsg;
    int errorLine, errorColumn;
    //将数据转换成xml格式
    if(!dom.setContent(readData, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "CHeck Parse error at line " +  errorMsg;
    }
    QDomElement root=dom.documentElement();
    QDomElement firstChild=root.firstChildElement();
    QString  imgDataStr=firstChild.text();
    if(imgDataStr.length()!=0)//判断传入的是不是要图片
    {
        QPixmap image;
        image.loadFromData(QByteArray::fromBase64(imgDataStr.toLocal8Bit()));
        //顺时针旋转270度图片
        QPixmap afterPix(image.size());//画布，大小为显示图片的大小
        afterPix.fill(Qt::transparent);
        QPainter painter(&afterPix);
        painter.translate(image.width()/2,image.height()/2); //让图片的中心作为旋转的中心
        painter.rotate(270); //顺时针旋转270度
        painter.translate(-(image.width()/2),-(image.height()/2)); //使原点复原
        painter.drawPixmap(0,0,image);
        painter.end();
        if(type==0)//判断是报警图片还是时事图片
        {
            emit dealReadDataImgToUi(afterPix);//时事图片传出去
        }else{
            emit dealReadDataAlarmImgToUi(afterPix);//报警图片传出去
            //数据库插入
            //插入
            QDateTime current_date_time;
            QString current_date;
            current_date_time =QDateTime::currentDateTime();
            current_date =current_date_time.toString("yyyy-MM-dd");

            QDomElement e=root.toElement();
            QString aDeviceIP=e.attribute("DeviceIP");
            QString aDefenceID=e.attribute("DefenceID");

            QString instertCmd="";
            instertCmd += "insert into AlarmData";
            instertCmd += " (日期,设备ID,防区,通道,瞬间张力,静态张力)";
            instertCmd += " values(" ;
            instertCmd += '"' + current_date + '"' + ","+ '"' + aDeviceIP + '"' + "," + '"' +aDefenceID + '"' + ")";
            qDebug()<<instertCmd;
            if(!sql_q->exec(instertCmd))                           //执行
            {
                qDebug() << "Error: Fail to insert."<< sql_q->lastError();
            }
            else
            {
                qDebug() << "insert success!";
            }
            emit toSqlArea(db);
        }

    }

}
//处理udp收到的数据
void DaelAllData::dealReadUdpData(QString readUdpData)
{
    emit dealReadDataToUi(readUdpData);//没有处理的数据传到界面，方便观察
    //处理数据
    QString xmlStr=readUdpData.mid(20);
    QDomDocument dom;
    QString errorMsg;
    int errorLine, errorColumn;
    if(!dom.setContent(xmlStr, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "Parse error at line " +  errorMsg;
    }
    QDomElement root=dom.documentElement();
    QDomElement e=root.toElement();
    DeviceIP=e.attribute("DeviceIP");
    if(DeviceIP!=ipTempStr&&DeviceIP!=""){
        emit deviceIP(DeviceIP,ipCount);
        ipCount++;
    }
    ipTempStr=DeviceIP;
   // ui->ipComboBox->addItem(DeviceIP);
}



