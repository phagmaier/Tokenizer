
from collections import Counter

def file_to_text():
    """Reads file and converts text into a list of characters."""
    with open("../data/infiniteJest.txt", 'r', encoding="utf-8") as file:
        return list(file.read())

def get_max(dic):
    """Find the most frequent token pair."""
    if not dic:
        return None
    x = max(dic.items(), key=lambda x: x[1])[0]
    print(f"MAX COUNT: {dic[x]}\n")
    return x

def tokenize(text, token):
    """Performs one step of BPE, replacing the most frequent token pair."""
    dic = Counter()
    new_text = []
    curr = None
    for i in range(len(text)-1):
        curr = text[i]+text[i+1]
        if(curr == token):
            if len(new_text) and new_text[-1] != token:
                new_text.pop()
                dic[text[i-1]+text[i]] -=1
                dic[text[i-1]+token]+=1
            text[i+1]=token
        else:
            new_text.append(text[i])
            dic[curr]+=1
    new_text.append(text[-1])
    return get_max(dic),new_text



def main():
    #text = file_to_text()
    text = list("hello worlld")
    max_token = None

    for i in range(3):  # Perform 25 iterations of BPE
        max_token, text = tokenize(text, max_token)
        if max_token is None:
            break  # Stop if no more merges possible
        print(f"Iteration {i+1}: Merged {max_token}")
        print(text)

if __name__ == '__main__':
    main()

