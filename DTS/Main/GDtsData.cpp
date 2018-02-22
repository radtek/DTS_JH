#include "GDtsData.h"

void MessageBox_Information(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, "确定");
    msgBox.setWindowTitle("通知");
    msgBox.setText(text);
    msgBox.exec();
}

void MessageBox_Warning(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, "确定");
    msgBox.setWindowTitle("警告");
    msgBox.setText(text);
    msgBox.exec();
}
void MessageBox_Critical(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, "确定");
    msgBox.setWindowTitle("错误");
    msgBox.setText(text);
    msgBox.exec();
}

bool MessageBox_Question(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Cancel, "取消");
    msgBox.setButtonText(QMessageBox::Yes, "确定");
    msgBox.setWindowTitle("询问");
    msgBox.setText(text);
    return QMessageBox::Yes == msgBox.exec();
}

int MessageBox_Question3(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Cancel, "取消");
    msgBox.setButtonText(QMessageBox::Yes, "确定");
    msgBox.setButtonText(QMessageBox::No, "放弃");
    msgBox.setWindowTitle("询问");
    msgBox.setText(text);
    return msgBox.exec();
}



GERROR GDtsData::Initialize()
{
    QDomDocument xmldoc;

    QFile file(GDtsData::QLoadVariant("Default_String_Path").toString());
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning().noquote() << QString("Open XML file error, FileName[%1]").arg(file.fileName());
        return GERROR_XML_OPEN_FILE_FAIL;
    }

    QByteArray strContent = file.readAll();
    file.close();

    QString errorStr;
    int errorLine = 0;
    int errorCol = 0;
    if (!xmldoc.setContent(strContent, true, &errorStr, &errorLine, &errorCol))
    {
        qWarning() << QStringLiteral("Parse XML file error, Error[%1], Position[ln:%2 col:%3]").arg(errorStr).arg(errorLine).arg(errorCol);
        return GERROR_XML_PARSE_FILE_FAIL;
    }

    QDomElement xmlElemRoot = xmldoc.documentElement();
    QDomElement xmlElemString = xmlElemRoot.firstChildElement("String");
    while (!xmlElemString.isNull())
    {
        QString id = xmlElemString.attribute("id");
        QString string = xmlElemString.text();

        Def_String.insert(id, string);
        xmlElemString = xmlElemString.nextSiblingElement();
    }

    return GERROR_OK;
}

QStringMap GDtsData::Def_String;

const QVariantMap GDtsData::Default_Variant =
{
    {"STR_EMPTY", ""},

    {"OrganizationName", "JHMES"},
    {"ApplicationName", "DTS"},
    {"ApplicationVersion", "1.0.0.0000"},

    {"Default_RegeditConfig_Path", "SOFTWARE"},
    {"Default_String_Path", "String.Configuration.xml"},
    {"Default_SqlManager_Path", "sqlmanager.xml"},



};



