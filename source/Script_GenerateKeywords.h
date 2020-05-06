#ifndef SCRIPT_GENERATEKEYWORDS_H
#define SCRIPT_GENERATEKEYWORDS_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>

struct KeywordEntry{
    QString command;
    QString keyword;

    KeywordEntry(QString command, QString symbol)
        : command(command), keyword(symbol) {}
};

int generateKeywords(QTextStream& out){
    //Open table file
    QFile table(":/TableKeywords.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

    //Process table file
    std::vector<KeywordEntry> rows;
    table.readLine(); //Discard headers
    unsigned line = 1;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        QList<QString> entries = line_str.split(",");

        QString command = entries.at(0);
        QString keyword = entries.at(1);

        rows.push_back(KeywordEntry(command, keyword));
    }

    //Macro definition
    out << "#define MATHBRAN_LATEX_KEYWORDS {\\\n";

    for(KeywordEntry e : rows)
        out << "    { \"" << e.keyword << "\" , \"" << e.command << "\" },\\\n";
    out << "}\n\n";

    return 0;
}

#endif // SCRIPT_GENERATEKEYWORDS_H
