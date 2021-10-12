#include "tgtWire.h"
#include "wireTool.h"

tgt_wire::tgt_wire() {
	tool = TOOL_WIRE;
}

DWORD tgt_wire::exec() {
	wireObjects::execTool(); //wire tool is handled externally
	return 0x01;
}