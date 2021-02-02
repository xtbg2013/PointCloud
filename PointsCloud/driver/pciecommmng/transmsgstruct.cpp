#include "transmsgstruct.h"
#include <string.h>

TransMsg CreateOneTransMsg(std::string strMsgName, uint64_t uiDataLen, void *ptrDataBuffHead)
{
    TransMsg transMsg;
    memset(transMsg.cMsgDataName, 0x0, sizeof(transMsg.cMsgDataName));
    memcpy(transMsg.cMsgDataName, strMsgName.c_str(), strMsgName.length());
    transMsg.uiDataLen = uiDataLen;
    transMsg.ptrMsgDataBuff = ptrDataBuffHead;
    return transMsg;
}
