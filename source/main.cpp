#include "Script_GenerateQtHeaders.h"
#include "Script_GenerateSymbols.h"
#include "Script_GenerateKeywords.h"

int main(int, char**){
    generateQtHeaders();


    //Open LaTeX gen file for writing
    QFile file("../include/QMathBranLatex_Macros.h");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << file.errorString();
        return 0;
    }
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForMib(106)); //UTF8
    out << "//CODE-GEN FILE\n"
           "//This file is generated from subfolder \"meta\".\n"
           "//Changes to this file must be made in the meta project.\n"
           "\n";
    generateKeywords(out);
    generateSymbols(out);
    file.close();
}
