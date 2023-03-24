#include "Variable.hpp"

#include <iostream>

void Variable::setStartAddr(const int startAdr) {
    startAddr = startAdr;
}

void Variable::setEndAddr(const int endAdr) {
    endAddr = endAdr;
}

void Variable::setBlockSize(const int size) {
    blockSize = size;
}
void Variable::setRefCount(const int refCount) {
    referenceCount = refCount;
}

void Variable::setVarName(const std::string varName) {
    name = varName;
}

int Variable::getStartAddr() { return startAddr; }
int Variable::getEndAddr() { return endAddr; }
int Variable::getBlockSize() { return blockSize; }
int Variable::getRefCount() { return referenceCount; }
std::string Variable::getVarName() { return name; }

void Variable::reference(Variable var) {
    startAddr = var.startAddr;
    endAddr = var.endAddr;
    blockSize = var.blockSize;
    referenceCount = var.referenceCount;
    name = var.name;
}
