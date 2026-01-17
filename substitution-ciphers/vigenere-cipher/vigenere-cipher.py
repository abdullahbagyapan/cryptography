

class VigenereCipher:

    def __init__(self, key, alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZ"):
        
        self.key = key
        self.alphabet = alphabet
        
        self.i = len(key)
        self.m = len(alphabet)

    def clean(self, text):
        """Keep only A-Z and convert to uppercase."""
        return ''.join(c for c in text.upper() if c in self.alphabet)

    def encrypt(self, plain_text):
        
        cipher_text = []

        for i, ch in enumerate(plain_text.upper()):
            if ch in self.alphabet:
                x = self.alphabet.index(ch)
                y = self.alphabet.index(self.key[i % self.i])

                c = (x + y) % self.m
                cipher_text.append(self.alphabet[c])
            else:
                cipher_text.append(ch)

        return "".join(cipher_text)

    def decrypt(self, cipher_text):

        plain_text = []

        for i,ch in enumerate(cipher_text.upper()):
            if ch in self.alphabet:
                c = self.alphabet.index(ch)
                y = self.alphabet.index(self.key[i % self.i])
                
                x = (c - y) % self.m
                plain_text.append(self.alphabet[x])
            else:
                plain_text.append(ch)

        return "".join(plain_text)


if __name__ == "__main__":


    vigenere_cipher = VigenereCipher("LEMON")

    message = "ATTACK AT DAWN"
    message = vigenere_cipher.clean(message)
    
    cipher_text = vigenere_cipher.encrypt(message)

    print(cipher_text)

    plain_text = vigenere_cipher.decrypt(cipher_text)

    print(plain_text)

    print(message == plain_text)
