# Homomorphic-Demonstrations
This repo contains 4 examples demonstrating [Homomorphic Encryption](https://en.wikipedia.org/wiki/Homomorphic_encryption#:~:text=Homomorphic%20encryption%20is%20a%20form,performed%20on%20the%20unencrypted%20data.) (Note that all results are mod 53):
- A calculator for +, -, *, and / 
- Dot product of 2 input vectors
- Matrix Multiplication
- Evaluation of a convolutional filter.
## How to run
- Clone the repo
- In the project root run `./bootstrap.sh` (Grab a coffee, this will take a while)
- `cd source/`
- `cmake .`
- `make`
- `../deps/bin/Homomorphic\ Demonstrations`
