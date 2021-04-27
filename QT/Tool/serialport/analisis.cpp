#include "serialport.h"
#include "ui_serialport.h"
//解析
void SerialPort::analisis(QStringList checkDataList)
{
    bool ok;
    for(int i=0;i<checkDataList.size();i++){
        QString tempOneId=checkDataList.at(i);
        QStringList dataList;
        for (int i = 0; i < tempOneId.length(); i = i + 3) {
            dataList.append(tempOneId.mid(i, 2).toUpper()) ;
            // int hexInt=temp.toInt(&ok,16);
        }
         /*****广播解析*******/
        if(dataList.at(4)=="F0"){
            QString addr=QString("%1").arg(dataList.at(2).toInt(&ok,16));
            QString tempShow="串口接收>>地址："+addr+" "+"状态：(";
            //十六进制转换二进制字符串
            int stateNum=dataList.at(5).toInt(&ok,16);
            QString binStrf="";
            QString binStrz="";
            for(int i=0;i<8;i++)
            {
                int tempStateNum=stateNum;
                binStrf+=QString("%1").arg(tempStateNum>>i&0x0001);
            }
            for(int j=binStrf.size()-1;j>=0;j--)
            {
                binStrz+=binStrf[j];
            }
            tempShow+=binStrz+")";
            ui->showDataTextEdit->append(tempShow);
        }

        /*****主机状态解析*******/
        else if(dataList.at(4)=="D9"){
            int dataInt=dataList.at(5).toInt(&ok,16);
            for(int j=0;j<8;j++)
            {
                int c=dataInt>>j&0x0001;
                switch (j) {
                case 0 :
                    if(c==0){
                        ui->showDataTextEdit->append(tr("放拆正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("放拆报警"));
                    }
                    break;
                case 1:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("电池正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("电池低电"));
                    }
                    break;
                case 2:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("警号正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("警号故障"));
                    }
                    break;
                case 3:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("主板正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("主板故障"));
                    }
                    break;
                case 4:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("电池正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("电池故障"));
                    }
                    break;
                case 6:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("警号不在驱动 "));
                    }else{
                        ui->showDataTextEdit->append(tr("警号在驱动 "));
                    }
                    break;
                case 7:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("AC220供电"));
                    }else{
                        ui->showDataTextEdit->append(tr("电池供电 "));
                    }
                default:
                    break;
                }

            }
        }
    }

}

void SerialPort::showDataCh(QByteArray compeletData)
{
    if(resultState==1)
    {
        //translate
        QString hexStr=compeletData.toHex();
        bool ok;
        QStringList dataList;
        for (int i = 0; i < hexStr.length(); i = i + 2) {
            dataList.append(hexStr.mid(i, 2).toUpper()) ;
            // int hexInt=temp.toInt(&ok,16);
        }
        //int idLen=dataList.at(3).toInt(&ok,16)+5;
        if(dataList.at(4)=="D9")
        {
            ui->showDataTextEdit->append(tr("主机状态："));
            // for(int i=5;i<=idLen-1;i++)
            // {
            int dataInt=dataList.at(5).toInt(&ok,16);
            for(int j=0;j<8;j++)
            {
                int c=dataInt>>j&0x0001;
                switch (j) {
                case 0 :
                    if(c==0){
                        ui->showDataTextEdit->append(tr("放拆正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("放拆报警"));
                    }
                    break;
                case 1:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("电池正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("电池低电"));
                    }
                    break;
                case 2:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("警号正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("警号故障"));
                    }
                    break;
                case 3:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("主板正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("主板故障"));
                    }
                    break;
                case 4:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("电池正常"));
                    }else{
                        ui->showDataTextEdit->append(tr("电池故障"));
                    }
                    break;
                case 6:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("警号不在驱动 "));
                    }else{
                        ui->showDataTextEdit->append(tr("警号在驱动 "));
                    }
                    break;
                case 7:
                    if(c==0){
                        ui->showDataTextEdit->append(tr("AC220供电"));
                    }else{
                        ui->showDataTextEdit->append(tr("电池供电 "));
                    }
                default:
                    break;
                }

            }
        }
    }
    else{
        ui->showDataTextEdit->append(tr("无法获取"));
    }
}

