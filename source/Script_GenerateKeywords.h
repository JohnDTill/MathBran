#ifndef SCRIPT_GENERATEKEYWORDS_H
#define SCRIPT_GENERATEKEYWORDS_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>

int generateKeywords(QTextStream& out){
    //Open table file
    QFile table(":/TableKeywords.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

    //Process table file
    std::vector<QString> keywords;
    table.readLine(); //Discard headers
    unsigned line = 1;
    int max_keyword_length = 0;
    int min_keyword_length = 999;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        QList<QString> entries = line_str.split(",");

        QString keyword = entries.at(0);
        keywords.push_back(keyword);

        if(keyword.size() < min_keyword_length) min_keyword_length = keyword.size();
        if(keyword.size() > max_keyword_length) max_keyword_length = keyword.size();
    }

    //Find a perfect hash function
    uint8_t best_range = 255;
    uint8_t offset = 0;

    for(uint8_t i = 0; i < 255; i++){
        QList<uint8_t> hashes;
        uint8_t min = 255;
        uint8_t max = 0;

        for(QString str : keywords){
            uint8_t hash = 1;
            for(ushort j = 0; j < str.size(); j++){
                ushort ch = str[j].unicode();
                hash += ch*(i+j);
            }

            if(hash < min) min = hash;
            if(hash > max) max = hash;

            if(hashes.contains(hash)) break;
            else hashes.push_back(hash);
        }

        if(hashes.size() == keywords.size()){
            uint8_t range = max - min;

            if(range < best_range){
                offset = i;
                qDebug() << range;
                best_range = range;
            }
        }
    }

    //Macro definition
    out << "#define MATHBRAN_LATEX_KEYWORD_LOOKUP_DEFINITION \\\n";

    out << "static void searchKeyword(const QString& source, QString::size_type& curr, QTextStream& out){ \\\n"
           "    QString::size_type start = curr-1; \\\n"
           "    while(curr < source.size() && isLetter(source[curr])) curr++; \\\n"
           "\\\n"
           "    int range = curr-start; \\\n"
           "    QStringRef str = source.midRef(start, range); \\\n"
           "    if(range >= " << min_keyword_length << " && range <= " << max_keyword_length << "){ \\\n"
           "        uint8_t hash = 1; \\\n"
           "        for(int i = 0; i < range; i++) hash += source[start+i].unicode()*(i+" << offset << "); \\\n"
           "\\\n"
           "        switch(hash){ \\\n";

    for(QString str : keywords){
        uint8_t hash = 1;
        for(ushort j = 0; j < str.size(); j++){
            ushort ch = str[j].unicode();
            hash += ch*(offset+j);
        }

        out << "            case " << hash << ": if(str==\"" << str << "\") out << '\\\\'; break; \\\n";
    }

    out << "        } \\\n"
           "    } \\\n"
           "\\\n"
           "    out << str; \\\n"
           "}\n\n";

    return 0;
}

#endif // SCRIPT_GENERATEKEYWORDS_H
