

class CaesarCipher:

    def __init__(self, shift, alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZ"):

        self.shift = shift
        self.alphabet = alphabet
        self.m = len(alphabet)

    def encrypt(self, plain_text):

        cipher_text = []

        for ch in plain_text.upper():
            if ch in self.alphabet:
                x = self.alphabet.index(ch)

                c = (x + self.shift) % self.m
                cipher_text.append(self.alphabet[c])
            else:
                cipher_text.append(ch)

        return "".join(cipher_text)

    def decrypt(self, cipher_text):

        plain_text = []

        for ch in cipher_text.upper():
            if ch in self.alphabet:
                c = self.alphabet.index(ch)

                x = (c - self.shift) % self.m
                plain_text.append(self.alphabet[x])
            else:
                plain_text.append(ch)

        return "".join(plain_text)



if __name__ == "__main__":

    caesar_cipher = CaesarCipher(-3)

    message = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG"

    cipher_text = caesar_cipher.encrypt(message)

    print(cipher_text)

    plain_text = caesar_cipher.decrypt(cipher_text)

    print(plain_text)

    print(plain_text == message)
