/*
 * Summary: Internal Interfaces for memory buffers in libxml2
 * Description: this module describes most of the new xmlBuf buffer
 *              entry points, those are private routines, with a
 *              few exceptions exported in tree.h. This was added
 *              in 2.9.0.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_BUF_H__
#define __XML_BUF_H__

#include <libxml/tree.h>

#ifdef __cplusplus
extern "C" {
#endif

xmlBufPtr xmlBufCreate(void);
xmlBufPtr xmlBufCreateSize(size_t size);
#if (XML_ACTIVATE_UNUSED)
/* { */
xmlBufPtr xmlBufCreateStatic(void *mem, size_t size);
/* } */
#endif
int xmlBufSetAllocationScheme(xmlBufPtr buf,
                              xmlBufferAllocationScheme scheme);
int xmlBufGetAllocationScheme(xmlBufPtr buf);

void xmlBufFree(xmlBufPtr buf);
#if (XML_ACTIVATE_UNUSED)
/* { */
void xmlBufEmpty(xmlBufPtr buf);
/* } */
#endif
/* size_t xmlBufShrink(xmlBufPtr buf, size_t len); */
int xmlBufGrow(xmlBufPtr buf, int len);
#if (XML_ACTIVATE_UNUSED)
/* { */
int xmlBufInflate(xmlBufPtr buf, size_t len);
/* } */
#endif
int xmlBufResize(xmlBufPtr buf, size_t len);

int xmlBufAdd(xmlBufPtr buf, const xmlChar *str, int len);
#if (XML_ACTIVATE_UNUSED)
/* { */
int xmlBufAddHead(xmlBufPtr buf, const xmlChar *str, int len);
/* } */
#endif
int xmlBufCat(xmlBufPtr buf, const xmlChar *str);
int xmlBufCCat(xmlBufPtr buf, const char *str);
#if (XML_ACTIVATE_UNUSED)
/* { */
int xmlBufWriteCHAR(xmlBufPtr buf, const xmlChar *string);
int xmlBufWriteChar(xmlBufPtr buf, const char *string);
/* } */
#endif
int xmlBufWriteQuotedString(xmlBufPtr buf, const xmlChar *string);

size_t xmlBufAvail(const xmlBufPtr buf);

#if (XML_ACTIVATE_UNUSED)
/* { */
size_t xmlBufLength(const xmlBufPtr buf);
/* } */
#endif
/* size_t xmlBufUse(const xmlBufPtr buf); */
int xmlBufIsEmpty(const xmlBufPtr buf);
int xmlBufAddLen(xmlBufPtr buf, size_t len);
#if (XML_ACTIVATE_UNUSED)
/* { */
int xmlBufErase(xmlBufPtr buf, size_t len);
/* } */
#endif
/* const xmlChar * xmlBufContent(const xmlBuf *buf); */
/* const xmlChar * xmlBufEnd(xmlBufPtr buf); */

xmlChar * xmlBufDetach(xmlBufPtr buf);

#if (XML_ACTIVATE_UNUSED)
/* { */
size_t xmlBufDump(FILE *file, xmlBufPtr buf);
xmlBufPtr xmlBufFromBuffer(xmlBufferPtr buffer);
xmlBufferPtr xmlBufBackToBuffer(xmlBufPtr buf);
/* } */
#endif
int xmlBufMergeBuffer(xmlBufPtr buf, xmlBufferPtr buffer);

int xmlBufResetInput(xmlBufPtr buf, xmlParserInputPtr input);
#if (XML_ACTIVATE_UNUSED)
/* { */
size_t xmlBufGetInputBase(xmlBufPtr buf, xmlParserInputPtr input);
int xmlBufSetInputBaseCur(xmlBufPtr buf, xmlParserInputPtr input,
                          size_t base, size_t cur);
/* } */
#endif
#ifdef __cplusplus
}
#endif
#endif /* __XML_BUF_H__ */

