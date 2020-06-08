#ifndef SCRIPT_GENERATESYMBOLS_H
#define SCRIPT_GENERATESYMBOLS_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>

struct SymbolEntry{
    const QString command;
    const QString symbol;

    SymbolEntry(QString command, QString symbol)
        : command(command), symbol(symbol) {}
};

int generateSymbols(QTextStream& out){
    //Open table file
    QFile table(":/TableSymbols.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

    //Process table file
    std::vector<SymbolEntry> rows;
    table.readLine(); //Discard headers
    unsigned line = 1;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        QList<QString> entries = line_str.split(",");

        QString command = entries.at(0);
        QString symbol = entries.at(1);

        rows.push_back(SymbolEntry(command, symbol));
    }

    //Macro definition
    out << "#define MATHBRAN_LATEX_SYMBOLS \\\n";

    for(SymbolEntry e : rows){
        if(e.symbol.front() == 55349) continue; //'�'
        out << "    case " << e.symbol.front().unicode() << ":\\\n"
               "        out << \"\\\\" << e.command << "\";\\\n";

        QChar c = e.command.front();
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            out << "        if(curr < source.size() && isLetter(source[curr])) out << \"{}\";\\\n";

        out << "        break;\\\n";
    }
    out << "\n";

    out << "#define MATHBRAN_LATEX_SYMBOLS_32BIT \\\n";

    for(SymbolEntry e : rows){
        if(e.symbol.front() != 55349) continue; //'�'
        out << "    case " << e.symbol.back().unicode() << ":\\\n"
               "        out << \"\\\\" << e.command << "\";\\\n";

        QChar c = e.command.front();
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            out << "        if(curr < source.size() && isLetter(source[curr])) out << \"{}\";\\\n";

        out << "        break;\\\n";
    }
    out << "\n";

    return 0;
}

#endif // SCRIPT_GENERATESYMBOLS_H
