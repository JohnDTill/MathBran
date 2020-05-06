#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextCodec>

static unsigned line = 1;
static QChar construct_symbol;

void skipHeader(QFile& table_file){
    //Discard headers
    table_file.readLine();
    table_file.readLine();
    line += 2;
}

int generateQtHeaders(){
    //Open files
    QFile table_file(":/ConstructCodes.csv");
    if(!table_file.open(QIODevice::ReadOnly)){
        qDebug() << table_file.errorString();
        return 0;
    }

    if(table_file.atEnd()){
        qDebug() << "File is empty.";
        return 0;
    }

    QFile gen_file("../include/QMathBran_Macros.h");
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
           "#ifndef QMATHBRAN_MACROS\n"
           "#define QMATHBRAN_MACROS\n\n";

    //Generate ushort codes for each symbol
    skipHeader(table_file);
    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        line_str.remove('\\'); //Remove escape characters used for GitHub
        QList<QString> entries = line_str.split(','); //Export to CSV with ',' separator

        QString symbol = entries.at(0);
        if(symbol.length() != 1){
            qDebug() << "Warning: Symbol field length not equal to 1 on line " << line++;
            continue;
        }
        if(line==3) construct_symbol = symbol.front();
        QString name = entries.at(1);
        QString ushort_name = "MB_USHORT_" + name.toUpper().replace(' ', '_');
        ushort val = symbol.front().unicode();

        out << "#define " << ushort_name << ' ' << val << " //" << symbol << '\n';
        line++;
    }

    //Generate qchar macros for each symbol
    out << '\n';
    table_file.reset();
    skipHeader(table_file);
    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        line_str.remove('\\');
        QList<QString> entries = line_str.split(',');

        QString symbol = entries.at(0);
        if(symbol.length() != 1) continue;
        QString name = entries.at(1);
        QString ushort_name = "MB_USHORT_" + name.toUpper().replace(' ', '_');
        QString char_name = "MB_" + name.toUpper().replace(' ', '_');

        out << "#define " << char_name << " QChar(" << ushort_name << ") //" << symbol << '\n';
    }

    //Generate escape strings for three special symbols
    out << '\n';
    table_file.reset();
    skipHeader(table_file);
    for(int i = 0; i < 3; i++){
        QString line_str = table_file.readLine();
        line_str.remove('\\');
        QList<QString> entries = line_str.split(',');

        QString symbol = entries.at(0);
        if(symbol.length() != 1) continue;
        QString name = entries.at(1);
        QString escaped_name = "MB_ESCAPED_" + name.toUpper().replace(' ', '_');

        out << "#define " << escaped_name << " \"" << construct_symbol << symbol << "\"\n";
    }

    //Cleanup
    out << "\n#endif\n";
    gen_file.close();

    QDir dir("./");
    dir.removeRecursively();

    return 0;
}
