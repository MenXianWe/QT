#include "mainwindow.h"
#include "ui_mainwindow.h"
void MainWindow::readMyCom()
{
    QStringList afterCheckData;//存储所有正确数据的链表
    //存储转换后（QByteArray->QString)的数据
    QByteArray data;
    if(ui->serialPortComboBox->currentIndex()==0)
    {
        //myCom0->bytesAvailable() <= 0&&  myCom0->readAll()+
        if (myCom1->bytesAvailable() <= 0&&myCom2->bytesAvailable() <= 0) {
            return;
        }
        data=myCom1->readAll()+myCom2->readAll();

    }
    else{
        if (myCom->bytesAvailable() <= 0) {
            return;
        }
        data=myCom->readAll();
    }

    //tempData=data;
    QByteArray compeletData=data;
    int dataLen = data.length();
    if (dataLen <= 0) {
        return;
    }
    /**判断包完整**/
    if(compeletData.size() == 0) {
        resultState=0;
        return;
    }

    while(compeletData.size() > 0) {
        //寻找帧头0x16的索引
        int index = compeletData.indexOf(0x16);
        if (index < 0) {//没有找到帧头
            resultState=0;
            return;
        }

        //取出命令长度
        if (compeletData.size() < (index + 3)) {
            resultState=0;
            return;
        }
        int length = compeletData.at(index + 3);

        //取出一个完整数据包，包含帧头0x16和校验和
        if (compeletData.size() < (index + length + 5)) {//没有收到一个完整的数据包
            resultState=0;
            return;
        }
        QByteArray VaildCompletePackage = compeletData.mid(index,length + 5);//一个包
        compeletData = compeletData.mid(index + length + 5);//从第index + length + 5位获取以后的所有长度;用于跳出循环

        //计算校验和
        QString temp = "";
        QString hexDataNoSpace= VaildCompletePackage.toHex();

        for (int i = 0; i < hexDataNoSpace.length()-2; i = i + 2) {
            temp += hexDataNoSpace.mid(i, 2) + " ";
        }
        QString testCompleteDataNoTail=temp.trimmed().toUpper();//没有尾巴
        QString testCompleteData=byteArrayToHexStr(VaildCompletePackage);
        int n=testCompleteData.size();
        QString tailStr=QString("%1%2").arg(testCompleteData[n-2]).arg(testCompleteData[n-1]);
        QString testSum=chexkSum(testCompleteDataNoTail);

        if (testSum==tailStr) {//数据校验正确，收到一个正确的完整的数据包
            tempData=VaildCompletePackage;
            resultState=1;
            QString buffer = byteArrayToHexStr(VaildCompletePackage);//转换成QString
            afterCheckData<<buffer;

        } else {//数据校验错误，数据包传输过程中发生错误
            resultState=0;
            return;
        }
    }
    for(int i=0;i<=afterCheckData.size()-1;i++)
    {
        QString recieve=tr("串口接收<<")+afterCheckData[i];
        ui->showDataTextEdit->append(recieve);
    }
    //如果单选框被选中，命令解析文字
    if(ui->translateCheckBox->isChecked())
    {
        analisis(afterCheckData);
    }
}
//发送
void MainWindow::sendData()
{
    QString str = ui->readySendDataComboBox->currentText();
    if (str.isEmpty()) {
        ui->readySendDataComboBox->setFocus();
        return;
    }

    sendData(str);
}

void MainWindow::sendData(QString data)
{

    QByteArray buffer;
    buffer =hexStrToByteArray(data);
    if(ui->serialPortComboBox->currentIndex()==0)
    {
        //myCom0->write(buffer);
        myCom1->write(buffer);
        myCom2->write(buffer);
    }else{
        myCom->write(buffer);
    }

    QString sendTemp=tr("串口发送>>")+data;
    ui->showDataTextEdit->setText(sendTemp);
}

void MainWindow::portSetting(QextSerialPort *portCom)
{
    portCom ->open(QIODevice::ReadWrite);
    if(ui->baudRateComboBox->currentText()==tr("4800")) //根据组合框内容对串口进行设置
        portCom->setBaudRate(BAUD4800);
    else if(ui->baudRateComboBox->currentText()==tr("9600"))
        portCom->setBaudRate(BAUD9600 );
    else if(ui->baudRateComboBox->currentText()==tr("19200"))
        portCom->setBaudRate(BAUD19200 );
    //设置波特率
    if(ui->dataBitComboBox->currentText()==tr("5"))
        portCom->setDataBits(DATA_5);
    else if(ui->dataBitComboBox->currentText()==tr("6"))
        portCom->setDataBits(DATA_6);
    else if(ui->dataBitComboBox->currentText()==tr("7"))
        portCom->setDataBits(DATA_7);
    else if(ui->dataBitComboBox->currentText()==tr("8"))
        portCom->setDataBits(DATA_8);
    //设置数据位
    if(ui->parityComboBox->currentText()==tr("无"))
        portCom->setParity(PAR_NONE);
    else if(ui->parityComboBox->currentText()==tr("奇"))
        portCom->setParity(PAR_ODD);
    else if(ui->parityComboBox->currentText()==tr("偶"))
        portCom->setParity(PAR_EVEN);
    //设置奇偶校验
    if(ui->stopComboBox->currentText()==tr("1"))
        portCom->setStopBits(STOP_1);
    else if(ui->stopComboBox->currentText()==tr("2"))
        portCom->setStopBits(STOP_2);
    //设置停止位
    portCom->setFlowControl(FLOW_OFF); //设置数据流控制，我们使用无数据流控制的默认设置
}

void MainWindow::openLightF()
{
    QString sendLightOpenIDStr="BURGLARY";
    // QString headStr="16";
    QString beforeData="16 00 01 11 D3";
    QString sendLightData=ui->modeComboBox->currentText()+" "+sendLightOpenIDStr+" "+ui->lightComboBox->currentText();//字符
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
    ui->showDataTextEdit->setText(sendTemp);
    ui->showDataTextEdit->append(tr("串口发送>>")+sendLightData);
}

void MainWindow::closeLightF()
{
    QString sendLightOpenIDStr="DISARMED";
    // QString headStr="16";
    QString beforeData="16 00 01 11 D3";
    QString sendLightData=ui->modeComboBox->currentText()+" "+sendLightOpenIDStr+" "+ui->lightComboBox->currentText();//字符
    QString allIdData=stringToCharToHexStr(sendLightData)+" "+"0D 0A";//实际命令
    QString imComData=beforeData+" "+allIdData;//去掉头的
    QString tail=chexkSum(imComData);//校验和
    QString completeSendData=imComData+" "+tail;
    QByteArray buffer;
    buffer =hexStrToByteArray(completeSendData);
    if(ui->serialPortComboBox->currentIndex()==0)
    {
       // myCom0->write(buffer);
        myCom1->write(buffer);
        myCom2->write(buffer);
    }else{
        myCom->write(buffer);
    }
    QString sendTemp=tr("串口发送>>")+completeSendData;
    ui->showDataTextEdit->setText(sendTemp);
    ui->showDataTextEdit->append(tr("串口发送>>")+sendLightData);
}

QString MainWindow::chexkSum(QString s)
{
    bool ok;
    long hexInt=0;
    for (int i =0; i < s.length(); i = i + 3) {//数据有空格，所以加三
        QString temp=s.mid(i, 2);
        hexInt+=temp.toInt(&ok,16);
    }
    int n=(hexInt-0x16)%256;
    return QString::number(n,16).toUpper();
}
