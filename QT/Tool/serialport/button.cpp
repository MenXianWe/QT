#include "serialport.h"
#include "ui_serialport.h"


//打开串口按钮
void SerialPort::on_openComBtn_clicked()
{
    if(ui->serialPortComboBox->currentIndex()==0)
    {
        //qDebug()<<"suoyou";
       // myCom0=new QextSerialPort("ttyS3",QextSerialPort::Polling);
        myCom1=new QextSerialPort("COM5",QextSerialPort::Polling);
        myCom2=new QextSerialPort("COM7",QextSerialPort::Polling);
        //portSetting(myCom0);
        portSetting(myCom1);
        portSetting(myCom2);
    }
    else
    {
        QString portName=ui->serialPortComboBox->currentText();
        myCom=new QextSerialPort(portName,QextSerialPort::Polling);
        portSetting(myCom);
    }
    timerRead->start();
    ui->openComBtn->setEnabled(false); //打开串口后“打开串口”按钮不可用
    ui->closeComBtn->setEnabled(true); //打开串口后“关闭串口”按钮可用
    ui->sendDataBtn->setEnabled(true); //打开串口后“发送数据”按钮可用
    ui->baudRateComboBox->setEnabled(false); //设置各个组合框不可用
    ui->dataBitComboBox->setEnabled(false);
    ui->parityComboBox->setEnabled(false);
    ui->stopComboBox->setEnabled(false);
    ui->serialPortComboBox->setEnabled(false);
    ui->volPushButton->setEnabled(true);
    ui->hStatePushButton->setEnabled(true);
    ui->dePushButton->setEnabled(true);
}

//关闭串口按钮
void SerialPort::on_closeComBtn_clicked()
{
    if(ui->serialPortComboBox->currentIndex()==0)
    {
        //myCom0->close();
        myCom1->close();
        myCom2->close();

    }else{
        myCom->close();
    }
    timerRead->stop();
    ui->openComBtn->setEnabled(true); //关闭串口后“打开串口”按钮可用
    ui->closeComBtn->setEnabled(false); //关闭串口后“关闭串口”按钮不可用
    ui->sendDataBtn->setEnabled(false); //关闭串口后“发送数据”按钮不可用
    ui->baudRateComboBox->setEnabled(true); //设置各个组合框可用
    ui->dataBitComboBox->setEnabled(true);
    ui->parityComboBox->setEnabled(true);
    ui->stopComboBox->setEnabled(true);
    ui->serialPortComboBox->setEnabled(true);
    ui->volPushButton->setEnabled(false);
    ui->hStatePushButton->setEnabled(false);
    ui->dePushButton->setEnabled(false);
}

//发送数据按钮
void SerialPort::on_sendDataBtn_clicked()
{

    sendData();

   // timerRead = new QTimer(this);
   // timerRead->setInterval(100);
    //timerRead->start();
   // connect(timerRead, SIGNAL(timeout()), this, SLOT(readMyCom()));
}

/******主机状态3按钮******/
//状态
void SerialPort::on_hStatePushButton_clicked()
{
    if(resultState==1)
    {
        ui->showDataTextEdit->clear();
        QString str = ui->readySendDataComboBox->currentText();
        QString sendTemp=tr("串口发送>>")+str;
        ui->showDataTextEdit->append(sendTemp);
        QString buffer = byteArrayToHexStr(tempData);
        QString recieve=tr("串口接收<<")+buffer;
        ui->showDataTextEdit->append(recieve);
        showDataCh(tempData);
    }
    else{
        ui->showDataTextEdit->append(tr("无法获取"));
    }
}

//电压
void SerialPort::on_volPushButton_clicked()
{
    if(resultState==1)
    {
        QString hexStr=tempData.toHex();
        bool ok;
        QStringList dataList;
        ui->showDataTextEdit->clear();
        QString str = ui->readySendDataComboBox->currentText();
        QString sendTemp=tr("串口发送>>")+str;
        ui->showDataTextEdit->append(sendTemp);
        QString buffer = byteArrayToHexStr(tempData);
        QString recieve=tr("串口接收<<")+buffer;
        ui->showDataTextEdit->append(recieve);
        for (int i = 0; i < hexStr.length(); i = i + 2) {
            dataList.append(hexStr.mid(i, 2).toUpper()) ;
            // int hexInt=temp.toInt(&ok,16);
        }
        //int idLen=dataList.at(3).toInt(&ok,16)+5;
        if(dataList.at(4)=="D9")
        {
            // ui->showDataTextEdit->append(tr("防区状态："));
            QString tempHexStr=dataList.at(7)+dataList.at(8);
            double dataInt=tempHexStr.toInt(&ok,16)/41.0;
            ui->showDataTextEdit->append(QString(tr("电压：%1")).arg(dataInt));

        }
    }else{
        ui->showDataTextEdit->append(tr("无法获取"));
    }
}

//电压
void SerialPort::on_dePushButton_clicked()
{
    if(resultState==1)
    {
        QString hexStr=tempData.toHex();
        bool ok;
        QStringList dataList;
        ui->showDataTextEdit->clear();
        QString str = ui->readySendDataComboBox->currentText();
        QString sendTemp=tr("串口发送>>")+str;
        ui->showDataTextEdit->append(sendTemp);
        QString buffer = byteArrayToHexStr(tempData);
        QString recieve=tr("串口接收<<")+buffer;
        ui->showDataTextEdit->append(recieve);
        for (int i = 0; i < hexStr.length(); i = i + 2) {
            dataList.append(hexStr.mid(i, 2).toUpper()) ;
            // int hexInt=temp.toInt(&ok,16);
        }
        //int idLen=dataList.at(3).toInt(&ok,16)+5;
        if(dataList.at(4)=="D9")
        {
            ui->showDataTextEdit->append(tr("防区状态："));
            // for(int i=5;i<=idLen-1;i++)
            // {
            int dataInt=dataList.at(6).toInt(&ok,16);
            for(int j=0;j<8;j++)
            {
                int c=dataInt>>j&0x0001;
                switch (j) {
                case 0 :
                    if(c==0){
                        ui->showDataTextEdit->append(tr("防区1正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("防区1报警"));
                    }
                    break;
                case 1:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("防区2正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("防区2报警"));
                    }
                    break;
                case 2:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("防区3正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("防区3报警"));
                    }
                    break;
                case 3:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("防区4正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("防区4报警"));
                    }
                    break;
                case 4:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("防区5正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("防区5报警"));
                    }
                    break;
                case 5:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("防区6正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("防区6报警"));
                    }
                    break;
                case 6:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("防区7正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("防区7报警"));
                    }
                    break;
                case 7:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("防区8正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("防区8报警"));
                    }
                default:
                    break;
                }
            }
        }

    }else
    {
        ui->showDataTextEdit->append(tr("无法获取"));
    }
}


/******灯的状态******/
void SerialPort::on_lightOpenPushButton_clicked()
{
    openLightF();
}

void SerialPort::on_lightClosePushButton_clicked()
{
    closeLightF();
}

void SerialPort::on_allClosePushButton_clicked()
{
    QString sendLightOpenIDStr="DISARMED";
    //QString headStr="16";
    QString beforeData="16 00 01 11 D3";
    for (int i=0;i<4 ;i++ )//地图
    {
        for (int j=0;j<32 ;j++ ) {
            QString sendLightData=ui->modeComboBox->itemText(i)+" "+sendLightOpenIDStr+" "+ui->lightComboBox->itemText(j);//字符
            QString allIdData=stringToCharToHexStr(sendLightData)+" "+"0D 0A";//实际命令
            QString imComData=beforeData+" "+allIdData;//去掉头的
            QString tail=chexkSum(imComData);//校验和
            QString completeSendData=imComData+" "+tail;
            QByteArray buffer;
            buffer =hexStrToByteArray(completeSendData);
            if(ui->serialPortComboBox->currentIndex()==0)
            {
                //myCom0->write(buffer);
                myCom1->write(buffer);
                myCom2->write(buffer);
            }else{
                myCom->write(buffer);
            }
            QString sendTemp=tr("串口发送>>")+completeSendData;
            ui->showDataTextEdit->append(sendTemp);
            ui->showDataTextEdit->append(tr("串口发送>>")+sendLightData);
        }
    }
}

void SerialPort::on_allOpenPushButton_clicked()
{
    QString sendLightOpenIDStr="BURGLARY";
    // QString headStr="16";
    QString beforeData="16 00 01 11 D3";
    for (int i=0;i<4 ;i++ )//地图
    {
        for (int j=0;j<32 ;j++ ) {
            QString sendLightData=ui->modeComboBox->itemText(i)+" "+sendLightOpenIDStr+" "+ui->lightComboBox->itemText(j);//字符
            QString allIdData=stringToCharToHexStr(sendLightData)+" "+"0D 0A";//实际命令
            QString imComData=beforeData+" "+allIdData;//去掉头的
            QString tail=chexkSum(imComData);//校验和
            QString completeSendData=imComData+" "+tail;
            QByteArray buffer;
            buffer =hexStrToByteArray(completeSendData);
            if(ui->serialPortComboBox->currentIndex()==0)
            {
                //myCom0->write(buffer);
                myCom1->write(buffer);
                myCom2->write(buffer);
            }else{
                myCom->write(buffer);
            }
            QString sendTemp=tr("串口发送>>")+completeSendData;
            ui->showDataTextEdit->append(sendTemp);
            ui->showDataTextEdit->append(tr("串口发送>>")+sendLightData);
        }
    }
}

//广播
void SerialPort::on_broadCastPushButton_clicked()
{
    QString broadCastId="16 FF 01 01 E0 E1";//广播命令
    QByteArray buffer;
    buffer =hexStrToByteArray(broadCastId);
    if(ui->serialPortComboBox->currentIndex()==0)
    {
        //myCom0->write(buffer);
        myCom1->write(buffer);
        myCom2->write(buffer);
    }else{
        myCom->write(buffer);
    }
    QString sendTemp=tr("串口发送>>")+broadCastId;
    ui->showDataTextEdit->append(sendTemp);
    timerRead = new QTimer(this);
    timerRead->setInterval(100);
    timerRead->start();
    connect(timerRead, SIGNAL(timeout()), this, SLOT(readMyCom()));
}

//清除命令
void SerialPort::on_clearPushButton_clicked()
{
    ui->showDataTextEdit->clear();
}
