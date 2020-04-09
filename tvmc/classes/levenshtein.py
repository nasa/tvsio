#!/usr/bin/env python
import re
# 
# Implementation of Levenshtein string distance formula

class TvmLevenshtein:    # TVM parameter Keywords
    tvmKeywords = { "cfsStructureType",
                    "cfsStructureFileName",
                    "messageId",
                    "members",
                    "commandCode",
                    "flowDirection"}

    # TVM Mapping Keywords
    tvmMapwords = { "trickVar",             
                    "trickType",
                    "cfsVar",
                    "cfsType"}
    # Implementation of Levenshtein string distance formula
    def __levenshtein_ratio_and_distance(self, s, t, ratio_calc = False):
        # initialize matrix of zeros
        rows = len(s) + 1
        cols = len(t) + 1
        distance = [[0 for col in range(cols)] for row in range(rows)]
        distRatio = [[0 for col in range(cols)] for row in range(rows)]
        # Populate matrix of zeros with the indeces of each character of both strings
        
        for i in range(1, rows):
            for k in range(1,cols):
                distance[i][0] = i
                distRatio[i][0] = i
                distance[0][k] = k
                distRatio[0][k] = k

        # Iterate over the matric to compute the cost of deletions, insertions and/or substitutions

        for col in range(1, cols):

            for row in range (1, rows):

                if s[row-1] == t[col-1]:
                    cost = 0 # if the characters are the same in the two strings in a given position [i,j] then the cost is 0

                else:
                    cost = 1

                distance[row][col] =    min(distance[row-1][col] + 1,    # Cost of deletions
                                        distance[row][col-1] + 1,       # Cost of insertions
                                        distance[row-1][col-1] + cost)  # Cost of substitutions
                if(ratio_calc == True):
                    distRatio[row][col] =   min(distRatio[row-1][col] + 1,    # Cost of deletions
                                            distRatio[row][col-1] + 1,       # Cost of insertions
                                            distRatio[row-1][col-1] + cost*2)  # Cost of substitutions

        ratio = -1

        if(ratio_calc):
            ratio = ((len(s) + len(t)) - distRatio[row][col]) / (len(s) + len(t))

        return (distance[row][col], ratio)

    def levDist(self, s, t):
        return self.__levenshtein_ratio_and_distance(s, t, False)[0]

    def levRatio(self, s, t):
        return self.__levenshtein_ratio_and_distance(s, t, True)[1]
    
    def levDistAndRatio(self, s, t):
        return self.__levenshtein_ratio_and_distance(s, t, True)

    def __checkElement(self, key, keywords):

        # check each key against keywords list
        if key not in keywords:
            print(" *** TVMC Warning: Non-standard TVM parameter: '{0}' ***".format(key))
            # Levenshtein each key not found
            distances = [];
            for kw in keywords:

                # lower case everything to reduce distances
                tup = self.levDistAndRatio(kw.lower(), key.lower())
                distances.append((kw, tup[0], tup[1]))

            minDist  = min(distances, key=(lambda x : x[1]))
            maxRatio = max(distances, key=(lambda x : x[2]))

            # TODO: More logic to evaluate differences. 
            # 1. Max distance or minimum ratio?
            # 2. fix capitalization
            # fix the element if get a strong Lev result
            print(" *** Matched '{0}' with: '{1}'. Levenshtein analysis: Edits: {2:2} Ratio: {3:.3f}".format(key, minDist[0], minDist[1], minDist[2]))
            return minDist[0]
        else:
            return None

    def checkTvmParam(self, key):
        return self.__checkElement(key, self.tvmKeywords)

    def checkMapKey(self, key):
        return self.__checkElement(key, self.tvmMapwords)

    def fixDict(self, tvmDict, isMembers=False):
        # Check Key of each dict entry, build list for fixing
        fixList = []
        for param in tvmDict.items():
            if isMembers:
                checkStr = self.checkMapKey(param[0])
            else:
                checkStr = self.checkTvmParam(param[0])
            if(checkStr != None):
                fixList.append((param[0], checkStr))

        # Fix each tvmObject entry in list
        for param in fixList:
            tvmDict[param[1]] = tvmDict.pop(param[0])

    def fixCase(self, JsonStr, tvmFilePath):
    
        fixedCase = JsonStr
        for keyword in self.tvmKeywords:
            formedKeyword = "\"{0}\":".format(keyword)
            fixedCase = re.sub(formedKeyword, formedKeyword, fixedCase, flags=re.IGNORECASE)

        return fixedCase