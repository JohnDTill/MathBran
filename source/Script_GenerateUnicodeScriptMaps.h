#ifndef SCRIPT_GENERATEUNICODESCRIPTMAPS_H
#define SCRIPT_GENERATEUNICODESCRIPTMAPS_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>

struct ScriptEntry{
    const QString symbol;
    const QString script;
    ScriptEntry(QString symbol, QString script) : symbol(symbol), script(script) {}
};

int generateUnicodeMacros(){
    //Open files
    QFile table(":/TableUnicodeSubscripts.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

    QFile gen_file("../include/QMathBranUnicode_Macros.h");
    if(!gen_file.open(QIODevice::WriteOnly)){
        qDebug() << gen_file.errorString();
        return 0;
    }

    //Header information
    QTextStream out(&gen_file);
    out.setCodec(QTextCodec::codecForMib(106)); //UTF8
    out << "//CODE-GEN FILE\n"
           "//This file defines macros to make use of the MathBran serialization format.\n"
           "//The list of definitions is generated from a spreadsheet in the MathBran project.\n\n"
           "#ifndef QMATHBRANUNICODE_MACROS\n"
           "#define QMATHBRANUNICODE_MACROS\n\n";

    //Process table file
    std::vector<ScriptEntry> rows;
    table.readLine(); //Discard headers
    unsigned line = 1;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        QList<QString> entries = line_str.split(",");

        QString symbol = entries.at(0);
        QString script = entries.at(1);

        if(symbol.contains(' ') || script.contains(' ')){
            qDebug() << "\"TableUnicodeSubscripts.csv\" line " << line << " has a space character";
            continue;
        }

        rows.push_back(ScriptEntry(symbol, script));
    }

    //Macro definition
    out << "#define MATHBRAN_SUBSCRIPT_ALLOWED_CASES";
    for(ScriptEntry e : rows)
        out << " \\\n\tcase " << e.symbol.front().unicode() << ":";
    out << "\n\n"
           "#define MATHBRAN_SYMBOL_TO_SUBSCRIPT_MAP";
    for(ScriptEntry e : rows)
        out << " \\\n\tcase " << e.symbol.front().unicode() << ":"
               " out << QChar(" << e.script.front().unicode() << "); break;";
    out << "\n\n";

    //Switch table
    table.close();
    table.setFileName(":/TableUnicodeSuperscripts.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

    //Process table file
    rows.clear();
    table.readLine(); //Discard headers
    line = 1;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        QList<QString> entries = line_str.split(",");

        QString symbol = entries.at(0);
        QString script = entries.at(1);

        if(symbol.contains(' ') || script.contains(' ')){
            qDebug() << "\"TableUnicodeSuperscripts.csv\" line " << line << " has a space character";
            continue;
        }

        rows.push_back(ScriptEntry(symbol, script));
    }

    //Macro definition
    out << "#define MATHBRAN_SUPERSCRIPT_ALLOWED_CASES";

    for(ScriptEntry e : rows)
        out << " \\\n\tcase " << e.symbol.front().unicode() << ":";
    out << "\n\n"
           "#define MATHBRAN_SYMBOL_TO_SUPERSCRIPT_MAP";
    for(ScriptEntry e : rows)
        out << " \\\n\tcase " << e.symbol.front().unicode() << ":"
               " out << QChar(" << e.script.front().unicode() << "); break;";
    out << "\n\n#endif\n";

    return 0;
}

#endif // SCRIPT_GENERATEUNICODESCRIPTMAPS_H
