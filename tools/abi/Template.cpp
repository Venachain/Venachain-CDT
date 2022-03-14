//
// Created by zhou.yang on 2018/10/9.
//

#include "Template.h"

using namespace std;

namespace venachain {

    string generateAbiCPlusPlus(ContractDef &contractDef, ABIDef &abiDef) {
        bool initGen = false;
        string code = "//venachain autogen begin\n";
        code += "extern \"C\" { \n";
        for (auto method : abiDef.abis) {
            code += method.returnType.realTypeName + " ";
            code += method.methodName + "(";
            for (int i = 0; i < method.args.size(); ++i) {
                code += method.types[i].realTypeName + " ";
                code += method.args[i];
                if (i != method.args.size()-1) {
                    code += ",";
                }
            }
            code += ") {\n";
            code += contractDef.fullName + " ";
            string var = contractDef.name + "_venachain";
            code += var + ";\n";
            if (method.returnType.realTypeName != "void"){
                code += "return ";
            }
            code += var + "." + method.methodName + "(";
            for (int i = 0; i < method.args.size(); ++i) {
                code += method.args[i];
                if (i != method.args.size()-1) {
                    code += ",";
                }
            }
            code += ");\n}\n";

            if (method.methodName == "init"){
                initGen = true;
            }
        }

        if (!initGen){
            code += "void init() {\n";
            code += (contractDef.fullName + " " + contractDef.name + "_venachain" + ";\n");
            code += (contractDef.name + "_venachain.init();\n");
            code += "}\n";
        }
        code += "\n}\n//venachain autogen end";
        return code;
    }
}
