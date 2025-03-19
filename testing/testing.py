
from collections import Counter

def file_to_text():
    """Reads file and converts text into a list of characters."""
    with open("../data/infiniteJest.txt", 'r', encoding="utf-8") as file:
        return list(file.read())

def get_max(dic):
    """Find the most frequent token pair."""
    if not dic:
        return None
    #x = max(dic.items(), key=lambda x: x[1])[0]
    #print(f"MAX COUNT: {dic[x]}\n")
    #return x
    return max(dic.items(), key=lambda x: x[1])[0]

def handle_end():
    pass

#think you have to process three at a time
def tokenize(text, token):
    """Performs one step of BPE, replacing the most frequent token pair."""
    dic = Counter()
    new_text = []
    pl = text[0] 
    pr = text[1]
    curr = None
    tmp = None
    tmp = pl+pr
    x = 2
    if tmp == token:
        pl = token
        pr = text[2]
        #new_text.append(token)
        x=3
    #have to do -1 because if you encounter a token you have to add an additional one
    #str = token if token else "No TOKEN FIRST ITERATION"
    #print(f"MAX TOKEN WE ARE WERGING: {str}\n") 
    for i in range(x,len(text)-1):
        curr = text[i]
        '''
        print("************************************************")
        print(f"LEFT: {pl}\n")
        print(f"RIGHT: {pr}\n")
        print(f"CURR: {curr}\n")
        '''
        tmp = pr + curr
        if tmp == token:
            #dic[pl+token]+=1
            #new_text.append(pl)
            #pl = token
            #pr = text[i+1]

            #pl doesn't change
            pr = token
            #i+=1
        else:
            new_text.append(pl)
            dic[pl+pr]+=1
            pl = pr
            pr = curr
        '''    
        print("----------------------------------")
        print("THE DICTIONARY NOW:")
        for key,value in dic.items():
            print(f"{key}: {value}\n")
        print("----------------------------------")
        print("----------------------------------")
        print("THE TEXT NOW:")
        print(new_text)
        print("----------------------------------")
        print("************************************************")
        '''
    #need to append p1
    #append pr 
    #and append curr
    #while also checking for token hits
    '''
    print("************************************************")
    print("-------------------------")
    print("END TOKENEZATION SPECIAL PART")
    print("-------------------------")
    print(f"TEXT LEFT: {pl}\n")
    print(f"TEXT RIGHT: {pr}\n")
    '''
    new_text.append(pl)
    curr = text[-1]
    #print(f"TEXT CURRENT: {pr}\n")
    tmp = pr + curr
    if tmp == token:
        dic[pl+token] +=1
        new_text.append(token)
    else:
        dic[pl+pr]+=1
        dic[pr+curr] +=1
        new_text.append(pr)
        new_text.append(curr)
    '''
        print("PRINTING DIC COUNT")
    for i,x in dic.items():
        print(f"{i}:{x}\n")
    print("PRINTING THE NEW TEXT")
    print(new_text)
    print("************************************************")
   ''' 
         
    return get_max(dic),new_text



def main():
    vocab = []
    text = file_to_text()
    #text = list("llhello worlllldll")
    max_token = None
    for i in range(10000):  
        max_token, text = tokenize(text, max_token)
        if max_token is None:
            print("ERROR TEXT NOT BIG ENOUGH")
            break  
        #print(f"Iteration {i+1}: Merged {max_token}")
        vocab.append(max_token)
    for i in vocab:
        print(i)

if __name__ == '__main__':
    main()

