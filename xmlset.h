#ifndef XMLSET_H
#define XMLSET_H
#include <QXmlStreamReader>
#include <QDomDocument>
#include "headinfo.h"

class XMLSet
{
public:
    XMLSet();
    static void setUploadStrangerXML();
private:
    static QString strangerXMLFile;

};

#endif // XMLSET_H
