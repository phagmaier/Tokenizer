from collections import Counter
#token, max_count = max(dic.items(), key=lambda x: x[1])

class Token:
    def __init__(self,l,r):
        self.l = l
        self.r = r
        self.string = l +r
    def __hash__(self):
        return hash((self.string))

    def __eq__(self, other):
        if isinstance(other, Token):
            return other.string == self.string
        if isinstance(other, str):
            return other == self.string
        return False
    def __repr__(self):
        return f"TOKEN: L: {self.l} R: {self.r}"

class T_Node:
    def __init__(self, token, count):
        self.token = token
        self.count = count
        self.right = None
        self.left = None
    def __lt__(self, other):
        return self.count < other.count
    def __gt__(self, other):
        return self.count > other.count
    def __eq__(self, other):
        return self.count == other.count and self.token == other.token
        return False
    def __repr__(self):
        return f"NODE: token: {self.token} COUNT: {self.count}"


class BST:
    def __init__(self):
        self.root = None
    
    def __insert__(self, item):
        if not self.root:
            self.root = item
            return
        parent = self.root
        curr = self.root
        dir = None
        while(curr):
            if curr == item:
                return
            if item > curr:
                parent = curr 
                curr = curr.right
                dir = 'right'
            else:
                parent = curr
                curr = curr.left
                dir = 'left'
        if not dir:
            print("ERROR  IN INSERT")
            return
        if dir=='left':
            parent.left = item
        else:
            parent.right = item

        def delete(self, item):
            if not self.root:
                print("BST EMPTY")
                return
            if self.root == item:
                pass
            curr = self.root 
            parent = self.root
            while(curr):
                if curr == item:





def file_to_text():
    with open("../data/infiniteJest.txt", 'r', encoding="utf-8") as file:
        return file.read()

def text_to_tokens(text):
    dic = Counter()
    for i in range(len(text)-1):
        dic[Token(text[i],text[i+1])] +=1


def main():
    text = file_to_text()
   
    
if __name__ == '__main__':
    main()

