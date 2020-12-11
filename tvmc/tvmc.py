#!/usr/bin/env python
import os
import argparse
import json
import re

#################################################################
##### Main classes for code-gen & defining a TVS IO Mapping #####
#################################################################

# TODO Add in a generated header that indicates source files, date/time, git commit, etc.. -JWP

# TODO: you're mixing some pretty nasty class naming conventions...
#       clean that up before this gets too far... 
# NOTE: I'm not sure where this todo is from and if its been todone -JWP

# TODO: you will need to update the generated code & CFS app to interpret
#       multiple messages possibly from TVS per frame... this is b.c. 
#       TVS has a max msg size as well as member count apparently...
#       need to test to verify how all that works...
# NOTE: I'm not sure where this todo is from and if its been todone -JWP

class TvsIoCodeGenerator:

    MaxCmdStrlenMacro = "TVS_IO_MAX_COMMAND_STRLEN" 

    def __init__(self, maxCmdStrlen = 1024):
        self.Mappings = []
        self.MaxCmdStrlen = maxCmdStrlen

    def AddMapping(self, mapping):
        self.Mappings.append(mapping)

    def GenerateHeaderFileData(self):

        magicCode = "#ifndef __TVS_IO_GENERATED_H__\n#define __TVS_IO_GENERATED_H__\n\n"
        magicCode += "#include <stdint.h>\n\n"
        magicCode += "#include \"cfe_sb.h\"\n\n"
        magicCode += "#include \"tvs_io_private_types.h\"\n\n"

        for mapping in self.Mappings:
            magicCode += mapping.EmitIncludeMessages()

        magicCode += "\nstatic const int TVS_IO_TOTAL_VARS_CONN[] = {" + str(self.GetTotalMemberPerConnCount()) + "};\n"
        magicCode += "#define TVS_IO_MAPPING_COUNT " + str(len(self.Mappings)) + "\n"
        magicCode += "#define TVS_IO_MAX_COMMAND_STRLEN " + str(self.MaxCmdStrlen) + "\n\n"

        magicCode += "void TVS_IO_InitGeneratedCode(TVS_IO_Mapping *mappings);\n\n"

        for mapping in self.Mappings:
            magicCode += mapping.EmitMacroDefs() + "\n"

            if mapping.FlowDirection & 1:
                magicCode += mapping.EmitInitMessagesDeclaration() + "\n"
                magicCode += mapping.EmitUnpackDeclaration()
            
            if mapping.FlowDirection & 2:
                magicCode += mapping.EmitPackDeclaration() + "\n"

        magicCode += "#endif // __TVS_IO_GENERATED_H__"

        return magicCode
    
    def GenerateSourceFileData(self):

        magicCode = "#include <string.h>\n\n"
        magicCode += "#include \"tvs_io_generated.h\"\n"
        magicCode += "#include \"tvs_io_utils.h\"\n\n"

        for mapping in self.Mappings:

            if mapping.FlowDirection & 1:
                magicCode += mapping.EmitInitMessagesDefinition() + "\n"

        # generate the initialization function
        magicCode += "void TVS_IO_InitGeneratedCode(TVS_IO_Mapping *mappings)\n{\n"

        if len(self.Mappings) == 0:

            magicCode += "\tOS_printf(\"TVS_IO WARNING: Initialized with no mapping definitions...\\n\");\n"
        
        else:

            for x in range(0, len(self.Mappings)):

                mapping = self.Mappings[x]

                magicCode += "\tmappings[" + str(x) + "].memberCount = " + mapping.MemberCountMacro + ";\n"
                magicCode += "\tmappings[" + str(x) + "].msgId = " + mapping.MsgIdString + ";\n"

                magicCode += "\tmappings[" + str(x) + "].commandCode = " + str(mapping.CommandCode) + ";\n"
                
                magicCode += "\tmappings[" + str(x) + "].packetType = " + str(0 if mapping.IsTelemetry else 1) + ";\n"
                magicCode += "\tmappings[" + str(x) + "].flowDirection = " + str(mapping.FlowDirection) + ";\n"
                magicCode += "\tmappings[" + str(x) + "].connectionIndex = " + str(mapping.ConnectionIndex) + ";\n"

                if mapping.FlowDirection & 1:

                    magicCode += "\tmappings[" + str(x) + "].initMessages = " + mapping.InitMessagesMemberName + ";\n"
                    #TODO don't these mallocs need free() cleanup to prevent creating memory leaks? -JWP
                    magicCode += "\tmappings[" + str(x) + "].unpackedDataBuffer = (char*)malloc(sizeof(" + mapping.StructureName + "));\n"
                    
                    magicCode += "\tCFE_SB_InitMsg(mappings[" + str(x) + "].unpackedDataBuffer,\n\t\t\t\t\t"
                    magicCode += mapping.MsgIdString + ", sizeof(" + str(mapping.StructureName) + "), TRUE);\n\n"

                    if not mapping.IsTelemetry:
                        magicCode += "\tCFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)mappings[" + str(x) + "].unpackedDataBuffer, " + str(mapping.CommandCode) + ");\n"

                    magicCode += "\tmappings[" + str(x) + "].unpack = " + mapping.UnpackFunctionName + ";\n\n"

                if mapping.FlowDirection & 2:

                    #TODO don't these mallocs need free() cleanup to prevent creating memory leaks? -JWP
                    magicCode += "\tmappings[" + str(x) + "].packedCommandBuffer = (char**)malloc(" + str(mapping.MemberCount()) + "*sizeof(char*));\n"
                    magicCode += "\tfor (int i = 0; i < " + str(mapping.MemberCount()) + "; ++i)\n"
                    #TODO don't these mallocs need free() cleanup to prevent creating memory leaks? -JWP
                    magicCode += "\t\tmappings[" + str(x) + "].packedCommandBuffer[i] = (char*)malloc(" + str(self.MaxCmdStrlen) + ");\n\n"

                    magicCode += "\tmappings[" + str(x) + "].pack = " + mapping.PackFunctionName + ";\n\n"

        magicCode += "}\n\n"

        for mapping in self.Mappings:

            if mapping.FlowDirection & 1:
                magicCode += mapping.EmitUnpackDefinition() + "\n"
            
            if mapping.FlowDirection & 2:    
                magicCode += mapping.EmitPackDefinition() + "\n"
        
        return magicCode
    
    def GetTotalMemberPerConnCount(self):
        # Find the highest connectionIndex to get the total number of connections, kinda sloppy
        idxMax = 0
        for mapping in self.Mappings:
            if mapping.FlowDirection == 1 or mapping.FlowDirection == 3:
                if mapping.ConnectionIndex > idxMax: idxMax = mapping.ConnectionIndex
        count = [0] * (idxMax+1) # Create a zeroed list of length idxMax plus one

        for mapping in self.Mappings:
            if mapping.FlowDirection == 1 or mapping.FlowDirection == 3:
                count[mapping.ConnectionIndex] = count[mapping.ConnectionIndex] + len(mapping.MemberList)

        return str(count)[1:-1]

class TvsIoMapping:

    def __init__(self, msgIdString, structureName, structureFilename, flowDirection, commandCode = None, connectionIndex = None):
        self.MsgIdString = msgIdString
        self.MsgId = int(msgIdString, 16)
        self.FlowDirection = flowDirection
        self.CommandCode = 0 if commandCode is None else commandCode
        self.ConnectionIndex = 0 if connectionIndex is None else connectionIndex

        self.IsTelemetry = True
        if ((self.MsgId & int('0x1000', 16)) >> 12 == 1):
            self.IsTelemetry = False

        self.StructureName = structureName
        self.MemberList = []
        self.MemberCountMacro = "TVS_" + structureName.upper() + "_MEMBER_COUNT"
        self.CfsStructureFilename = structureFilename
        self.InitMessagesMemberName = "TVS_" + self.StructureName + "_Init_Msgs"

        self.PackFunctionName = "TVS_Pack_" + self.MsgIdString
        self.UnpackFunctionName = "TVS_Unpack_" + self.MsgIdString

    def MemberCount(self):

        return len(self.MemberList)

    def EmitIncludeMessages(self):

        return "#include \"" + self.CfsStructureFilename + "\"\n"

    def EmitMacroDefs(self):

        return "#define " + self.MemberCountMacro + " " + str(len(self.MemberList)) + "\n"

    def EmitInitMessagesDeclaration(self):

        magicCode = "extern char *" + self.InitMessagesMemberName + "[" + self.MemberCountMacro
        magicCode += "];\n"

        return magicCode

    def EmitInitMessagesDefinition(self):

        magicCode = "char *" + self.InitMessagesMemberName + "[" + self.MemberCountMacro
        magicCode += "] = \n{\n"

        for member in self.MemberList:
            magicCode += "\t\"" + member.EmitInitMessage() + "\",\n"

        magicCode += "};\n"
        return magicCode

    def EmitPackDeclaration(self):

        magicCode = "void " + self.PackFunctionName
        magicCode += "(void **buffer, void *mystruct);\n"
        return magicCode

    def EmitUnpackDeclaration(self):

        magicCode = "uint32_t " + self.UnpackFunctionName
        magicCode += "(void *mystruct, void *buffer);\n"
        return magicCode

    def EmitPackDefinition(self):

        magicCode = "void " + self.PackFunctionName
        magicCode += "(void **buffer, void *mystruct)\n{\n\t"
        magicCode += self.StructureName + " *mystructptr = ("
        magicCode += self.StructureName + "*)mystruct;"
        magicCode += "\n\tchar **data = (char**)buffer;\n\n"

        for member in self.MemberList:
            magicCode += member.EmitPackCode()
        
        magicCode += "}\n"
        return magicCode

    def EmitUnpackDefinition(self):

        magicCode = "uint32_t " + self.UnpackFunctionName
        magicCode += "(void *mystruct, void *buffer)\n"
        magicCode += "{\n\t" + self.StructureName + " *mystructptr = ("
        magicCode += self.StructureName + "*)mystruct;"
        magicCode += "\n\tchar *data = (char*)buffer;\n\n"

        magicCode += "\tuint32 byteOffset = 0;\n"
        magicCode += "\tint32 currentMemberLength = -1;\n\n"

        for member in self.MemberList:
            magicCode += member.EmitUnpackCode()

        magicCode += "\treturn byteOffset;\n"
        magicCode += "}\n"
        return magicCode

####################################################
######## TVS IO Supported Primitive Types ##########
####################################################

class TvsIoPrimitiveMapping(object):

    def __init__(self, trickFieldName, cfsFieldName, memberIndex, length = None):
        self.TrickFieldName = trickFieldName
        self.CfsFieldName = cfsFieldName
        self.MemberIndex = memberIndex
        self.Length = length
        
    def EmitInitMessage(self):

        magicCode = "trick.var_add(\\\"" + self.TrickFieldName
        magicCode += "\\\") \\n"

        return magicCode

    def EmitPackBySprintf(self, formatSpecifier):

        magicCode = "\tsnprintf(data[" + str(self.MemberIndex)
        magicCode += "], " + TvsIoCodeGenerator.MaxCmdStrlenMacro
        magicCode += ", \"trick.var_set('" + self.TrickFieldName
        magicCode += "', " + formatSpecifier +  ") \\n\", mystructptr->" + self.CfsFieldName
        magicCode += ");\n"

        return magicCode

class TvsIoByteArray(TvsIoPrimitiveMapping):

    def __init__(self, trickFieldName, cfsFieldName, memberIndex, length):
        super(TvsIoByteArray, self).__init__(
            trickFieldName, cfsFieldName, memberIndex, length)

    def EmitPackCode(self):
        return self.EmitPackBySprintf("'%s'")

    def EmitUnpackCode(self):

        magicCode = "\tcurrentMemberLength = *((int32*) &data[byteOffset + 4] );\n"        
        magicCode += "\tmemcpy(mystructptr->" + self.CfsFieldName
        magicCode += ", &data[byteOffset + 8], currentMemberLength);\n"
        magicCode += "\tbyteOffset += 8 + currentMemberLength;\n\n"

        return magicCode

# TvsIoInt, TvsIoFloat and TvsIoDouble should work for all sizes of 
# floating point and integer types since we are handling
# all values by assignment by default... this can be overriden
# with custom behavior by creating new mapping types if needed...

class TvsIoInt(TvsIoPrimitiveMapping):

    def __init__(self, trickFieldName, cfsFieldName, memberIndex, signed = True):
        super(TvsIoInt, self).__init__(
            trickFieldName, cfsFieldName, memberIndex)
        self.Signed = signed

        self.PackCodeFormatSpecifier = "%d" if signed else "%u"

    def EmitPackCode(self):
        return self.EmitPackBySprintf(self.PackCodeFormatSpecifier)

    def EmitUnpackCode(self):

        magicCode = "\tcurrentMemberLength = *((int32*) &data[byteOffset + 4] );\n"
        magicCode += "\tmystructptr->" + self.CfsFieldName + " = TVS_Unpack"
        magicCode += "Signed" if self.Signed else "Unsigned"
        magicCode += "Integer( &data[byteOffset + 8], currentMemberLength );\n"
        magicCode += "\tbyteOffset += 8 + currentMemberLength;\n\n"

        return magicCode

class TvsIoFloat(TvsIoPrimitiveMapping):

    def __init__(self, trickFieldName, cfsFieldName, memberIndex):
        super(TvsIoFloat, self).__init__(
            trickFieldName, cfsFieldName, memberIndex, length = 4)

    def EmitPackCode(self):
        return self.EmitPackBySprintf("%f")

    def EmitUnpackCode(self):

        magicCode = "\tmystructptr->" + self.CfsFieldName + " = TVS_UnpackFloat( &data[byteOffset + 8] );\n"
        magicCode += "\tbyteOffset += 12;\n\n"

        return magicCode

class TvsIoDouble(TvsIoPrimitiveMapping):

    def __init__(self, trickFieldName, cfsFieldName, memberIndex):
        super(TvsIoDouble, self).__init__(
            trickFieldName, cfsFieldName, memberIndex, length = 8)

    def EmitPackCode(self):
        return self.EmitPackBySprintf("%f")

    def EmitUnpackCode(self):

        magicCode = "\tmystructptr->" + self.CfsFieldName + " = TVS_UnpackDouble( &data[byteOffset + 8] );\n"
        magicCode += "\tbyteOffset += 16;\n\n"

        return magicCode

####################################################
################# Begin App Code ###################
####################################################

def main():

    parser = argparse.ArgumentParser(description='TVM Compiler for TVS_IO App.')
    parser.add_argument('-o', '--output', help='Output directory for TVS_IO generated code files.')
    parser.add_argument('tvm_files', metavar='FILE', nargs='*', help='List of paths to TVM Files')

    args = parser.parse_args()

    outputDirectory = ""
    if (args.output):
        outputDirectory = os.path.abspath(args.output)
        if not os.path.exists(outputDirectory):
            print("Output file path '" + outputDirectory + "' does not exist.")
            return 
    else:
        outputDirectory = os.path.abspath(".")

    outputSourceFilePath = os.path.join(outputDirectory, "tvs_io_generated.c")
    outputHeaderFilePath = os.path.join(outputDirectory, "tvs_io_generated.h")

    generator = TvsIoCodeGenerator()

    characterArrayRegex = re.compile("char\[([0-9]+)\]")

    tvm_files = []
    fileObjects = ""
    tvmObjectList = []

    # handle wild-card characters b.c. sometimes they come through w/o being globbed
    for filePath in args.tvm_files:

        if filePath.endswith("*") or filePath.endswith("*.tvm"):
            
            basePath = os.path.dirname(os.path.abspath(filePath))

            try:
                fileNames = os.listdir(basePath)
            except Exception:
                print("\n\nDir '{0}' does not exist\n".format(filePath))
                fileNames = []

            for fileName in fileNames:
                tvm_files.append(basePath + fileName)

        elif filePath.endswith(".tvm"):

            tvm_files.append(filePath)

    if len(tvm_files) == 0:
        print("\n\n *** TVMC WARNING: No TVM file paths specified *** \n\n")

    # # process the tvm files
    for tvmFilePath in tvm_files:

        tvmJsonString = ""

        with open(tvmFilePath, 'r') as tvmFile:
            tvmJsonString = tvmFile.read()

        try:
            fileObjects = json.loads(tvmJsonString)
        except ValueError as err:
            print("\n\nTVMC Error when parsing file '{0}': {1}\n".format(tvmFilePath, err))
            continue
        
        if isinstance(fileObjects, list):
            for singleTemp in fileObjects:
                tvmObjectList.append(singleTemp)
        else:
            tvmObjectList.append(fileObjects)

    for tvmObject in tvmObjectList:
        try:
            msgId = tvmObject['messageId']
            cfsStructureType = tvmObject['cfsStructureType']
            cfsStructureFileName = tvmObject['cfsStructureFileName']
            members = tvmObject['members']
            flowDirection = tvmObject['flowDirection']
        except KeyError as err:
            print("\n\nTVMC Error: missing required parameter '{0}' in tvm file: {1}\n\n".format(err.args[0], tvmFilePath))
            continue

        if flowDirection < 1 or flowDirection > 3:
            print("\n\nTVMC Error: flowDirection in tvm file '{0}' must have a value of 1, 2, or 3!\n\n", tvmFilePath)
            continue

        commandCode = None
        if "commandCode" in tvmObject:
            commandCode = tvmObject["commandCode"]

        connectionIndex = None
        if "connectionIndex" in tvmObject:
            connectionIndex = tvmObject["connectionIndex"]

        nMembers = len(members)

        mapping = TvsIoMapping(msgId, cfsStructureType, cfsStructureFileName, flowDirection, commandCode, connectionIndex)

        for x in range(0, nMembers):

            try:            
                trickVar = members[x]['trickVar']
                trickType = members[x]['trickType']
                cfsVar = members[x]['cfsVar']
                cfsType = members[x]['cfsType']
            except KeyError as err:
                print("\n\nTVMC Error: missing '{0}' parameter in tvm file: {1}\n\n".format(err.args[0], tvmFilePath))
                continue
                
            if trickType.startswith("char"):

                match = characterArrayRegex.match(cfsType)

                if match is not None: # it's a char array

                    if not cfsVar.startswith("pad"):

                        mapping.MemberList.append(TvsIoByteArray(trickVar, cfsVar, x, int(match.group(1))))

                    else: # it's padding
                        
                        continue

                else: # just a single-byte char

                    # if not cfsVar.startwith("pad"):
                        # TODO: add support for single character

                    # else: # it's padding
                        
                        continue

            elif trickType == "float":

                mapping.MemberList.append(TvsIoFloat(trickVar, cfsVar, x))

            elif trickType == "double":

                mapping.MemberList.append(TvsIoDouble(trickVar, cfsVar, x))

            elif trickType == "int":

                mapping.MemberList.append(TvsIoInt(trickVar, cfsVar, x))

            elif trickType == "uint":

                mapping.MemberList.append(TvsIoInt(trickVar, cfsVar, x, False))

        generator.AddMapping(mapping)

    with open(outputHeaderFilePath, "w") as outputHeaderFile:
        outputHeaderFile.write(generator.GenerateHeaderFileData())

    with open(outputSourceFilePath, "w") as outputSourceFile:
        outputSourceFile.write(generator.GenerateSourceFileData())

if __name__ == "__main__":
    main()

