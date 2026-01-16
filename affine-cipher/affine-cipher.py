import math

class AffineCipher:

    def __init__(self, a, b, alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"):

        m = len(alphabet)

        if (math.gcd(a, m) != 1):
            raise ValueError("a has no multiplicative inverse of mod m")

        self.alphabet = alphabet

        self.a = a
        self.b = b
        self.m = m

    def encrypt(self, plain_text):

        cipher_text = []
        
        for ch in plain_text.upper():
            
            if ch in self.alphabet:
                x = self.alphabet.index(ch)
                y = (self.a * x + self.b) % self.m
                cipher_text.append(self.alphabet[y])
            else:
                cipher_text.append(ch)
        return "".join(cipher_text)

    def decrypt(self, cipher_text):

        def modinv(a, m):
            return pow(a, -1, m)
        
        a_inv = modinv(self.a, self.m)
        
        plain_text = []

        for ch in cipher_text.upper():
            if ch in self.alphabet:
                y = self.alphabet.index(ch)
                x = (a_inv * (y - self.b)) % self.m
                plain_text.append(self.alphabet[x])
            else:
                plain_text.append(ch)
        return "".join(plain_text)
    

if __name__ == "__main__":

    affine_cipher = AffineCipher(23, 7)

    message = "HELLOWORLD"

    cipher_text = affine_cipher.encrypt(message)
    print(cipher_text)

    plain_text = affine_cipher.decrypt(cipher_text)

    print(plain_text)

    print(plain_text == message)