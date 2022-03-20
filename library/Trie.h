#include <cstdint>
#include <vector>
#include <map>
#include <algorithm>
#include "Node.h"
#include "Symbol.h"
#include "BitReader.h"

class Trie {
public:
    Trie() {}

    Trie(NodePtr root);

    std::vector<Symbol> codes;

    void AddWord(const Symbol &symbol);

    Trie(std::vector<Symbol>& alphabet);

    void TraverseTrie(NodePtr cur, uint64_t current_code, size_t current_code_len);

    std::vector<Symbol> GetCodes();

    NodePtr GetRoot();

    uint16_t GetNextValueInStream(BitReader &in);


private:
    NodePtr root_ = std::make_shared<Node>();

};