#ifndef _VARIABLE_HPP_
#define _VARIABLE_HPP_

#include <fstream>
#include <iostream>


class Variable {
    private:
        int startAddr;
        int endAddr;
        int blockSize;
        int referenceCount;
        std::string name;
    public:
        Variable(const int startAdr, const int endAdr, const int refCount, const int size, const std::string varName)
            :startAddr(), endAddr(), blockSize(), referenceCount(), name() {startAddr = startAdr; endAddr = endAdr; blockSize = size; referenceCount = refCount; name = varName;}

        Variable(const std::string varName, const Variable &var) {
            startAddr = var.startAddr;
            endAddr = var.endAddr;
            blockSize = var.blockSize;
            referenceCount = var.referenceCount;
            name = varName;
        }

        void setStartAddr(const int startAdr);
        void setEndAddr(const int endAdr);
        void setBlockSize(const int size);
        void setRefCount(const int refCount);
        void setVarName(const std::string varName);

        int getStartAddr();
        int getEndAddr();
        int getBlockSize();
        int getRefCount();
        std::string getVarName();

        void reference(Variable var);
};

#endif