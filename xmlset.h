#ifndef XMLSET_H
#define XMLSET_H
#include <QXmlStreamReader>
#include <QDomDocument>
#include "headinfo.h"

class XMLSet
{
public:
    XMLSet();
    static void setUploadStrangerXML(QString stName);
    static QString strangerXMLFile;
private:


};

#endif // XMLSET_H
