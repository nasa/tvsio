#!/usr/bin/env python
import re
import copy

###########################################################
## Implementation of Levenshtein string distance formula ##
###########################################################

class TvmLevenshtein:    # TVM parameter Keywords
    """Implementation of Levenstein string distance formula \n
    For use with parsing 
    """

    # Default TVM parameter Keywords
    tvmKeywords = { "cfsStructureType",
                    "cfsStructureFileName",
                    "messageId",
                    "members",
                    "commandCode",
                    "flowDirection"}

    # Default TVM mapping Keywords
    tvmMapwords = { "trickVar",             
                    "trickType",
                    "cfsVar",
                    "cfsType"}

    def __levenshtein_ratio_and_distance(self, s, t, ratio_calc = False):
        """The actual Levenshtein formula implementation\n
        Args:
            s (str): first string
            t (str): second string
            ratio_calc (bool): flag to calculate the Levenshtein Ratio as well
                (default is False)
        Returns:
            turple (int, float): (distance, ratio)
        """
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
                if ratio_calc == True:
                    distRatio[row][col] =   min(distRatio[row-1][col] + 1,    # Cost of deletions
                                            distRatio[row][col-1] + 1,       # Cost of insertions
                                            distRatio[row-1][col-1] + cost*2)  # Cost of substitutions

        ratio = -1

        if ratio_calc:
            ratio = ((len(s) + len(t)) - distRatio[row][col]) / (len(s) + len(t))

        return (distance[row][col], ratio)

    def levDist(self, s, t):
        """Calculate Levenshtein distance for two strings\n
        Args:
            s (str): first string
            t (str): second string
        Returns:
            int: distance
        """
        return self.__levenshtein_ratio_and_distance(s, t, False)[0]

    def levRatio(self, s, t):
        """Calculate Levenshtein ratio for two strings\n
        Args:
            s (str): first string
            t (str): second string
        Returns:
            float: ratio
        """
        return self.__levenshtein_ratio_and_distance(s, t, True)[1]
    
    def levDistAndRatio(self, s, t):
        """Calculate Levenshtein distance and ratio for two strings\n
        Args:
            s (str): first string
            t (str): second string
        Returns:
            Tuple (int, float): (distance, ratio)
        """
        return self.__levenshtein_ratio_and_distance(s, t, True)

    def __checkElements(self, key, keywords):
        """Check key against provided list of keywords.
        If it doesn't match any, calculate Levenshtein distance and ratio
        the key against each key to find the lowest distance and highest ratio.\n
        Args:
            key (str): The parameter or mapping name to eval
            keywords (list(str)): List of define TVM keywords to eval against
        Returns:
            List(Tuple (str, int, float)): a list of Tuples each represnting a match
                (<variable name>, <Levenshtein distance>, <Levenshtein ratio>)
                And empty list indicates a perfect match
        """
        
        match = ""
        # In single pass, check for lowercase match, and save a result
        for kw in keywords:
            if key.lower() == kw.lower():
                match = kw
                break

        if match == key: # if a **perfect** match was found, nothing to replace
            return []
            
        elif match is not "": # only case is wrong, returning match tuple
            return [(match, 0, 1)]

        else: # Levenshtein against each keyword not found
            # print("\n *** TVMC Warning: Non-standard TVM parameter: '{0}' ***".format(key))
            
            distances = []
            for kw in keywords:

                # lower case everything to reduce distances
                tup = self.levDistAndRatio(kw.lower(), key.lower())
                distances.append((kw, tup[0], tup[1]))

            # might encounter multiple possible minimum distances 
            # eg "cfsStructurefNam" scores 4 for both 
            # "cfsStructureType" and "cfsStructureFileName" 
            # To a human it is clearly supposed to be cfsStructureFileName
            # But computer needs a more help. Thus, the following 
            # isn't good enough
            # # minDist  = min(distances, key=(lambda x : x[1]))

            minDists  = [('d', 999, 1.0)]
            maxRatios = [('r', 999, 0.0)]
            for dist in distances:
                # grab lowest distance matches with ratios
                if dist[1] < minDists[0][1]:
                    minDists.clear()
                    minDists.append(dist)
                elif dist[1] == minDists[0][1]:
                    minDists.append(dist)
                
                # grab highest ratio matches with distances
                if dist[2] > maxRatios[0][2]:
                    maxRatios.clear()
                    maxRatios.append(dist)
                elif dist[2] == maxRatios[0][2]:
                    maxRatios.append(dist)

            distsAndRatios = list(set().union(minDists,maxRatios))
            return distsAndRatios
            

            # TODO: More logic to evaluate differences. 
            # 1. Max distance or minimum ratio?
            # 2. fix capitalization
            # fix the element if get a strong Lev result
            # print(" *** Matched '{0}' with: '{1}'. Levenshtein analysis: Edits: {2:2} Ratio: {3:.3f}".format(key, minDists[0][0], minDist[1], minDist[2]))
            # return minDists[0]

    def fixDict(self, tvmDict, isMapping=False, keywords=None):
        """Evaluate top level item keys against defined keywords 
        and attempt to fix with closest match\n
        Args:
            tvmDict (dict): dict with keys to fix
            isMapping (bool): set True if `tvmDict` is variable mappings
            keywords (list[str]): alternate list of good keys to try fixing dict with
                (default is None)
        Returns:
            dict: Updated copy of `tvmDict`
        """
        
        tmpDict = copy.deepcopy(tvmDict)
        fixList = []

        # Check Key of each dict entry, build list for fixing
        for param in tmpDict.items():
            if keywords is None:
                keywords = self.tvmKeywords if not isMapping else self.tvmMapwords

            checkTups = self.__checkElements(param[0], keywords)
            if len(checkTups) > 0:
                fixList.append((param[0], checkTups))
        
        # process of elimination may help with fixing some elements
        # after some are fixed, try fixing others again
        # co-dependent typos won't be fixed
        totalFixed = 0
        fixed = -1

        while fixed != 0:
            fixed = self.__processFixList(tmpDict, fixList, fixed == -1)
            totalFixed += fixed
        
        return tmpDict

    def __processFixList(self, tmpDict, fixList, firstLoop):
        # Fix each tvmObject entry in list
        fixedList = []
        fixedCnt = 0
        for param in fixList:
            oldName = param[0]
            possibles = param[1]
            
            if(firstLoop): print("\n\n *** TVMC WARNING: Non-standard TVM parameter: '{0}' ***".format(oldName))
            if len(possibles) == 1:
                if(firstLoop):
                    if oldName.lower() == possibles[0][0].lower():
                        print("\n\tFixed casing to '{0}'\n".format(possibles[0][0]))
                    else:
                        print("\n\tMatched '{0}' with: '{1}'. Levenshtein analysis: Edits: {2:2}, Ratio: {3:.3f}\n".format(oldName, possibles[0][0], possibles[0][1], possibles[0][2]))

                tmpDict[param[1][0][0]] = tmpDict.pop(oldName)
                fixedList.append(param)
                continue
            else:
                if(firstLoop): print("\n\tMultiple results for '{0}'".format(oldName))
                pops = []
                for item in possibles:
                    if(firstLoop): print("\t- '{0}' Levenstein Analysis == Edits: {1:2}, Ratio: {2:.3f}".format(item[0], item[1], item[2]) )
                    # see if only one of the possibles doesn't exists. if so, use it
                    
                    if item[0] in tmpDict:
                        if(firstLoop): print("\t\t'{0}' already exists".format(item[0]))
                        pops.append(item)

                for pop in pops:
                    possibles.remove(pop)
                
                if len(possibles) == 1:
                    print("\tOther options eliminated, using '{0}' for '{1}'".format(possibles[0][0], oldName))
                    tmpDict[possibles[0][0]] = tmpDict.pop(oldName)
                    fixedList.append(param)

        for fixed in fixedList:
            fixedCnt += 1
            fixList.remove(fixed)

        return fixedCnt