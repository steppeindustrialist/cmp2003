# CMP2003-Movie Rating Model

An UBCF based movie rating predictor

## Development Prerequisites

- [git](https://git-scm.com/) - For cloning project
- [g++ compiler](https://gcc.gnu.org) - For creating executable binary
- [nodejs](https://nodejs.org/en) - To convert datasets

### To run the program locally

```bash
git clone git@github.com:steppeindustrialist/cmp2003.git &&
cd cmp2003 &&
node convert.js &&
g++ -std=c++17 -O2 main.cpp -o recommender &&
./recommender < combined_dataset.txt
```
