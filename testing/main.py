from collections import Counter

def file_to_text():
    with open("../data/infiniteJest.txt", 'r', encoding="utf-8") as file:
        return file.read()


def get_max_token(text):
    dic = Counter()
    for i in range(len(text) - 1):  
        pair = text[i] + text[i + 1]
        dic[pair] += 1

    if not dic:
        print("ERROR")
        return None

    token, max_count = max(dic.items(), key=lambda x: x[1])
    return token, max_count



def get_new_text(text, token):
    new_text = []
    i = 0

    while i < len(text) - 1:
        pair = text[i] + text[i + 1]
        if pair == token:
            new_text.append(token)  # Merge token
            i += 2  # Skip the next character (since we merged)
        else:
            new_text.append(text[i])
            i += 1

    if i < len(text):  # Append the last character if not merged
        new_text.append(text[i])

    return new_text

def main():
    vocab = []
    counts = []
    text = file_to_text()
    total = 0
    for _ in range(100):
        token,count = get_max_token(text)
        vocab.append(token)
        counts.append(count)
        text = get_new_text(text,token)
        total +=1 
    with open("results.txt",'w') as file:
        for word,count in zip(vocab,counts):
            file.write(f"{word}: {count}\n")


    
if __name__ == '__main__':
    main()


