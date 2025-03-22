import time
from collections import Counter

def file_to_text():
    """Reads file and converts text into a list of characters."""
    #should use below version but want to match C code and assuming UTF8 
    #because downloading all my data from pdfs converting to utf8
    #with open("../data/infiniteJest.txt", 'r', encoding="utf-8") as file:
    with open("../data/infiniteJest.txt", 'r') as file:
        return list(file.read())

def get_max(dic):
    """Find the most frequent token pair."""
    if not dic:
        print("ERROR")
        return None
    return max(dic.items(), key=lambda x: x[1])[0]

def tokenize(text, token):
    """Performs one step of BPE, replacing the most frequent token pair."""
    dic = Counter()
    new_text = []
    pl = text[0] 
    pr = text[1]
    curr = None
    tmp = pl+pr
    x = 2
    if tmp == token:
        pl = token
        pr = text[2]
        x=3
    for i in range(x,len(text)-1):
        curr = text[i]
        tmp = pr + curr
        if tmp == token:
            pr = token
        else:
            new_text.append(pl)
            dic[pl+pr]+=1
            pl = pr
            pr = curr
    new_text.append(pl)
    curr = text[-1]
    tmp = pr + curr
    if tmp == token:
        dic[pl+token] +=1
        new_text.append(token)
    else:
        dic[pl+pr]+=1
        dic[pr+curr] +=1
        new_text.append(pr)
        new_text.append(curr)

    return get_max(dic),new_text

def main():
    start_time = time.time()
    vocab = []
    text = file_to_text()
    #text = list("llhello worlllldll")
    max_token = None
    for i in range(1000):  
        max_token, text = tokenize(text, max_token)
        if max_token is None:
            print("ERROR TEXT NOT BIG ENOUGH")
            break  
        vocab.append(max_token)
    end_time = time.time()
    elapsed_time = end_time - start_time

    minutes = int(elapsed_time // 60)  
    seconds = elapsed_time % 60  
    with open("../python_version.txt",'w') as file:
        file.write(f"Elapsed time: {minutes}.{int(seconds):02d} minutes\n")
        for i,x in enumerate(vocab):
            file.write(f"{i}: {x}\n")

if __name__ == '__main__':
    main()

