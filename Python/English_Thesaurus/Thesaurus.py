import json
import os

from difflib import get_close_matches

fileName = "data.json"

if not os.path.exists(fileName):
    raise Exception("Data file does not exist!")

data = json.load(open(fileName, 'r'))    

def getCloseMatches(word):
    return get_close_matches(word, data.keys())

def printDefinition(word):
    wordIsInDatabase = False
    wordIsInDatabase = word in data

    if not wordIsInDatabase:
        word = word.lower()
        wordIsInDatabase = word in data

    if wordIsInDatabase:
        definitions = data[word]
        for definition in definitions:
            print(definition)
            
        return

    # The list is ordered, the first element is the closest match.
    closestMatches = getCloseMatches(word)
    if (len(closestMatches) > 0):
        yesOrNo = input("Did you mean %s instead? Enter Y if yes or N if no: " % closestMatches[0])

        if (yesOrNo == 'Y'):
            printDefinition(closestMatches[0])
            return

        elif (yesOrNo != 'N'):
            ("We did not understand your entry!")        
            
    print("The world does not exist in the dictionary!")

word = input("Enter word: ")    
printDefinition(word)