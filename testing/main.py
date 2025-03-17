TOKENS = 1000

class Token:
    def __init__(self, l,r):
        self.l =l
        self.r = r
    def get_str(self):
        return self.l + self.r
    def __eq__(self, other):
        if isinstance(other, Token):
            return self.l == other.l and self.r == other.r
        return False

    def __hash__(self):
        return hash(self.l + self.r)
    def __repr__(self):
        return f"PAIR CLASS: ({self.l}, {self.r})"

def file_to_string():
    string = ""
    with open("../data/infiniteJest.txt",'r') as file:
        while True:
            char = file.read(1)
            if not char:
                return string
            string += char
    return string

def clean_string(string, tokens):
    first = string[1]
    for i in range(1, len(string)):
        pair = Pair(first,string[i])
        if pair in tokens:

        



def add_word(string):
    dic = {}
    first =string[0]
    for i in range(1,len(string)):
        pair = Pair(first,string[i])
        if pair in dic:
            dic[pair]+=1
        else:
            dic[pair] = 1
        #pairs.append((first,i))
        first = string[i]
    max = 0
    token = None
    for i,x in dic.items():
        if x > max:
            token = i
            max = x
    return token

def main():
    string = file_to_string()
    add_word(string)

if __name__ == '__main__':
    main()


